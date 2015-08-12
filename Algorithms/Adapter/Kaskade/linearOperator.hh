#ifndef ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH
#define ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH

#include <memory>
#include <utility>

#include "../../operator.hh"
#include "vectorSpace.hh"
#include "Interface/Operator/linearizedOperator.hh"
#include "Interface/Operator/abstractOperator.hh"
#include "Util/Mixins/disableAssembly.hh"
#include "Util/create.hh"

#include "directSolver.hh"
#include "vector.hh" // copy

namespace Algorithm
{
  namespace Kaskade
  {
    template <class OperatorImpl, class AnsatzVariableSetDescription, class TestVariableSetDescription>
    class LinearOperator : public Interface::AbstractOperator
    {
      using Spaces = typename AnsatzVariableSetDescription::Spaces;
      using Variables = typename AnsatzVariableSetDescription::Variables;
      using Domain = typename AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Range = typename TestVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Matrix = ::Kaskade::MatrixAsTriplet<double>;

    public:
      LinearOperator(const OperatorImpl& A,
               std::shared_ptr<Interface::AbstractVectorSpace> domain_,
               std::shared_ptr<Interface::AbstractVectorSpace> range_)
        : AbstractOperator(domain_,range_),
          A_(A),
          spaces_( extractSpaces<AnsatzVariableSetDescription>(domain()) )
      {}

      LinearOperator(const OperatorImpl& f, const ::Algorithm::VectorSpace& domain, const ::Algorithm::VectorSpace& range)
        : LinearOperator(f,domain.sharedImpl(),range.sharedImpl())
      {}

      std::unique_ptr<Interface::AbstractVector> operator()(const Interface::AbstractVector& x) const final override
      {
        Domain x_( AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<AnsatzVariableSetDescription>(x,x_);
        Range y_( TestVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

        A_.apply(x_,y_);

        auto y = range().element();
        copyFromCoefficientVector<TestVariableSetDescription>(y_,*y);

        return std::move(y);
      }

      std::unique_ptr<Interface::AbstractVector> d1(const Interface::AbstractVector&, const Interface::AbstractVector& dx) const final override
      {
        Domain dx_( AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<AnsatzVariableSetDescription>(dx,dx_);
        Range y_( TestVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

        A_.apply( dx_ , y_ );

        auto y = range().element();
        copyFromCoefficientVector<TestVariableSetDescription>(y_,*y);

        return std::move(y);
      }

    protected:
      LinearOperator* cloneImpl() const final override
      {
        return new LinearOperator(*this);
      }

      std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const
      {
        return std::make_unique< DirectSolver<AnsatzVariableSetDescription,TestVariableSetDescription> >( A_ , spaces_, sharedRange() , sharedDomain() );
      }

      OperatorImpl A_;
      Spaces spaces_;
    };




    template <class OperatorImpl, class AnsatzVariableSetDescription, class TestVariableSetDescription>
    auto makeLinearOperator(const OperatorImpl& f,
                      std::shared_ptr<Interface::AbstractVectorSpace> domain,
                      std::shared_ptr<Interface::AbstractVectorSpace> range)
    {
      return createFromUniqueImpl< ::Algorithm::Operator , LinearOperator<OperatorImpl, AnsatzVariableSetDescription, TestVariableSetDescription> >( f, domain , range );
    }

    template <class OperatorImpl, class AnsatzVariableSetDescription, class TestVariableSetDescription>
    auto makeLinearOperator(const OperatorImpl& f,
                      const ::Algorithm::VectorSpace& domain,
                      const ::Algorithm::VectorSpace& range)
    {
      return createFromUniqueImpl< ::Algorithm::Operator , LinearOperator<OperatorImpl, AnsatzVariableSetDescription, TestVariableSetDescription> >( f, domain , range );
    }
  }
}

#endif // ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH
