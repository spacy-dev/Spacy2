#ifndef ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH

#include <memory>

#include "vector.hh"
#include "Util/Mixins/cloneable.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace Interface
  {
    /// \cond
    class AbstractLinearSolver;
    class Hessian;
    /// \endcond

    /**
     * @brief Abstract interface for functionals \f$f:\ X\rightarrow \mathbb{R}\f$.
     */
    class AbstractFunctional : public Mixin::Cloneable<AbstractFunctional>
    {
    public:
      /**
       * @brief Constructor.
       * @param domain Domain space \f$X\f$.
       */
      AbstractFunctional(VectorSpace* domain);

      virtual ~AbstractFunctional();

      /// Apply functional, i.e. compute \f$f(x)\f$.
      double operator()(const Vector& x) const;

      /// Apply derivative of functional, i.e. compute \f$f'(x) \in X^* \f$.
      virtual Vector d1(const Vector& x) const;

      /// Apply second derivative of functional, i.e. compute \f$f''(x)dx \in X^* \f$.
      virtual Vector d2(const Vector& x, const Vector& dx) const;

      /// Get second derivative \f$f''(x)': X \rightarrow X^*\f$.
      std::unique_ptr<Hessian> hessian(const Vector& x) const;

      /// Access domain space \f$X\f$.
      const VectorSpace& domain() const;

      /// Access pointer to domain space \f$X\f$.
      VectorSpace* domain_ptr() const;

    protected:
      virtual double d0(const Vector&) const = 0;

      friend class Hessian;

      virtual std::unique_ptr<Hessian> makeHessian(const Vector& x) const;
      virtual std::unique_ptr<AbstractLinearSolver> makeSolver() const;

    private:
      VectorSpace* domain_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH
