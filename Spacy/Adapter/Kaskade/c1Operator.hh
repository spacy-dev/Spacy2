#ifndef SPACY_KASKADE_C1_OPERATOR_HH
#define SPACY_KASKADE_C1_OPERATOR_HH

#include <utility>

#include "fem/assemble.hh"
#include "fem/istlinterface.hh"

#include "Spacy/Util/Base/operatorBase.hh"
#include "Spacy/Util/Mixins/numberOfThreads.hh"

#include "Spacy/linearizedOperator.hh"
#include "directSolver.hh"

namespace Spacy
{
  /// @cond
  class VectorSpace;
  /// @endcond

  namespace Kaskade
  {
    /**
     * @ingroup KaskadeGroup
     * @brief %Operator interface for %Kaskade 7. Models a differentiable operator \f$A:X\rightarrow Y\f$.
     *
     * @see @ref C1OperatorAnchor "C1Operator", @ref C1OperatorConceptAnchor "C1OperatorConcept"
     */
    template <class OperatorDefinition>
    class C1Operator :
        public OperatorBase ,
        public Mixin::NumberOfThreads
    {
      using AnsatzVariableSetDescription = typename OperatorDefinition::AnsatzVars;
      using TestVariableSetDescription = typename OperatorDefinition::TestVars;
      using VectorImpl = typename AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Spaces = typename AnsatzVariableSetDescription::Spaces;
      using Assembler = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt<OperatorDefinition> >;
      using Domain = typename Assembler::AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Range = typename Assembler::TestVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Matrix = ::Kaskade::MatrixAsTriplet<double>;
      using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain,Range>;

    public:
      /**
       * @brief Construct operator for %Kaskade 7.
       * @param f operator definition from %Kaskade 7
       * @param domain domain space
       * @param range range space
       * @param rbegin first row to be considered in the definition of f
       * @param rend one after the last row to be considered in the definition of f
       * @param cbegin first column to be considered in the definition of f
       * @param cend one after the last column to be considered in the definition of f
       *
       * The optional parameters rbegin, rend, cbegin and cend can be used to define operators that correspond to parts of
       * a system of equation.
       */
      C1Operator(const OperatorDefinition& f, const VectorSpace& domain, const VectorSpace& range,
                 int rbegin = 0, int rend = OperatorDefinition::AnsatzVars::noOfVariables,
                 int cbegin = 0, int cend = OperatorDefinition::TestVars::noOfVariables)
        : OperatorBase(domain,range),
          f_(f),
          spaces_( extractSpaces<AnsatzVariableSetDescription>(domain) ),
          assembler_(spaces_),
          rbegin_(rbegin), rend_(rend), cbegin_(cbegin), cend_(cend)
      {}

      /**
       * @brief Copy constructor.
       * @param B object to copy from
       */
      C1Operator(const C1Operator& B)
        : OperatorBase(B),
          NumberOfThreads(B),
          f_(B.f_), spaces_(B.spaces_),
          assembler_(spaces_),
          A_(B.A_),
          old_X_A_(B.old_X_A_),
          old_X_dA_(B.old_X_dA_),
          onlyLowerTriangle_(B.onlyLowerTriangle_),
          rbegin_(B.rbegin_), rend_(B.rend_), cbegin_(B.cbegin_), cend_(B.cend_)
      {}

      /**
       * @brief Copy assignment.
       * @param B object to copy from
       */
      C1Operator& operator=(const C1Operator& B)
      {
        setNumberOfThreads(B.nThreads());
        f_ = B.f_;
        spaces_ = B.spaces_;
        assembler_ = Assembler(spaces_);
        A_ = B.A_;
        old_X_A_ = B.old_X_A_;
        old_X_dA_ = B.old_X_dA_;
        onlyLowerTriangle_ = B.onlyLowerTriangle_;
        rbegin_ = B.rbegin_;
        rend_ = B.rend_;
        cbegin_ = B.cbegin_;
        cend_ = B.cend_;
      }

      /**
       * @brief Move constructor.
       * @param B object to move from
       */
      C1Operator(C1Operator&& B) = default;

      /**
       * @brief Move assignment.
       * @param B object to move from
       */
      C1Operator& operator=(C1Operator&& B) = default;

      /**
       * @brief Apply operator.
       * @param x argument
       * @return \f$A(x)\f$
       */
      ::Spacy::Vector operator()(const ::Spacy::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&C1Operator::assembleOperator,std::ref(*this), std::placeholders::_1),x);

        VectorImpl v( assembler_.rhs() );

        auto y = range().vector();
        copyFromCoefficientVector<TestVariableSetDescription>(v,y);
        return y;
      }

