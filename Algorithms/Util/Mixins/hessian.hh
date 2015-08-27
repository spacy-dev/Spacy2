#ifndef ALGORITHM_UTIL_MIXINS_HESSIAN_HH
#define ALGORITHM_UTIL_MIXINS_HESSIAN_HH

#include <functional>
#include <memory>
#include <utility>

#include "linearSolver.hh"
#include "operator.hh"
#include "vector.hh"

namespace Algorithm
{
  namespace Mixin
  {
    template <class Derived>
    class Hessian
    {
    public:
      Hessian(std::function<void(Vector)> beforeHessian = [](Vector){})
        : beforeHessian_(std::move(beforeHessian))
      {}

      void setUseSolver(bool useSolver = true) noexcept
      {
        useSolver_ = useSolver;
      }

      bool useSolver() const noexcept
      {
        return useSolver_;
      }

      LinearOperator hessian(const Vector& x) const
      {
        beforeHessian_(x);
        auto newOperator = static_cast<const Derived&>(*this);
        newOperator.disableAssembly();
        if( !useSolver() )
          return ::Algorithm::Hessian( std::move(newOperator) , x );
        return ::Algorithm::Hessian( std::move(newOperator) , x , static_cast<const Derived&>(*this).solver_ );
      }

    private:
      std::function<void(const Vector&)> beforeHessian_ = [](const Vector&){};
      bool useSolver_ = true;
    };

  }
}

#endif // ALGORITHM_UTIL_MIXINS_HESSIAN_HH

