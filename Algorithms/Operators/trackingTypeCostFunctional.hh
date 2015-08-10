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
    class AbstractFunctionSpaceElement;
    class AbstractFunctionSpace;
  }

  class ProductSpaceElement;
  class FunctionSpace;

  class TrackingTypeCostFunctional :
      public Interface::AbstractFunctional , public Mixin::ControlIndex , public Mixin::StateIndex
  {
  public:
    TrackingTypeCostFunctional(double alpha, const Interface::AbstractFunctionSpaceElement &referenceState, const FunctionSpace& domain,
                               std::unique_ptr<Interface::AbstractOperator>&& My,
                               std::unique_ptr<Interface::AbstractOperator>&& Mu);

    TrackingTypeCostFunctional(double alpha, const Interface::AbstractFunctionSpaceElement &referenceState,
                               std::shared_ptr<Interface::AbstractFunctionSpace> domain,
                               std::unique_ptr<Interface::AbstractOperator>&& My,
                               std::unique_ptr<Interface::AbstractOperator>&& Mu);

//    void setArgument(const Interface::AbstractFunctionSpaceElement &x) final override;

    std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement& x) const final override;

    std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const final override;

  private:
    TrackingTypeCostFunctional* cloneImpl() const final override;

    double d0(const Interface::AbstractFunctionSpaceElement& x) const final override;

    double alpha_;
    std::unique_ptr<Interface::AbstractFunctionSpaceElement> referenceState_ = nullptr;
    std::unique_ptr<Interface::AbstractOperator> My_, Mu_ = nullptr;
  };

//  TwiceDifferentiableFunctional makeTrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement& referenceState,
//                                                               const FunctionSpace& domain, const FunctionSpace& range);
}

#endif // ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH
