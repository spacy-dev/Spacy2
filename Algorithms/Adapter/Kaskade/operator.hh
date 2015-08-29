#ifndef ALGORITHM_KASKADE_OPERATOR_HH
#define ALGORITHM_KASKADE_OPERATOR_HH

#include <utility>

#include "fem/assemble.hh"
#include "fem/istlinterface.hh"

#include "Algorithms/Util/Base/operatorBase.hh"
#include "Algorithms/Util/Mixins/disableAssembly.hh"
#include "Algorithms/Util/Mixins/numberOfThreads.hh"

#include "Algorithms/linearizedOperator.hh"
#include "directSolver.hh"

namespace Algorithm
{
  class VectorSpace;

  namespace Kaskade
  {
    /**
     * @ingroup KaskadeGroup
     * @brief Operator interface for %Kaskade 7. Models a differentiable operator \f$A:X\rightarrow Y\f$.
     *
     * @see C1Operator, C1OperatorConcept
     */
    template <class OperatorDefinition>
    class Operator :
        public OperatorBase ,
        public Mixin::DisableAssembly ,
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
      Operator(const OperatorDefinition& f, const VectorSpace& domain, const VectorSpace& range,
               int rbegin = 0, int rend = OperatorDefinition::AnsatzVars::noOfVariables,
               int cbegin = 0, int cend = OperatorDefinition::TestVars::noOfVariables)
        : OperatorBase(domain,range),
          f_(f),
          spaces_( extractSpaces<AnsatzVariableSetDescription>(domain) ),
          assembler_(spaces_),
          rbegin_(rbegin), rend_(rend), cbegin_(cbegin), cend_(cend)
      {}

      /// Copy constructor.
      Operator(const Operator& B)
        : OperatorBase(B),
          DisableAssembly(B),
          NumberOfThreads(B),
          f_(B.f_), spaces_(B.spaces_),
          assembler_(spaces_),
          A_(B.A_),
          old_X_A_(B.old_X_A_),
          old_X_dA_(B.old_X_dA_),
          onlyLowerTriangle_(B.onlyLowerTriangle_),
          rbegin_(B.rbegin_), rend_(B.rend_), cbegin_(B.cbegin_), cend_(B.cend_)
      {}

      /// Copy assignment.
      Operator& operator=(const Operator& B)
      {
        disableAssembley(B.assemblyIsDisabled());
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

      /// Move constructor.
      Operator(Operator&&) = default;

      /// Move assignment.
      Operator& operator=(Operator&&) = default;

      /**
       * @brief Compute \f$A(x)\f$.
       */
      ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleOperator,std::ref(*this), std::placeholders::_1),x);

        VectorImpl v( assembler_.rhs() );

        auto y = range().vector();
        copyFromCoefficientVector<TestVariableSetDescription>(v,y);
        return y;
      }

      /**
       * @brief Compute \f$A'(x)dx\f$.
       */
      ::Algorithm::Vector d1(const ::Algorithm::Vector& x, const ::Algorithm::Vector& dx) const
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);

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
       * @see LinearizedOperator, LinearOperator, LinearOperatorConcept
       */
      ::Algorithm::LinearOperator linearization(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);
        auto newOp = *this;
        newOp.disableAssembly();
        return LinearizedOperator(std::move(newOp),x,
                                  DirectSolver<KaskadeOperator,AnsatzVariableSetDescription,TestVariableSetDescription>
                                    ( A_ , spaces_, range() , domain() )
                                  );
      }

    private:
      /// Assemble discrete representation of \f$A(x)\f$.
      void assembleOperator(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( ( (assembler_.valid() & Assembler::RHS) != 0 ) && (old_X_A_==x) ) return;

        AnsatzVariableSetDescription variableSet(spaces_);
        typename AnsatzVariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::RHS , nThreads() );

        old_X_A_ = x;
      }

      /// Assemble discrete representation of \f$A'(x)\f$.
      void assembleGradient(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;
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
      mutable ::Algorithm::Vector old_X_A_ = {}, old_X_dA_ = {};
      bool onlyLowerTriangle_ = false;
      int rbegin_=0, rend_=OperatorDefinition::AnsatzVars::noOfVariables;
      int cbegin_=0, cend_=OperatorDefinition::TestVars::noOfVariables;
    };

    /**
     * @ingroup KaskadeGroup
     * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow X\f$ from %Kaskade 7.
     * @param f operator definition from %Kaskade 7
     * @param domain domain space
     * @param range range space
     * @param rbegin first row to be considered in the definition of f
     * @param rend one after the last row to be considered in the definition of f
     * @param cbegin first column to be considered in the definition of f
     * @param cend one after the last column to be considered in the definition of f
     * @return ::Algorithm::Kaskade::Operator<OperatorDefinition>( f , domain , range , rbegin , rend , cbegin , cend )
     *
     * The optional parameters rbegin, rend, cbegin and cend can be used to define operators that correspond to parts of
     * a system of equation.
     */
    template <class OperatorDefinition>
    auto makeOperator(const OperatorDefinition& f, const VectorSpace& domain, const VectorSpace& range,
                      int rbegin = 0, int rend = OperatorDefinition::AnsatzVars::noOfVariables,
                      int cbegin = 0, int cend = OperatorDefinition::TestVars::noOfVariables)
    {
      return Operator<OperatorDefinition>( f, domain , range , rbegin , rend , cbegin , cend);
    }
  }
}

#endif // ALGORITHM_KASKADE_OPERATOR_HH
