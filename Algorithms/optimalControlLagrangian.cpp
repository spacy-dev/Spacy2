#include "optimalControlLagrangian.hh"

#include "FunctionSpaces/PrimalDualProductSpace/primalDualProductSpaceElement.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  OptimalControlLagrangian::OptimalControlLagrangian(const C2Functional& costFunctional, const C2Operator& state, const C2Operator& control)
    : f_(costFunctional), state_(state), control_(control)
  {}

  double OptimalControlLagrangian::operator()(const FunctionSpaceElement& x)
  {
    if( !isPrimalDualProductSpaceElement(x.impl())) throw InvalidArgumentException("LagrangeFunctional::operator()");

    const auto& y = dynamic_cast<const PrimalDualProductSpaceElement&>(x.impl());

    FunctionSpaceElement y_ = y.primalElement();
    if( !y.isDualEnabled() )
    {
      return f_(y_);
    }

    FunctionSpaceElement p_ = y.dualElement();

    return f_(y_) + p_ * ( state_(y_) + control_(y_) );
  }

  double OptimalControlLagrangian::d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const
  {
    if( !isPrimalDualProductSpaceElement(dx.impl())) throw InvalidArgumentException("LagrangeFunctional::d1");

    const auto& dx_ = dynamic_cast<const PrimalDualProductSpaceElement&>(dx.impl());

    FunctionSpaceElement y_ = dynamic_cast<const PrimalDualProductSpaceElement&>(x.impl()).primalElement();
    FunctionSpaceElement p_ = dynamic_cast<const PrimalDualProductSpaceElement&>(x.impl()).dualElement();

    FunctionSpaceElement dxy_ = dx_.primalElement();
    if( !dx_.isDualEnabled() )
    {
      dx_.reset();
      return f_.d1(y_,dxy_) + p_ * ( state_.d1(y_,dxy_) + control_.d1(y_,dxy_) );
    }

    FunctionSpaceElement dxp_ = dx_.dualElement();
    if( !dx_.isPrimalEnabled() )
    {
      dx_.reset();
      return dxp_ * ( state_(y_) + control_(y_) );
    }

    return f_.d1(y_,dxy_) + p_ * ( state_.d1(y_,dxy_) + control_.d1(y_,dxy_) ) + dxp_ * ( state_(y_) + control_(y_) );
  }

  double OptimalControlLagrangian::d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const
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

    FunctionSpaceElement dxy_ = dx_.primalElement();
    FunctionSpaceElement dyy_ = dy_.primalElement();

    FunctionSpaceElement dxp_ = dx_.dualElement();
    FunctionSpaceElement dyp_ = dy_.dualElement();

    if( !dx_.isDualEnabled() )
    {
      if( !dy_.isDualEnabled() )
      {
        reset(dx_,dy_);
        return f_.d2(y_,dxy_,dyy_) + p_ * ( state_.d2(y_,dxy_,dyy_) + control_.d2(y_,dxy_,dyy_) );
      }
      if( !dy_.isPrimalEnabled() )
      {
        reset(dx_,dy_);
        return dyp_ * ( state_.d1(y_,dxy_) + control_.d1(y_,dxy_) );
      }
      reset(dx_,dy_);
      return f_.d2(y_,dxy_,dyy_) + p_ * ( state_.d2(y_,dxy_,dyy_) + control_.d2(y_,dxy_,dyy_) ) + dyp_ * ( state_.d1(y_,dxy_) + control_.d1(y_,dxy_) );
    }

    if( !dy_.isDualEnabled() )
    {
      if( !dx_.isPrimalEnabled() )
      {
        reset(dx_,dy_);
        return dxp_ * ( state_.d1(y_,dyy_) + control_.d1(y_,dyy_) );
      }
      reset(dx_,dy_);
      return f_.d2(y_,dxy_,dyy_) + dxp_ * ( state_.d1(y_,dyy_) + control_.d1(y_,dyy_) );
    }

    if( !dx_.isPrimalEnabled() )
    {
      reset(dx_,dy_);
      return dxp_ * ( state_.d1(y_,dyy_) + control_.d1(y_,dyy_) );
    }

    if( !dy_.isPrimalEnabled() )
    {
      reset(dx_,dy_);
      return dyp_ * ( state_.d1(y_,dxy_) + control_.d1(y_,dxy_) );
    }

    reset(dx_,dy_);
    return f_.d2(y_,dxy_,dyy_) +
        p_ * ( state_.d2(y_,dxy_,dyy_) + control_.d2(y_,dxy_,dyy_) ) +
        dyp_ * ( state_.d1(y_,dxy_) + control_.d1(y_,dxy_) ) +
        dxp_ * ( state_.d1(y_,dyy_) + control_.d1(y_,dyy_) );
  }

  const C2Functional &OptimalControlLagrangian::getCostFunctional() const
  {
    return f_;
  }

  const C2Operator& OptimalControlLagrangian::getStateOperator() const
  {
    return state_;
  }

  const C2Operator& OptimalControlLagrangian::getControlOperator() const
  {
    return control_;
  }
}
