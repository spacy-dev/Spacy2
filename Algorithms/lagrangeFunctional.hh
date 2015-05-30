//#ifndef ALGORITHM_LAGRANGEFUNCTIONAL_HH
//#define ALGORITHM_LAGRANGEFUNCTIONAL_HH

//#include <memory>

//#include "twiceDifferentiableFunctional.hh"
//#include "twiceDifferentiableOperator.hh"
//#include "functionSpaceElement.hh"

//namespace Algorithm
//{
//  class AbstractFunctionSpaceElement;
//  class FunctionSpace;
//  class ProductSpaceElement;

//  class LagrangeFunctional
//  {
//  public:
//    LagrangeFunctional(const TwiceDifferentiableFunctional& costFunctional, const TwiceDifferentiableOperator& contraint);

//    double operator()(const FunctionSpaceElement& x) const;

//    double d1(const FunctionSpaceElement& dx) const;

//    double d2(const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;

//    const TwiceDifferentiableFunctional& getCostFunctional() const;

//    const TwiceDifferentiableOperator& getConstraint() const;

//  private:
//    TwiceDifferentiableFunctional costFunctional_;
//    TwiceDifferentiableOperator constraint_;
//    mutable FunctionSpaceElement x_, y_, p_;
//  };
//}

//#endif // ALGORITHM_LAGRANGEFUNCTIONAL_HH
