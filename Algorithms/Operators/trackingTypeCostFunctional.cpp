#include "trackingTypeCostFunctional.hh"

#include "functionSpace.hh"

#include "Interface/abstractBanachSpace.hh"

#include "FunctionSpaces/RealNumbers/real.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "FunctionSpaces/PrimalDualProductSpace/primalDualProductSpaceElement.hh"
#include "Util/invalidargumentexception.hh"

namespace Algorithm
{
  TrackingTypeCostFunctional::TrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement &referenceState,
                                                         const FunctionSpace &domain)
    : AbstractC2Functional(domain.impl()),
      alpha_(alpha),
      referenceState_(referenceState.clone())
  {}

  TrackingTypeCostFunctional::TrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement &referenceState,
                                                         const AbstractBanachSpace &domain)
    : AbstractC2Functional(domain),
      alpha_(alpha),
      referenceState_(referenceState.clone())
  {}

  std::unique_ptr<AbstractC0Functional> TrackingTypeCostFunctional::clone() const
  {
    return std::make_unique<TrackingTypeCostFunctional>(alpha_,*referenceState_,getDomain());
  }

  void TrackingTypeCostFunctional::setArgument(const AbstractFunctionSpaceElement &x)
  {
    if( isPrimalDualProductSpaceElement(x) ) std::cout << "Primaldualelement" << std::endl;
    if( !isProductSpaceElement(x) ) throw InvalidArgumentException("TrackingTypeCostFunctional::operator()");
    x_.reset( dynamic_cast<ProductSpaceElement*>(x.clone().release()) );
  }

//  double TrackingTypeCostFunctional::operator()(const AbstractFunctionSpaceElement& x) const
//  {
//    if( isPrimalDualProductSpaceElement(x) ) std::cout << "Primaldualelement" << std::endl;
//    if( !isProductSpaceElement(x) ) throw InvalidArgumentException("TrackingTypeCostFunctional::operator()");

//    x_.reset( dynamic_cast<ProductSpaceElement*>(x.clone().release()) );

//    auto stateDifference = referenceState_->clone();
//    *stateDifference -= x_->variable(stateId_);

//    return 0.5 * (*stateDifference * *stateDifference)
//        + 0.5 * alpha_ * ( x_->variable(controlId_) * x_->variable(controlId_) );
//  }

  double TrackingTypeCostFunctional::d0() const
  {
    auto stateDifference = referenceState_->clone();
    *stateDifference -= x_->variable(stateId_);

    return 0.5 * (*stateDifference * *stateDifference)
        + 0.5 * alpha_ * ( x_->variable(controlId_) * x_->variable(controlId_) );
  }

  double TrackingTypeCostFunctional::d1(const AbstractFunctionSpaceElement& dx) const
  {
    if( !isProductSpaceElement(dx) ) throw InvalidArgumentException("TrackingTypeCostFunctional::operator()");

    const auto& dx_ = dynamic_cast<const ProductSpaceElement&>(dx);

    auto stateDifference = referenceState_->clone();
    *stateDifference *= -1;
    *stateDifference += x_->variable(stateId_);

    return (*stateDifference * dx_.variable(stateId_))
        + alpha_ * ( x_->variable(controlId_) * dx_.variable(controlId_) );
  }

  double TrackingTypeCostFunctional::d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const
  {
    if( !isProductSpaceElement(dx) || !isProductSpaceElement(dy) ) throw InvalidArgumentException("TrackingTypeCostFunctional::operator()");

    const auto& dx_ = dynamic_cast<const ProductSpaceElement&>(dx);
    const auto& dy_ = dynamic_cast<const ProductSpaceElement&>(dy);

    return dx_.variable(stateId_) * dy_.variable(stateId_)
        + alpha_ * ( dx_.variable(controlId_) * dy_.variable(controlId_) );
  }

  void TrackingTypeCostFunctional::setStateId(unsigned stateId)
  {
    stateId_ = stateId;
  }

  void TrackingTypeCostFunctional::setControlId(unsigned controlId)
  {
    controlId_ = controlId;
  }

//  TwiceDifferentiableFunctional makeTrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement& referenceState,
//                                                               const FunctionSpace& domain, const FunctionSpace& range)
//  {
//    return TwiceDifferentiableFunctional( std::make_shared<TrackingTypeCostFunctional>(alpha,referenceState,domain.impl(),range.impl()) );
//  }

}
