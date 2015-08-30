#include "derivative.hh"

#include "Algorithms/Util/Base/functionalBase.hh"
#include "Algorithms/Util/Base/operatorBase.hh"

#include "Algorithms/vector.hh"

namespace Algorithm
{
  namespace
  {
    class C2FunctionalDerivative : public OperatorBase
    {
    public:
      C2FunctionalDerivative(C2Functional f)
        : OperatorBase(f.domain(),f.domain().dualSpace()),
          f_(std::move(f))
      {}

      Vector operator()(const Vector& x) const
      {
        return f_.d1(x);
      }

      Vector d1(const Vector& x, const Vector& dx) const
      {
        return f_.d2(x,dx);
      }

      LinearOperator linearization(const Vector& x) const
      {
        return f_.hessian(x);
      }

    private:
      C2Functional f_;
    };

    class C2FunctionalDerivativeAtX
    {
    public:
      C2FunctionalDerivativeAtX(C2Functional f, Vector x)
        : f_(std::move(f)),
          x_(std::move(x)),
          domain_(f.domain())
      {}

      double operator()(const Vector& dx) const
      {
        return f_.d1(x_,dx);
      }

      const VectorSpace& domain() const noexcept
      {
        return domain_;
      }

    private:
      C2Functional f_;
      Vector x_;
      const VectorSpace& domain_;
    };
  }

  C1Operator derivative(C2Functional f)
  {
    return C2FunctionalDerivative(std::move(f));
  }

  Functional derivative(C2Functional f, Vector x)
  {
    return C2FunctionalDerivativeAtX(std::move(f),std::move(x));
  }
}
