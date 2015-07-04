#ifndef ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH
#define ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH

#include <memory>

#include "Interface/Functional/abstractC2Functional.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractFunctionSpaceElement;
    class AbstractHilbertSpace;
  }
  class ProductSpaceElement;
  class HilbertSpace;

  class TrackingTypeCostFunctional : public Interface::AbstractC2Functional
  {
  public:
    TrackingTypeCostFunctional(double alpha, const Interface::AbstractFunctionSpaceElement &referenceState, const HilbertSpace& domain);

    TrackingTypeCostFunctional(double alpha, const Interface::AbstractFunctionSpaceElement &referenceState, std::shared_ptr<Interface::AbstractBanachSpace> domain);

//    void setArgument(const Interface::AbstractFunctionSpaceElement &x) final override;

    double d1(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const final override;

    std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const final override;

    void setStateId(unsigned stateId);

    void setControlId(unsigned controlId);

    using Interface::AbstractC2Functional::d1;
    using Interface::AbstractC2Functional::d2;
  private:
    TrackingTypeCostFunctional* cloneImpl() const final override;

    double d0(const Interface::AbstractFunctionSpaceElement& x) const final override;

    double alpha_;
    unsigned stateId_ = 0;
    unsigned controlId_ = 1;
    std::unique_ptr<Interface::AbstractFunctionSpaceElement> referenceState_;
  };

//  TwiceDifferentiableFunctional makeTrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement& referenceState,
//                                                               const FunctionSpace& domain, const FunctionSpace& range);
}

#endif // ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH
