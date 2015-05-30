//#include "lagrangeFunctional.hh"

//#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
//#include "Util/invalidargumentexception.hh"

//namespace Algorithm
//{
//  LagrangeFunctional::LagrangeFunctional(const TwiceDifferentiableFunctional& costFunctional, const TwiceDifferentiableOperator& constraint)
//    : costFunctional_(costFunctional), constraint_(constraint)
//  {}

//  double LagrangeFunctional::operator()(const FunctionSpaceElement& x) const
//  {
//    if( !isProductSpaceElement(x.impl())) throw InvalidArgumentException("LagrangeFunctional::operator()");

//    x_ = x;
//    y_ = dynamic_cast<const ProductSpaceElement&>(x.impl()).primalElement();
//    p_ = dynamic_cast<const ProductSpaceElement&>(x.impl()).dualElement();

//    return costFunctional_(y_) + p_ * constraint_(y_);
//  }

//  double LagrangeFunctional::d1(const FunctionSpaceElement& dx) const
//  {
//    if( !isProductSpaceElement(dx.impl())) throw InvalidArgumentException("LagrangeFunctional::d1");

//    const auto& dx_ = dynamic_cast<const ProductSpaceElement&>(dx.impl());

//    auto dxy_ = dx_.primalElement();
//    auto dxp_ = dx_.dualElement();//FunctionSpaceElement( dx_.dualElement().clone() );

//    return costFunctional_.d1(dxy_) + p_ * constraint_.d1(dxy_) + dxp_ * constraint_(y_);
//  }

//  double LagrangeFunctional::d2(const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const
//  {
//    if( !isProductSpaceElement(dx.impl()) || !isProductSpaceElement(dy.impl())) throw InvalidArgumentException("LagrangeFunctional::d2");

//    auto dxy_ = dynamic_cast<const ProductSpaceElement&>(dx.impl()).primalElement();//FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dx.impl()).primalElement().clone() );
//    auto dxp_ = dynamic_cast<const ProductSpaceElement&>(dx.impl()).dualElement();// FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dx.impl()).dualElement().clone() );

//    auto dyy_ = dynamic_cast<const ProductSpaceElement&>(dy.impl()).primalElement();//FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dy.impl()).primalElement().clone() );
//    auto dyp_ = dynamic_cast<const ProductSpaceElement&>(dy.impl()).dualElement();//FunctionSpaceElement( dynamic_cast<const ProductSpaceElement&>(dy.impl()).dualElement().clone() );

//    return costFunctional_.d2(dxy_,dyy_) + p_ * constraint_.d2(dxy_,dyy_) + dyp_ * constraint_.d1(dxy_) + dxp_ * constraint_.d1(dyy_);
//  }

//  const TwiceDifferentiableFunctional& LagrangeFunctional::getCostFunctional() const
//  {
//    return costFunctional_;
//  }

//  const TwiceDifferentiableOperator& LagrangeFunctional::getConstraint() const
//  {
//    return constraint_;
//  }

//}
