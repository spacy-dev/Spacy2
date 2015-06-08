#ifndef ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH
#define ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH

#include <memory>

#include "Interface/Functional/abstractC2Functional.hh"

namespace Algorithm
{
  class AbstractFunctionSpaceElement;
  class AbstractBanachSpace;
  class ProductSpaceElement;
  class FunctionSpace;

  class TrackingTypeCostFunctional : public AbstractC2Functional
  {
  public:
    TrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement &referenceState, const FunctionSpace& domain);

    TrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement &referenceState, const AbstractBanachSpace& domain);

    void setArgument(const AbstractFunctionSpaceElement &x) final override;

    double operator()(const AbstractFunctionSpaceElement& x) const final override;

    std::unique_ptr<AbstractFunctionSpaceElement> d1() const final override;

    std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& dx) const final override;

    double d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement&) const final override;

    void setStateId(unsigned stateId);

    void setControlId(unsigned controlId);

    using AbstractC2Functional::d1;
    using AbstractC2Functional::d2;
  private:
    TrackingTypeCostFunctional* cloneImpl() const final override;

    double alpha_;
    unsigned stateId_ = 0;
    unsigned controlId_ = 1;
    std::unique_ptr<AbstractFunctionSpaceElement> referenceState_;
  };

//  TwiceDifferentiableFunctional makeTrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement& referenceState,
//                                                               const FunctionSpace& domain, const FunctionSpace& range);
}

#endif // ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH
