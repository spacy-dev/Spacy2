#ifndef ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH
#define ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH

#include <memory>

#include "Interface/abstractC2Functional.hh"
//#include "../twiceDifferentiableFunctional.hh"

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

    std::unique_ptr<AbstractC0Functional> clone() const final override;

    void setArgument(const AbstractFunctionSpaceElement &x) final override;

//    double operator()(const AbstractFunctionSpaceElement& x) const final override;

    double d0() const final override;

    double d1(const AbstractFunctionSpaceElement& dx) const final override;

    double d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement&) const final override;

    void setStateId(unsigned stateId);

    void setControlId(unsigned controlId);

  private:
    double alpha_;
    unsigned stateId_ = 0;
    unsigned controlId_ = 1;
    mutable std::unique_ptr<ProductSpaceElement> x_;
    std::unique_ptr<AbstractFunctionSpaceElement> referenceState_;
  };

//  TwiceDifferentiableFunctional makeTrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement& referenceState,
//                                                               const FunctionSpace& domain, const FunctionSpace& range);
}

#endif // ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH
