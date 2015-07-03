#include "lagrangeFunctional.hh"

#include "FunctionSpaces/PrimalDualProductSpace/primalDualProductSpaceElement.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include <iostream>
namespace Algorithm
{
  LagrangeFunctional::LagrangeFunctional(const C2Functional& costFunctional, const C2Operator& constraint)
    : f_(costFunctional), c_(constraint)
  {}

  double LagrangeFunctional::operator()(const FunctionSpaceElement& x)
  {
    if( !isPrimalDualProductSpaceElement(x.impl())) throw InvalidArgumentException("LagrangeFunctional::operator()");

    const auto& y = dynamic_cast<const PrimalDualProductSpaceElement&>(x.impl());

    FunctionSpaceElement y_ = y.primalElement();
    if( !y.isDualEnabled() )
    {
      return f_(y_);
    }

    FunctionSpaceElement p_ = y.dualElement();

    return f_(y_) + p_ * c_(y_);
  }

  double LagrangeFunctional::d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const
  {
    if( !isPrimalDualProductSpaceElement(dx.impl())) throw InvalidArgumentException("LagrangeFunctional::d1");

    const auto& dx_ = dynamic_cast<const PrimalDualProductSpaceElement&>(dx.impl());

    FunctionSpaceElement y_ = dynamic_cast<const PrimalDualProductSpaceElement&>(x.impl()).primalElement();
    FunctionSpaceElement p_ = dynamic_cast<const PrimalDualProductSpaceElement&>(x.impl()).dualElement();

    FunctionSpaceElement dxy_ = dx_.primalElement();
    if( !dx_.isDualEnabled() )
    {
      dx_.reset();
      return f_.d1(y_,dxy_) + p_ * c_.d1(y_,dxy_);
    }

    FunctionSpaceElement dxp_ = dx_.dualElement();//FunctionSpaceElement( dx_.dualElement().clone() );
    if( !dx_.isPrimalEnabled() )
    {
      dx_.reset();
      return dxp_ * c_(y_);
    }

    return f_.d1(y_,dxy_) + p_ * c_.d1(y_,dxy_) + dxp_ * c_(y_);
  }

  double LagrangeFunctional::d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const
  {
    if( !isPrimalDualProductSpaceElement(dx.impl()) || !isPrimalDualProductSpaceElement(dy.impl())) throw InvalidArgumentException("LagrangeFunctional::d2");

    const auto& dx_ = dynamic_cast<const PrimalDualProductSpaceElement&>(dx.impl());
    const auto& dy_ = dynamic_cast<const PrimalDualProductSpaceElement&>(dy.impl());

    auto reset = [](const auto& x, const auto& y) { x.reset(); y.reset(); };

    if( !dx_.isPrimalEnabled() && !dy_.isPrimalEnabled() )
    {
      reset(dx_,dy_);
      return 0;
    }

    FunctionSpaceElement y_ = dynamic_cast<const PrimalDualProductSpaceElement&>(x.impl()).primalElement();
    FunctionSpaceElement p_ = dynamic_cast<const PrimalDualProductSpaceElement&>(x.impl()).dualElement();

    FunctionSpaceElement dxy_ = dx_.primalElement();//FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dx.impl()).primalElement().clone() );
    FunctionSpaceElement dyy_ = dy_.primalElement();//FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dy.impl()).primalElement().clone() );

    FunctionSpaceElement dxp_ = dx_.dualElement();// FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dx.impl()).dualElement().clone() );
    FunctionSpaceElement dyp_ = dy_.dualElement();//FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dy.impl()).dualElement().clone() );

    if( !dx_.isDualEnabled() )
    {
      if( !dy_.isDualEnabled() )
      {
        reset(dx_,dy_);
        return f_.d2(y_,dxy_,dyy_) + p_ * c_.d2(y_,dxy_,dyy_);
      }
      if( !dy_.isPrimalEnabled() )
      {
        reset(dx_,dy_);
        return dyp_ * c_.d1(y_,dxy_);
      }
      reset(dx_,dy_);
      return f_.d2(y_,dxy_,dyy_) + p_ * c_.d2(y_,dxy_,dyy_) + dyp_ * c_.d1(y_,dxy_);
    }

    if( !dy_.isDualEnabled() )
    {
      if( !dx_.isPrimalEnabled() )
      {
        reset(dx_,dy_);
        return dxp_ * c_.d1(y_,dyy_);
      }
      reset(dx_,dy_);
      return f_.d2(y_,dxy_,dyy_) + dxp_ * c_.d1(y_,dyy_);
    }

    if( !dx_.isPrimalEnabled() )
    {
      reset(dx_,dy_);
      return dxp_ * c_.d1(y_,dyy_);
    }

    if( !dy_.isPrimalEnabled() )
    {
      reset(dx_,dy_);
      return dyp_ * c_.d1(y_,dxy_);
    }

    reset(dx_,dy_);
    return f_.d2(y_,dxy_,dyy_) + p_ * c_.d2(y_,dxy_,dyy_) + dyp_ * c_.d1(y_,dxy_) + dxp_ * c_.d1(y_,dyy_);
  }

  const C2Functional &LagrangeFunctional::getCostFunctional() const
  {
    return f_;
  }

  const C2Operator& LagrangeFunctional::getConstraint() const
  {
    return c_;
  }
}
