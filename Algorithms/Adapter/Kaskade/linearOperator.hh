#ifndef ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH
#define ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH

#include <utility>

#include "../../vector.hh"
#include "Util/Base/operatorBase.hh"

#include "directSolver.hh"
#include "util.hh" // copy

namespace Algorithm
{
  namespace Kaskade
  {
    /**
     * @ingroup KaskadeGroup
     * @brief Linear operator interface for operators in %Kaskade 7.
     * @tparam OperatorImpl %Kaskade 7 operator, i.e. ::Kaskade::AssembledGalerkinOperator or ::Kaskade::MatrixRepresentedOperator.
     * @tparam AnsatzVariableSetDescription ::Kaskade::VariableSetDescription for ansatz variables
     * @tparam TestVariableSetDescription ::Kaskade::VariableSetDescription for test variables
     * @see LinearOperator, LinearOperatorConcept
     */
    template <class OperatorImpl, class AnsatzVariableSetDescription, class TestVariableSetDescription>
    class LinearOperator : public OperatorBase
    {
      using Spaces = typename AnsatzVariableSetDescription::Spaces;
      using Variables = typename AnsatzVariableSetDescription::Variables;
      using Domain = typename AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Range = typename TestVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Matrix = ::Kaskade::MatrixAsTriplet<double>;

    public:
      /**
       * @brief Construct linear operator for %Kaskade 7.
       * @param A operator from %Kaskade 7
       * @param domain domain space
       * @param range range space
       */
      LinearOperator(const OperatorImpl& A, const VectorSpace& domain_, const VectorSpace& range_)
        : OperatorBase(domain_,range_),
          A_(A),
          spaces_( extractSpaces<AnsatzVariableSetDescription>(domain()) )
      {}

      /**
       * @brief Compute \f$A(x)\f$.
       */
      ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const
      {
        Domain x_( AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<AnsatzVariableSetDescription>(x,x_);
        Range y_( TestVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

        A_.apply(x_,y_);

        auto y = range().element();
        copyFromCoefficientVector<TestVariableSetDescription>(y_,y);

        return y;
      }
      /**
       * @brief Compute \f$A'(x)dx = A(dx)\f$.
       */
      ::Algorithm::Vector d1(const ::Algorithm::Vector&, const ::Algorithm::Vector& dx) const
      {
        Domain dx_( AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<AnsatzVariableSetDescription>(dx,dx_);
        Range y_( TestVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

        A_.apply( dx_ , y_ );

        auto y = range().element();
        copyFromCoefficientVector<TestVariableSetDescription>(y_,y);

        return y;
      }

      /// Access solver.
      const LinearSolver& solver() const
      {
        return DirectSolver<OperatorImpl,AnsatzVariableSetDescription,TestVariableSetDescription>( A_ , spaces_, range() , domain() );
      }

    private:
      OperatorImpl A_;
      Spaces spaces_;
    };

    /**
     * @ingroup KaskadeGroup
     * @brief Convenient generation of a linear operator for %Kaskade 7.
     * @param A operator from %Kaskade 7
     * @param domain domain space
     * @param range range space
     * @tparam OperatorImpl %Kaskade 7 operator, i.e. ::Kaskade::AssembledGalerkinOperator or ::Kaskade::MatrixRepresentedOperator.
     * @tparam AnsatzVariableSetDescription ::Kaskade::VariableSetDescription for ansatz variables
     * @tparam TestVariableSetDescription ::Kaskade::VariableSetDescription for test variables
     * @return LinearOperator<OperatorImpl, AnsatzVariableSetDescription, TestVariableSetDescription>( A , domain , range )
     */
    template <class AnsatzVariableSetDescription, class TestVariableSetDescription, class OperatorImpl>
    auto makeLinearOperator(const OperatorImpl& A, const VectorSpace& domain, const VectorSpace& range)
    {
      return LinearOperator<OperatorImpl, AnsatzVariableSetDescription, TestVariableSetDescription>( A , domain , range );
    }
  }
}

#endif // ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH
