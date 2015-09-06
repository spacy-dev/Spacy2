#ifndef SPACY_KASKADE_OPERATOR_HH
#define SPACY_KASKADE_OPERATOR_HH

#include <utility>

#include "fem/assemble.hh"
#include "fem/istlinterface.hh"

#include "Spacy/Util/Base/operatorBase.hh"
#include "Spacy/Util/Mixins/numberOfThreads.hh"

namespace Spacy
{
  class Spacy;

  namespace Kaskade
  {
    /**
     * @ingroup KaskadeGroup
     * @brief %Operator interface for %Kaskade 7. Models an operator \f$A:X\rightarrow Y\f$.
     *
     * @see @ref OperatorAnchor "Operator", @ref OperatorConceptAnchor "OperatorConcept"
     */
    template <class OperatorDefinition>
    class Operator :
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
       *
       * The optional parameters rbegin, rend, cbegin and cend can be used to define operators that correspond to parts of
       * a system of equation.
       */
      Operator(const OperatorDefinition& f, const VectorSpace& domain, const VectorSpace& range)
        : OperatorBase(domain,range),
          f_(f),
          spaces_( extractSpaces<AnsatzVariableSetDescription>(domain) ),
          assembler_(spaces_)
      {}

      /**
       * @brief Copy constructor.
       * @param B object to copy from
       */
      Operator(const Operator& B)
        : OperatorBase(B),
          NumberOfThreads(B),
          f_(B.f_), spaces_(B.spaces_),
          assembler_(spaces_),
          old_X_A_(B.old_X_A_)
      {}

      /**
       * @brief Copy assignment.
       * @param B object to copy from
       */
      Operator& operator=(const Operator& B)
      {
        setNumberOfThreads(B.nThreads());
        f_ = B.f_;
        spaces_ = B.spaces_;
        assembler_ = Assembler(spaces_);
        old_X_A_ = B.old_X_A_;
      }

      /**
       * @brief Move constructor.
       * @param B object to move from
       */
      Operator(Operator&& B) = default;

      /**
       * @brief Move assignment.
       * @param B object to move from
       */
      Operator& operator=(Operator&& B) = default;

      /**
       * @brief Apply operator.
       * @param x argument
       * @return \f$A(x)\f$
       */
      ::Spacy::Vector operator()(const ::Spacy::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleOperator,std::ref(*this), std::placeholders::_1),x);

        VectorImpl v( assembler_.rhs() );

        auto y = range().vector();
        copyFromCoefficientVector<TestVariableSetDescription>(v,y);
        return y;
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

      OperatorDefinition f_;
      Spaces spaces_;
      mutable Assembler assembler_;
      mutable ::Spacy::Vector old_X_A_;
    };

    /**
     * @ingroup KaskadeGroup
     * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow Y\f$ from %Kaskade 7.
     * @param f operator definition from %Kaskade 7
     * @param domain domain space \f$X\f$
     * @param range range space \f$Y\f$
     * @return @ref Operator "::Spacy::Kaskade::Operator<OperatorDefinition>( A , domain , range  )"
     */
    template <class OperatorDefinition>
    auto makeOperator(const OperatorDefinition& f, const VectorSpace& domain, const VectorSpace& range)
    {
      return Operator<OperatorDefinition>( f, domain , range );
    }

    /**
     * @ingroup KaskadeGroup
     * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow X^*\f$ from %Kaskade 7.
     * @param f operator definition from %Kaskade 7
     * @param domain domain space \f$X\f$
     * @return @ref Operator "::Spacy::Kaskade::Operator<OperatorDefinition>( A , domain , range )"
     */
    template <class OperatorDefinition>
    auto makeOperator(const OperatorDefinition& f, const VectorSpace& domain)
    {
      return Operator<OperatorDefinition>( f, domain , domain.dualSpace() );
    }
  }
}

#endif // SPACY_KASKADE_OPERATOR_HH
