#include "trackingTypeCostFunctional.hh"

#include "hilbertSpace.hh"

#include "Interface/abstractHilbertSpace.hh"

#include "FunctionSpaces/RealNumbers/real.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "FunctionSpaces/PrimalDualProductSpace/primalDualProductSpaceElement.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  TrackingTypeCostFunctional::TrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement &referenceState,
                                                         std::shared_ptr<AbstractBanachSpace> domain)
    : AbstractC2Functional(domain),
      alpha_(alpha),
      referenceState_( clone(referenceState) )
  {}

  TrackingTypeCostFunctional::TrackingTypeCostFunctional(double alpha, const AbstractFunctionSpaceElement &referenceState,
                                                         const HilbertSpace &domain)
    : TrackingTypeCostFunctional(alpha, referenceState, domain.sharedImpl())
  {}



//  void TrackingTypeCostFunctional::setArgument(const AbstractFunctionSpaceElement &x)
//  {
//    if( isPrimalDualProductSpaceElement(x) ) std::cout << "Primaldualelement" << std::endl;
//    if( !isProductSpaceElement(x) ) throw InvalidArgumentException("TrackingTypeCostFunctional::setArgument");
//    AbstractC2Functional::setArgument(x);
////    x_ = clone( dynamic_cast<const ProductSpaceElement&>(x) );
//  }

  double TrackingTypeCostFunctional::d0(const AbstractFunctionSpaceElement& x) const
  {
    if( !isProductSpaceElement(x) ) throw InvalidArgumentException("TrackingTypeCostFunctional::operator()");

    const auto& x_ =  dynamic_cast<const ProductSpaceElement&>(x);

    auto stateDifference = clone(*referenceState_);
    *stateDifference -= x_.variable(stateId_);

    return 0.5 * (*stateDifference * *stateDifference)
        + 0.5 * alpha_ * ( x_.variable(controlId_) * x_.variable(controlId_) );
  }

  double TrackingTypeCostFunctional::d1(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx) const
  {
    auto y = getDomain().element();

    const auto& x_ =  dynamic_cast<const ProductSpaceElement&>(x);
    auto& y_ = dynamic_cast<ProductSpaceElement&>(*y);

    x_.variable(stateId_).copyTo( y_.variable(stateId_) );
    y_.variable(stateId_) -= *referenceState_;
    y_.variable(controlId_) = x_.variable(controlId_);
    y_.variable(controlId_) *= alpha_;

    return (*y)(dx);
  }

  std::unique_ptr<AbstractFunctionSpaceElement> TrackingTypeCostFunctional::d2(const AbstractFunctionSpaceElement&, const AbstractFunctionSpaceElement& dx) const
  {
    if( !isProductSpaceElement(dx) ) throw InvalidArgumentException("TrackingTypeCostFunctional::d2");

    auto y = clone(dx);
    auto& y_ = dynamic_cast<ProductSpaceElement&>(*y);
    y_.variable(controlId_) *= alpha_;
    return y;
  }


//  double TrackingTypeCostFunctional::d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const
//  {
//    if( !isProductSpaceElement(dx) || !isProductSpaceElement(dy) ) throw InvalidArgumentException("TrackingTypeCostFunctional::d2");

//    const auto& dx_ = dynamic_cast<const ProductSpaceElement&>(dx);
//    const auto& dy_ = dynamic_cast<const ProductSpaceElement&>(dy);

//    return dx_.variable(stateId_) * dy_.variable(stateId_)
//        + alpha_ * ( dx_.variable(controlId_) * dy_.variable(controlId_) );
//  }

  void TrackingTypeCostFunctional::setStateId(unsigned stateId)
  {
    stateId_ = stateId;
  }

  void TrackingTypeCostFunctional::setControlId(unsigned controlId)
  {
    controlId_ = controlId;
  }

  TrackingTypeCostFunctional* TrackingTypeCostFunctional::cloneImpl() const
  {
    return new TrackingTypeCostFunctional(alpha_,*referenceState_,getSharedDomain());
  }

}
