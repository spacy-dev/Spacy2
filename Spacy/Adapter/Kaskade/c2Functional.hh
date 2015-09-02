#ifndef ALGORITHM_OPERATORS_KASKADE_C2_FUNCTIONAL_HH
#define ALGORITHM_OPERATORS_KASKADE_C2_FUNCTIONAL_HH

#include <memory>

#include "fem/assemble.hh"
#include "fem/istlinterface.hh"

#include "Spacy/operator.hh"
#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"
#include "Spacy/hessian.hh"
#include "Spacy/Util/Mixins/numberOfThreads.hh"
#include "Spacy/Util/Base/functionalBase.hh"

#include "directSolver.hh"
#include "vectorSpace.hh"
#include "vector.hh"

namespace Spacy
{
  namespace Kaskade
  {
    /**
     * @ingroup KaskadeGroup
     * @brief %Functional interface for %Kaskade 7. Models a twice differentiable functional \f$f:X\rightarrow \mathbb{R}\f$.
     * @tparam FunctionalDefinition functional definition from %Kaskade 7
     * @see @ref C2FunctionalAnchor "::Spacy::C2Functional", @ref C2FunctionalConceptAnchor "C2FunctionalConcept"
     */
    template <class FunctionalDefinition>
    class C2Functional :
        public C2FunctionalBase< C2Functional<FunctionalDefinition> > ,
        public Mixin::NumberOfThreads
    {
    public:
      /// %Kaskade::VariableSetDescription
      using VariableSetDescription = typename FunctionalDefinition::AnsatzVars;
      /// Coefficient vector type.
      using CoefficientVector = typename VariableSetDescription::template CoefficientVectorRepresentation<>::type;
      /// boost::fusion::vector<const Space0*,const Space1*,...>
      using Spaces = typename VariableSetDescription::Spaces;
      /// %Kaskade::VariationalFunctionalAssembler
      using Assembler = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt<FunctionalDefinition> >;
      /// Matrix type
      using Matrix = ::Kaskade::MatrixAsTriplet<double>;
      /// operator for the description of the second derivative
      using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix,CoefficientVector,CoefficientVector>;

      /**
       * @brief Construct a twice differentiable functional \f$f: X\rightarrow \mathbb{R}\f$ from %Kaskade 7.
       * @param f operator definition from %Kaskade 7
       * @param domain domain space
       * @param rbegin first row to be considered in the definition of f
       * @param rend one after the last row to be considered in the definition of f
       * @param cbegin first column to be considered in the definition of f
       * @param cend one after the last column to be considered in the definition of f
       *
       * The optional parameters rbegin, rend, cbegin and cend can be used to define operators that correspond to parts of
       * a system of equation.
       */
      C2Functional(const FunctionalDefinition& f, const VectorSpace& domain,
                   int rbegin = 0, int rend = FunctionalDefinition::AnsatzVars::noOfVariables,
                   int cbegin = 0, int cend = FunctionalDefinition::TestVars::noOfVariables)
        : C2FunctionalBase< C2Functional<FunctionalDefinition> >(domain),
          f_(f),
          spaces_( extractSpaces<VariableSetDescription>(domain) ),
          assembler_(spaces_),
          rbegin_(rbegin), rend_(rend), cbegin_(cbegin), cend_(cend)
      {}

      /**
       * @brief Copy constructor.
       * @param g functional to copy from
       */
      C2Functional(const C2Functional& g)
        : C2FunctionalBase< C2Functional<FunctionalDefinition> >(g.domain()),
          NumberOfThreads(g),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(spaces_),
          A_(g.A_),
          old_X_f_(g.old_X_f_),
          old_X_df_(g.old_X_df_),
          old_X_ddf_(g.old_X_ddf_),
          onlyLowerTriangle_(g.onlyLowerTriangle_),
          rbegin_(g.rbegin_), rend_(g.rend_), cbegin_(g.cbegin_), cend_(g.cend_),
          solverCreator_(g.solverCreator_)
      {}

      /**
       * @brief Copy assignment.
       * @param g functional to copy from
       */
      C2Functional& operator=(const C2Functional& g)
      {
        setNumberOfThreads(g.nThreads());
        f_ = g.f_;
        spaces_ = g.spaces_;
        assembler_ = Assembler(spaces_);
        A_ = g.A_;
        old_X_f_ = g.old_X_f_;
        old_X_df_ = g.old_X_df_;
        old_X_ddf_ = g.old_X_ddf_;
        onlyLowerTriangle_ = g.onlyLowerTriangle_;
        rbegin_ = g.rbegin_;
        rend_ = g.rend_;
        cbegin_ = g.cbegin_;
        cend_ = g.cend_;
        solverCreator_ = g.solverCreator_;
      }

      /**
       * @brief Move constructor.
       * @param g functional to move from
       */
      C2Functional(C2Functional&& g) = default;

      /**
       * @brief Move assignment.
       * @param g functional to move from
       */
      C2Functional& operator=(C2Functional&& g) = default;

      /**
       * @brief Apply functional.
       * @param x argument
       * @return \f$f(x)\f$
       */
      double operator()(const ::Spacy::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&C2Functional::assembleFunctional,std::ref(*this), std::placeholders::_1),x);

        return assembler().functional();
      }

