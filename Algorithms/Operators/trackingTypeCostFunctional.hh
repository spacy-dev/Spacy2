#ifndef ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH
#define ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH

#include <memory>

#include "Interface/Operator/abstractOperator.hh"
#include "Interface/abstractFunctional.hh"
#include "Interface/abstractScalarProduct.hh"
#include "Util/Mixins/controlIndex.hh"
#include "Util/Mixins/stateIndex.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractVector;
    class AbstractVectorSpace;
  }

  class ProductSpaceElement;
  class VectorSpace;

  class TrackingTypeCostFunctional :
      public Interface::AbstractFunctional , public Mixin::ControlIndex , public Mixin::StateIndex
  {
  public:
    TrackingTypeCostFunctional(double alpha, const Interface::AbstractVector &referenceState, const VectorSpace& domain,
                               std::unique_ptr<Interface::AbstractOperator>&& My,
                               std::unique_ptr<Interface::AbstractOperator>&& Mu);

    TrackingTypeCostFunctional(double alpha, const Interface::AbstractVector &referenceState,
                               std::shared_ptr<Interface::AbstractVectorSpace> domain,
                               std::unique_ptr<Interface::AbstractOperator>&& My,
                               std::unique_ptr<Interface::AbstractOperator>&& Mu);

//    void setArgument(const Interface::AbstractVector &x) final override;

    std::unique_ptr<Interface::AbstractVector> d1(const Interface::AbstractVector& x) const final override;

    std::unique_ptr<Interface::AbstractVector> d2(const Interface::AbstractVector& x, const Interface::AbstractVector& dx) const final override;

  private:
    TrackingTypeCostFunctional* cloneImpl() const final override;

    double d0(const Interface::AbstractVector& x) const final override;

    double alpha_;
    std::unique_ptr<Interface::AbstractVector> referenceState_ = nullptr;
    std::unique_ptr<Interface::AbstractOperator> My_, Mu_ = nullptr;
  };

//  TwiceDifferentiableFunctional makeTrackingTypeCostFunctional(double alpha, const AbstractVector& referenceState,
//                                                               const FunctionSpace& domain, const FunctionSpace& range);
}

#endif // ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH
