//#include "lagrangeFunctional.hh"

//#include "FunctionSpaces/PrimalDualProductSpace/primalDualProductSpaceElement.hh"
//#include "Util/invalidargumentexception.hh"
//#include <iostream>
//namespace Algorithm
//{
//  LagrangeFunctional::LagrangeFunctional(const C2Functional& costFunctional, const C2Operator& constraint)
//    : f_(costFunctional), c_(constraint)
//  {}

//  double LagrangeFunctional::operator()(const FunctionSpaceElement& x)
//  {
//    if( !isPrimalDualProductSpaceElement(x.impl())) throw InvalidArgumentException("LagrangeFunctional::operator()");

//    const auto& y = dynamic_cast<const PrimalDualProductSpaceElement&>(x.impl());

//    y_ = y.primalElement();
//    f_.setArgument(y_);
//    c_.setArgument(y_);
//    if( !y.isDualEnabled() )
//    {
//      x_ = x;
//      return f_(y_);
//    }

//    x_ = x;
//    p_ = y.dualElement();

//    return f_() + p_ * c_();
//  }

//  double LagrangeFunctional::d1(const FunctionSpaceElement& dx) const
//  {
//    if( !isPrimalDualProductSpaceElement(dx.impl())) throw InvalidArgumentException("LagrangeFunctional::d1");

//    const auto& dx_ = dynamic_cast<const PrimalDualProductSpaceElement&>(dx.impl());

//    FunctionSpaceElement dxy_ = dx_.primalElement();
//    if( !dx_.isDualEnabled() )
//    {
//      dx_.reset();
//      return f_.d1(dxy_) + p_ * c_.d1(dxy_);
//    }

//    FunctionSpaceElement dxp_ = dx_.dualElement();//FunctionSpaceElement( dx_.dualElement().clone() );
//    if( !dx_.isPrimalEnabled() )
//    {
//      dx_.reset();
//      return dxp_ * c_();
//    }

//    return f_.d1(dxy_) + p_ * c_.d1(dxy_) + dxp_ * c_();
//  }

//  double LagrangeFunctional::d2(const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const
//  {
//    if( !isPrimalDualProductSpaceElement(dx.impl()) || !isPrimalDualProductSpaceElement(dy.impl())) throw InvalidArgumentException("LagrangeFunctional::d2");

//    const auto& dx_ = dynamic_cast<const PrimalDualProductSpaceElement&>(dx.impl());
//    const auto& dy_ = dynamic_cast<const PrimalDualProductSpaceElement&>(dy.impl());

//    auto reset = [](const auto& x, const auto& y) { x.reset(); y.reset(); };

//    if( !dx_.isPrimalEnabled() && !dy_.isPrimalEnabled() )
//    {
//      reset(dx_,dy_);
//      return 0;
//    }

//    FunctionSpaceElement dxy_ = dx_.primalElement();//FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dx.impl()).primalElement().clone() );
//    FunctionSpaceElement dyy_ = dy_.primalElement();//FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dy.impl()).primalElement().clone() );

//    FunctionSpaceElement dxp_ = dx_.dualElement();// FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dx.impl()).dualElement().clone() );
//    FunctionSpaceElement dyp_ = dy_.dualElement();//FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dy.impl()).dualElement().clone() );

//    if( !dx_.isDualEnabled() )
//    {
//      if( !dy_.isDualEnabled() )
//      {
//        reset(dx_,dy_);
//        return f_.d2(dxy_,dyy_) + p_ * c_.d2(dxy_,dyy_);
//      }
//      if( !dy_.isPrimalEnabled() )
//      {
//        reset(dx_,dy_);
//        return dyp_ * c_.d1(dxy_);
//      }
//      reset(dx_,dy_);
//      return f_.d2(dxy_,dyy_) + p_ * c_.d2(dxy_,dyy_) + dyp_ * c_.d1(dxy_);
//    }

//    if( !dy_.isDualEnabled() )
//    {
//      if( !dx_.isPrimalEnabled() )
//      {
//        reset(dx_,dy_);
//        return dxp_ * c_.d1(dyy_);
//      }
//      reset(dx_,dy_);
//      return f_.d2(dxy_,dyy_) + dxp_ * c_.d1(dyy_);
//    }

//    if( !dx_.isPrimalEnabled() )
//    {
//      reset(dx_,dy_);
//      return dxp_ * c_.d1(dyy_);
//    }

//    if( !dy_.isPrimalEnabled() )
//    {
//      reset(dx_,dy_);
//      return dyp_ * c_.d1(dxy_);
//    }

//    reset(dx_,dy_);
//    return f_.d2(dxy_,dyy_) + p_ * c_.d2(dxy_,dyy_) + dyp_ * c_.d1(dxy_) + dxp_ * c_.d1(dyy_);
//  }

//  const C2Functional &LagrangeFunctional::getCostFunctional() const
//  {
//    return f_;
//  }

//  const C2Operator& LagrangeFunctional::getConstraint() const
//  {
//    return c_;
//  }

//  const FunctionSpaceElement& LagrangeFunctional::getStateVariable() const
//  {
//    return y_;
//  }

//  const FunctionSpaceElement& LagrangeFunctional::getLagrangeMultiplier() const
//  {
//    return p_;
//  }
//}
