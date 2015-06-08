//#ifndef ALGORITHM_LAGRANGEFUNCTIONAL_HH
//#define ALGORITHM_LAGRANGEFUNCTIONAL_HH

//#include <memory>

//#include "c2Functional.hh"
//#include "c2Operator.hh"
//#include "functionSpaceElement.hh"

//namespace Algorithm
//{
//  class AbstractFunctionSpaceElement;
//  class FunctionSpace;
//  class ProductSpaceElement;

//  class LagrangeFunctional
//  {
//  public:
//    LagrangeFunctional(const C2Functional& costFunctional, const C2Operator& contraint);

//    double operator()(const FunctionSpaceElement& x);

//    double d1(const FunctionSpaceElement& dx) const;

//    double d2(const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;

//    const C2Functional& getCostFunctional() const;

//    const C2Operator& getConstraint() const;

//    const FunctionSpaceElement& getStateVariable() const;

//    const FunctionSpaceElement& getLagrangeMultiplier() const;

//  private:
//    C2Functional f_;
//    C2Operator c_;
//    mutable FunctionSpaceElement x_, y_, p_;
//  };
//}

//#endif // ALGORITHM_LAGRANGEFUNCTIONAL_HH
