//#include <iostream>

//#include <armadillo>

//#include "../../RFFGen/RFFGen/RFFGen.hh"
////#include "../../RFFGen/src/RFFGen.hh"
//#include "Interface/abstractBanachSpace.hh"
//#include "Interface/Operator/abstractOperator.hh"
//#include "Algorithm/ConjugateGradients/cg.hh"
//#include "FunctionSpaces/VectorSpace/vectorSpace.hh"
//#include "FunctionSpaces/VectorSpace/vectorSpaceElement.hh"
//#include "FunctionSpaces/VectorSpace/cgSolver.hh"
//#include "Algorithm/ConjugateGradients/jacobipreconditioner.hh"

//#include "FunctionSpaces/PrimalDualProductSpace/primalDualProductSpace.hh"
//#include "FunctionSpaces/PrimalDualProductSpace/primalDualProductSpaceElement.hh"
//#include "Test/scalarRFFGenOperator.hh"
//#include "Algorithm/newton.hh"
//#include "spaces.hh"
//#include "lagrangeFunctional.hh"
//#include "Operators/trackingTypeCostFunctional.hh"

//namespace Algorithm
//{
//  template <class VectorSpaceElement>
//  class Test2Operator : public AbstractOperator
//  {
//  public:
//    Test2Operator(const AbstractBanachSpace& space)
//      : AbstractOperator(space,space)
//    {}

//    Test2Operator(const FunctionSpace& space)
//      : Test2Operator(space.impl())
//    {}

//    ~Test2Operator(){}

//    std::unique_ptr<AbstractOperator> clone() const override
//    {
//      return std::make_unique<Test2Operator>(getDomain());
//    }

//    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement &x) const override
//    {
//      return computeResult(x);
//    }

//  private:
//    std::unique_ptr<AbstractFunctionSpaceElement> computeResult(const AbstractFunctionSpaceElement& x) const
//    {
//      auto y = x.clone();
//      for(unsigned i = 0; i < x.size(); ++i)
//      {
//        if(i==0) y->coefficient(0) = 2*x.coefficient(0) + x.coefficient(1);
//        else if(i+1==x.size()) y->coefficient(i) = 2*x.coefficient(i) + x.coefficient(i-1);
//        else y->coefficient(i) = 2*x.coefficient(i) + x.coefficient(i+1) + x.coefficient(i-1);
//      }
//      return y;
//    }
//  };
//}


//using namespace std;

//int main()
//{
//  cout << "start" << endl;
//  using namespace Algorithm;
//  using Spaces::R; // Real numbers

//  cout << "bla" << endl;
//  auto x = R.element();
////  auto y = R.element();

////  // set function value
//  x.coefficient(0) = -2;
////  y.coefficient(0) = 1;
////  auto norm = R.getNorm();
////  auto sp = R.getDualPairing();
////  cout << "|x| = " << norm(x) << endl;
////  cout << "|y| = " << norm(y) << endl;
////  cout << "(x,y) = " << sp(x,y) << endl;
////  cout << "or equivalently: x*y = " << x*y << endl;


////  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace> ,
////                                             PackSpaces<RealSpace> >() );
////  cout << "get x2" << endl;
////  auto x2 = R2.element();
////  auto y2 = R2.element();
////  y2.coefficient(0) = x2.coefficient(0) = 1;
////  y2.coefficient(1) = 2;
////  auto norm2 = R2.getNorm();
////  auto sp2 = R2.getDualPairing();

////  cout << "x2: " << x2 << endl;
////  cout << "y2: " << y2 << endl;
////  cout << "|x2| = " << norm2(x2) << endl;
////  cout << "x2*y2 = " << x2*y2 << endl;
////  cout << (x2 += primal(y2)) << endl;
////  primal(x2) += y2;
////  cout << x2 << endl;
////  primal(x2) += dual(y2);
////  cout << x2 << endl;
////  cout << "z: " << endl;
////  auto z = (dual(x2) += y2);
////  cout << z << endl;
////  z = dual(x2) + y2;
////  cout << "z2: " << z << endl;
////  cout << (x2 += dual(y2)) << endl;
////  cout << ( dual(x2) + y2  ) << endl;


//std::cout << "make operator" << std::endl;
//  auto A = makeC1Operator<TestOperator>(R) ;//C1Operator( std::make_shared<TestOperator>(R) ); // operator
//  std::cout << "make newton" << std::endl;
//  auto newton = Newton(A,true);
//  std::cout << "solve newton " << endl;
//  cout << "sol: " << newton.solve(x) << endl;

//    using namespace RFFGen::CMath;
//  using Vector = arma::vec::fixed<10>;
//  FunctionSpace vectorSpace(std::make_unique<VectorSpace<Vector> >());
//  auto v0 = vectorSpace.element();
//  auto rhs = vectorSpace.element();
//  for(unsigned i=0; i<rhs.size(); ++i) rhs.coefficient(i) = (i==0 || i+1==rhs.size()) ? 3 : 4;
//  auto B = makeOperator<Algorithm::Test2Operator<decltype(v0)> >(vectorSpace);
////  auto jacobi = jacobiPreconditioner(B);
//  //CGSolver cg(B);//,jacobi);
//  LinearSolver cg( std::make_unique<CGSolver>(B) );
//  auto vsol = cg(rhs);
//  cout << "cg sol: " << vsol << endl;
////    FunctionSpace R(std::make_unique<RealNumbers>());
////    ExampleOperator_1 A(R);
////  auto f = Exp() - 2;
////  auto A = Algorithm::makeScalarRFFGenOperator( f , R );
////    CG<double> cg(A);

////  cout << "test lagrange functional" << endl;
////  auto X = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace,RealSpace> , PackSpaces<RealSpace> >() );
////  const auto& Y = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getPrimalProductSpace();
////  const auto& P = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getDualProductSpace();
////  FunctionSpaceElement reference = Y.element();
////  reference.coefficient(0) = 1;
////  cout << "ref: " << isProductSpaceElement(reference.impl()) << endl;

////  double alpha = 3;
////  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
////  C2Functional J(std::make_shared<TrackingTypeCostFunctional>(alpha,referenceState,Y,P));
////  C2Operator c(std::make_shared<TestOperator2>(Y,P));
////  LagrangeFunctional L(J,c);

////  auto x = X.element();
////  cout << "x: " << isProductSpaceElement(x.impl()) << endl;
////  auto y = X.element();
////  y.coefficient(0) = 1;
////  y.coefficient(1) = 1;
////  y.coefficient(2) = 1;
////  auto z = y;
////  z.coefficient(2) = 3;
////  z.coefficient(0) = 2;
////  cout << L(x) << endl;

//  return 0;
//}

