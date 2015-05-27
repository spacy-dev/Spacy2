#include <iostream>

//#include <armadillo>

//#include "../../RFFGen/src/RFFGen.hh"
//#include "Algorithm/ConjugateGradients/cg.hh"
//#include "FunctionSpaces/VectorSpace/vectorSpace.hh"
//#include "FunctionSpaces/VectorSpace/cgSolver.hh"
//#include "Algorithm/ConjugateGradients/jacobipreconditioner.hh"

#include "Test/scalarRFFGenOperator.hh"
#include "Algorithm/newton.hh"
#include "spaces.hh"

//namespace Algorithm
//{
//  template <class VectorSpaceElement>
//  class Test2Operator : public Operator
//  {
//  public:
//    Test2Operator(const FunctionSpace& space)
//      : Operator(space,space)
//    {}

//    ~Test2Operator(){}

//    void update(const FunctionSpaceElement &x) override
//    {
//      std::cout << "AAAAAAAAAAAAAAA" << std::endl;
//      assert(false);
//    }

//    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const override
//    {
//      return computeResult(x);
//    }

//    FunctionSpaceElement d1(const FunctionSpaceElement &dx) const override
//    {
//      return computeResult(dx);
//    }

//  private:
//    auto computeResult(const FunctionSpaceElement& x) const
//    {
//      auto y = x;
//      for(unsigned i = 0; i < x.size(); ++i)
//      {
//        if(i==0) y.coefficient(0) = 2*x.coefficient(0) + x.coefficient(1);
//        else if(i+1==x.size()) y.coefficient(i) = 2*x.coefficient(i) + x.coefficient(i-1);
//        else y.coefficient(i) = 2*x.coefficient(i) + x.coefficient(i+1) + x.coefficient(i-1);
//      }
//      return y;
//    }
//  };
//}

using namespace std;

int main()
{
  using namespace Algorithm;

  using Spaces::R; // Real numbers

  auto x = R.element();
  auto y = R.element();

  // set function value
  x.coefficient(0) = -2;
  y.coefficient(0) = 1;
  auto norm = R.getNorm();
  auto sp = R.getScalarProduct();
  cout << "|x| = " << norm(x) << endl;
  cout << "|y| = " << norm(y) << endl;
  cout << "(x,y) = " << sp(x,y) << endl;
  cout << "or equivalently: x*y = " << x*y << endl;

  auto A = DifferentiableOperator( std::make_shared<TestOperator>(R) ); // operator
  auto newton = Newton(A,true);
  cout << "sol: " << newton.solve() << endl;

  //  using namespace RFFGen::CMath;
//  using Vector = arma::vec::fixed<10>;
//  FunctionSpace vectorSpace(std::make_unique<VectorSpace<Vector> >());
//  auto v0 = vectorSpace.element();
//  auto rhs = vectorSpace.element();
//  for(unsigned i=0; i<rhs.size(); ++i) rhs.coefficient(i) = (i==0 || i+1==rhs.size()) ? 3 : 4;
//  Test2Operator<decltype(v0)> B(vectorSpace);
//  JacobiPreconditioner jacobi(B);
//  CGSolver cg(B);//,jacobi);
//  auto vsol = cg(v0,rhs);
//  cout << vsol << endl;
  //  FunctionSpace R(std::make_unique<RealNumbers>());
  //  ExampleOperator_1 A(R);
//  auto f = Exp() - 2;
//  auto A = Algorithm::makeScalarRFFGenOperator( f , R );
  //  CG<double> cg(A);

  return 0;
}

