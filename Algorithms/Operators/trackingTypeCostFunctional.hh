#ifndef ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH
#define ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH

#include <memory>

#include "../Interface/abstractTwiceDifferentiableFunctional.hh"
//#include "../twiceDifferentiableFunctional.hh"

namespace Algorithm
{
  class AbstractFunctionSpaceElement;
  class AbstractBanachSpace;
  class ProductSpaceElement;
  class FunctionSpace;

  class TrackingTypeCostFunctional : public AbstractTwiceDifferentiableFunctional
  {
  public:
    TrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement &referenceState, const FunctionSpace& domain, const FunctionSpace& range);

    TrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement &referenceState, const AbstractBanachSpace& domain, const AbstractBanachSpace& range);

    std::unique_ptr<AbstractFunctional> clone() const final override;

    double operator()(const AbstractFunctionSpaceElement& x) const final override;

    const AbstractBanachSpace& getDomain() const ;

    const AbstractBanachSpace& getRange() const ;

    double d1(const AbstractFunctionSpaceElement& dx) const final override;

    double d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement&) const final override;

    void setStateId(unsigned stateId);

    void setControlId(unsigned controlId);

  private:
    double alpha_;
    const AbstractBanachSpace& domain_;
    const AbstractBanachSpace& range_;
    unsigned stateId_ = 0;
    unsigned controlId_ = 1;
    mutable std::unique_ptr<ProductSpaceElement> x_;
    std::unique_ptr<AbstractFunctionSpaceElement> referenceState_;
  };

//  TwiceDifferentiableFunctional makeTrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement& referenceState,
//                                                               const FunctionSpace& domain, const FunctionSpace& range);
}

#endif // ALGORITHM_OPERATORS_TRACKINGTYPECOSTFUNCTIONAL_HH