      /**
       * @cond
       * @brief Compute first directional derivative \f$f'(x):\ X \rightarrow X^* \f$.
       *
       * Actual implementation of d1 is provided in base class C2FunctionalBase.
       * @see C2FunctionalBase
       */
      ::Spacy::Vector d1_(const ::Spacy::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&C2Functional::assembleGradient,std::ref(*this), std::placeholders::_1),x);

        CoefficientVector v( assembler_.rhs() );

        auto y = this->domain().dualSpace().vector();
        copyFromCoefficientVector<VariableSetDescription>(v,y);
        return y;
      }

      /**
       * @brief Compute second directional derivative \f$f''(x):\ X \rightarrow X^* \f$.
       *
       * Actual implementation of d2 is provided in base class C2FunctionalBase.
       * @see C2FunctionalBase
       */
      ::Spacy::Vector d2_(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const
      {
        primalDualIgnoreReset(std::bind(&C2Functional::assembleHessian,std::ref(*this), std::placeholders::_1),x);

        CoefficientVector dx_( VariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<VariableSetDescription>(dx,dx_);
        CoefficientVector y_( VariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

        A_.apply( dx_ , y_ );

        auto y = this->domain().dualSpace().vector();
        copyFromCoefficientVector<VariableSetDescription>(y_,y);

        return y;
      }
      /// @endcond

      /**
       * @brief Access \f$f''(x)\f$ as linear operator \f$X\rightarrow X^*\f$.
       * @param x point of linearization
       * @see Hessian, @ref LinearOperatorAnchor "LinearOperator", @ref LinearOperatorConceptAnchor "LinearOperatorConcept"
       */
      Hessian hessian(const ::Spacy::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&C2Functional::assembleHessian,std::ref(*this), std::placeholders::_1),x);

        return Hessian( *this , x , solverCreator_(*this) );
      }

      /**
       * @brief Access assembler.
       * @return object of type %Kaskade::VariationalFunctionalAssembler<...>
       */
      Assembler& assembler()
      {
        return assembler_;
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
       * @brief Access operator representing \f$f''\f$.
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
      void setSolverCreator(std::function<LinearSolver(const C2Functional&)> f)
      {
        solverCreator_ = std::move(f);
      }

    private:
      /// Assemble \f$f(x)\f$.
      void assembleFunctional(const ::Spacy::Vector& x) const
      {
        if( ( (assembler_.valid() & Assembler::VALUE) != 0 ) && (old_X_f_==x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::VALUE , nThreads() );

        old_X_f_ = x;
      }

      /// Assemble discrete representation of \f$f'(x)\f$.
      void assembleGradient(const ::Spacy::Vector& x) const
      {
        if( ( (assembler_.valid() & Assembler::RHS) != 0 ) && (old_X_df_==x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::RHS , nThreads() );

        old_X_df_ = x;
      }

      /// Assemble discrete representation of \f$f''(x)\f$.
      void assembleHessian(const ::Spacy::Vector& x) const
      {
        if( ( (assembler_.valid() & Assembler::MATRIX) != 0 ) && (old_X_ddf_==x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::MATRIX , nThreads() );
        A_ = KaskadeOperator( assembler().template get<Matrix>(onlyLowerTriangle_,rbegin_,rend_,cbegin_,cend_) );

        old_X_ddf_ = x;
      }

      FunctionalDefinition f_;
      Spaces spaces_;
      mutable Assembler assembler_;
      mutable KaskadeOperator A_ = {};
      mutable ::Spacy::Vector old_X_f_ = {}, old_X_df_ = {}, old_X_ddf_ = {};
      bool onlyLowerTriangle_ = false;
      int rbegin_=0, rend_=FunctionalDefinition::AnsatzVars::noOfVariables;
      int cbegin_=0, cend_=FunctionalDefinition::TestVars::noOfVariables;
      std::function<LinearSolver(const C2Functional&)> solverCreator_ = [](const C2Functional& f)
        {
            return makeDirectSolver<VariableSetDescription,VariableSetDescription>( f.A() , f.spaces(),
                                                                                     f.domain().dualSpace() ,
                                                                                     f.domain() ,
                                                                                     DirectType::MUMPS ,
                                                                                     MatrixProperties::GENERAL );

        };
    };

    /**
     * @ingroup KaskadeGroup
     * @brief Convenient generation of a twice differentiable functional \f$f: X\rightarrow \mathbb{R}\f$ for %Kaskade 7.
     * @param f operator definition from %Kaskade 7
     * @param domain domain space
     * @param rbegin first row to be considered in the definition of f
     * @param rend one after the last row to be considered in the definition of f
     * @param cbegin first column to be considered in the definition of f
     * @param cend one after the last column to be considered in the definition of f
     * @return @ref C2Functional "::Spacy::Kaskade::C2Functional<FunctionalDefinition>( f, domain, rbegin, rend, cbegin, cend )"
     *
     * The optional parameters rbegin, rend, cbegin and cend can be used to define operators that correspond to parts of
     * a system of equation.
     */
    template <class FunctionalDefinition>
    auto makeC2Functional(const FunctionalDefinition& f, const VectorSpace& domain,
                        int rbegin = 0, int rend = FunctionalDefinition::AnsatzVars::noOfVariables,
                        int cbegin = 0, int cend = FunctionalDefinition::TestVars::noOfVariables)
    {
      return C2Functional<FunctionalDefinition>( f, domain , rbegin, rend , cbegin , cend );
    }
  }
}

#endif // ALGORITHM_OPERATORS_KASKADE_C2_FUNCTIONAL_HH
