#include <iostream>

#include <armadillo>

#include "../../RFFGen/src/RFFGen.hh"
#include "ConjugateGradients/cg.hh"
#include "FunctionSpaces/RealNumbers/realSpace.hh"
#include "FunctionSpaces/VectorSpace/vectorSpace.hh"
#include "Test/scalarRFFGenOperator.hh"
#include "Newton/affinecovariantnewton.hh"
#include "spaces.hh"
#include "FunctionSpaces/VectorSpace/cgSolver.hh"
#include "ConjugateGradients/jacobipreconditioner.hh"

namespace Algorithm
{
  template <class VectorSpaceElement>
  class Test2Operator : public Operator
  {
  public:
    Test2Operator(const FunctionSpace& space)
      : Operator(space,space)
    {}

    ~Test2Operator(){}

    void update(const FunctionSpaceElement &x) override
    {
      std::cout << "AAAAAAAAAAAAAAA" << std::endl;
      assert(false);
    }

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const override
    {
      return computeResult(x);
    }

    FunctionSpaceElement d1(const FunctionSpaceElement &dx) const override
    {
      return computeResult(dx);
    }

  private:
    auto computeResult(const FunctionSpaceElement& x) const
    {
      auto y = x;
      for(unsigned i = 0; i < x.size(); ++i)
      {
        if(i==0) y.coefficient(0) = 2*x.coefficient(0) + x.coefficient(1);
        else if(i+1==x.size()) y.coefficient(i) = 2*x.coefficient(i) + x.coefficient(i-1);
        else y.coefficient(i) = 2*x.coefficient(i) + x.coefficient(i+1) + x.coefficient(i-1);
      }
      return y;
    }
  };
}

using namespace std;

int main()
{
  using namespace RFFGen::CMath;
  using namespace Algorithm;
  using Vector = arma::vec::fixed<10>;
  FunctionSpace vectorSpace(std::make_unique<VectorSpace<Vector> >());
  auto v0 = vectorSpace.element();
  auto rhs = vectorSpace.element();
  for(unsigned i=0; i<rhs.size(); ++i) rhs.coefficient(i) = (i==0 || i+1==rhs.size()) ? 3 : 4;
  Test2Operator<decltype(v0)> B(vectorSpace);
  JacobiPreconditioner jacobi(B);
  CGSolver cg(B);//,jacobi);
  auto vsol = cg(v0,rhs);
  cout << vsol << endl;
  //  FunctionSpace R(std::make_unique<RealNumbers>());
  using Spaces::R;
  //  ExampleOperator_1 A(R);
  auto f = Exp() - 2;
  auto A = Algorithm::makeScalarRFFGenOperator( f , R );
  //  CG<double> cg(A);
  auto newton = Algorithm::makeNewton(A,true);
  //  AffineCovariantNewton<double> newton(A,true);
  using Algorithm::Norm;
  cout << "Hello World!" << endl;
  auto x = R.element();
  auto dx = R.element();
  dx.coefficient(0) = 1;
  //  auto x2 = R2.element();
  auto y = A(x);
  auto z = A.d1(x);
  auto sp = R.getScalarProduct();
  auto norm = R.getNorm();
  cout << x.spaceIndex() << endl;
  cout << x << endl;
  x.coefficient(0) = 5;
  cout << x << endl;
  //  cout << x2.spaceIndex() << endl;
  //  cout << x << endl;
  //  cout << x2 << endl;
  //  cout << "norm: " << (dynamic_cast<const HilbertSpaceNorm*>(&norm)!=nullptr) << endl;
  cout << norm(x) << endl;
  //  cout << "sp: " << (dynamic_cast<const RealProduct*>(&sp)!=nullptr) << endl;
  cout << sp(x,x) << endl;
  cout << x*x << endl;
  cout << "y: " << y << endl;
  cout << "z: " << z << endl;
  auto sol = newton.solve(x);
  cout << "sol: " << sol << endl;
  return 0;
}