      /**
       * @brief Compute \f$A'(x)dx\f$.
       * @param x current iterate
       * @param dx correction
       * @return \f$A'(x)dx\f$
       */
      ::Spacy::Vector d1(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const
      {
        primalDualIgnoreReset(std::bind(&C1Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);

        VectorImpl dx_( AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<AnsatzVariableSetDescription>(dx,dx_);
        VectorImpl y_( TestVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

        A_.apply( dx_ , y_ );

        auto y = range().vector();
        copyFromCoefficientVector<TestVariableSetDescription>(y_,y);

        return y;
      }

      /**
       * @brief Access \f$A'(x)\f$ as linear operator \f$X\rightarrow Y\f$
       * @param x point of linearization
       * @see LinearizedOperator, @ref LinearOperatorAnchor "LinearOperator", @ref LinearOperatorConceptAnchor "LinearOperatorConcept"
       */
      ::Spacy::LinearOperator linearization(const ::Spacy::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&C1Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);
        return LinearizedOperator( *this , x , solverCreator_(*this) );
      }

      /**
       * @brief Access assembler.
       * @return object of type %Kaskade::VariationalFunctionalAssembler<...>
       */
      const Assembler& assembler() const noexcept
      {
        return assembler_;
      }

      /**
       * @brief Access operator representing \f$A'(x)\f$.
       */
      const KaskadeOperator& A() const noexcept
      {
        return A_;
      }

      /**
       * @brief Access boost::fusion::vector of pointers to spaces.
       */
      const Spaces& spaces() const noexcept
      {
        return spaces_;
      }

      /**
       * @brief Access onlyLowerTriangle flag.
       * @return true if only the lower triangle of a symmetric matrix is stored in the operator definition, else false
       */
      bool onlyLowerTriangle() const noexcept
      {
        return onlyLowerTriangle_;
      }

      /**
       * @brief Change solver creator.
       * @param f function/functor for the creation of a linear solver
       */
      void setSolverCreator(std::function<LinearSolver(const C1Operator&)> f)
      {
        solverCreator_ = std::move(f);
      }

    private:
      /// Assemble discrete representation of \f$A(x)\f$.
      void assembleOperator(const ::Spacy::Vector& x) const
      {
        if( ( (assembler_.valid() & Assembler::RHS) != 0 ) && (old_X_A_==x) ) return;

        AnsatzVariableSetDescription variableSet(spaces_);
        typename AnsatzVariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::RHS , nThreads() );

        old_X_A_ = x;
      }

      /// Assemble discrete representation of \f$A'(x)\f$.
      void assembleGradient(const ::Spacy::Vector& x) const
      {
        if( ( (assembler_.valid() & Assembler::MATRIX) != 0 ) && (old_X_dA_==x) ) return;

        AnsatzVariableSetDescription variableSet(spaces_);
        typename AnsatzVariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::MATRIX , nThreads() );
        A_ = KaskadeOperator( assembler_.template get<Matrix>(onlyLowerTriangle_,rbegin_,rend_,cbegin_,cend_) );
        old_X_dA_ = x;
      }

      OperatorDefinition f_;
      Spaces spaces_;
      mutable Assembler assembler_;
      mutable KaskadeOperator A_ = {};
      mutable ::Spacy::Vector old_X_A_ = {}, old_X_dA_ = {};
      bool onlyLowerTriangle_ = false;
      int rbegin_=0, rend_=OperatorDefinition::AnsatzVars::noOfVariables;
      int cbegin_=0, cend_=OperatorDefinition::TestVars::noOfVariables;
      std::function<LinearSolver(const C1Operator&)> solverCreator_ = [](const C1Operator& M)
        {
            return makeDirectSolver<TestVariableSetDescription,AnsatzVariableSetDescription>( M.A() , M.spaces(),
                                                                                              M.range() ,
                                                                                              M.domain() ,
                                                                                              DirectType::MUMPS ,
                                                                                              MatrixProperties::GENERAL );

        };
    };

    /**
     * @ingroup KaskadeGroup
     * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow Y\f$ from %Kaskade 7.
     * @param f operator definition from %Kaskade 7
     * @param domain domain space \f$X\f$
     * @param range range space \f$Y\f$
     * @param rbegin first row to be considered in the definition of A
     * @param rend one after the last row to be considered in the definition of A
     * @param cbegin first column to be considered in the definition of A
     * @param cend one after the last column to be considered in the definition of A
     * @return @ref C1Operator "::Spacy::Kaskade::C1Operator<OperatorDefinition>( A , domain , range , rbegin , rend , cbegin , cend )"
     *
     * The optional parameters rbegin, rend, cbegin and cend can be used to define operators that correspond to parts of
     * a system of equation.
     */
    template <class OperatorDefinition>
    auto makeC1Operator(const OperatorDefinition& f, const VectorSpace& domain, const VectorSpace& range,
                      int rbegin = 0, int rend = OperatorDefinition::AnsatzVars::noOfVariables,
                      int cbegin = 0, int cend = OperatorDefinition::TestVars::noOfVariables)
    {
      return C1Operator<OperatorDefinition>( f, domain , range , rbegin , rend , cbegin , cend);
    }

    /**
     * @ingroup KaskadeGroup
     * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow X^*\f$ from %Kaskade 7.
     * @param f operator definition from %Kaskade 7
     * @param domain domain space \f$X\f$
     * @param rbegin first row to be considered in the definition of A
     * @param rend one after the last row to be considered in the definition of A
     * @param cbegin first column to be considered in the definition of A
     * @param cend one after the last column to be considered in the definition of A
     * @return @ref C1Operator "::Spacy::Kaskade::C1Operator<OperatorDefinition>( A , domain , range , rbegin , rend , cbegin , cend )"
     *
     * The optional parameters rbegin, rend, cbegin and cend can be used to define operators that correspond to parts of
     * a system of equation.
     */
    template <class OperatorDefinition>
    auto makeC1Operator(const OperatorDefinition& f, const VectorSpace& domain,
                      int rbegin = 0, int rend = OperatorDefinition::AnsatzVars::noOfVariables,
                      int cbegin = 0, int cend = OperatorDefinition::TestVars::noOfVariables)
    {
      return C1Operator<OperatorDefinition>( f, domain , domain.dualSpace() , rbegin , rend , cbegin , cend);
    }
  }
}

#endif // SPACY_KASKADE_C1_OPERATOR_HH
