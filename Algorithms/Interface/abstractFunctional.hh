#ifndef ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH

#include <memory>

#include "Util/Mixins/cloneable.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// \cond
    class AbstractVectorSpace;
    class AbstractVector;
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
      AbstractFunctional(std::shared_ptr<AbstractVectorSpace> domain);

      virtual ~AbstractFunctional();

      /// Apply functional, i.e. compute \f$f(x)\f$.
      double operator()(const AbstractVector& x) const;

      /// Apply derivative of functional, i.e. compute \f$f'(x) \in X^* \f$.
      virtual std::unique_ptr<AbstractVector> d1(const AbstractVector& x) const;

      /// Apply second derivative of functional, i.e. compute \f$f''(x)dx \in X^* \f$.
      virtual std::unique_ptr<AbstractVector> d2(const AbstractVector& x, const AbstractVector& dx) const;

      /// Get second derivative \f$f''(x)': X \rightarrow X^*\f$.
      std::unique_ptr<Hessian> hessian(const AbstractVector& x) const;

      /// Access domain space \f$X\f$.
      const AbstractVectorSpace& domain() const;

      /// Access shared domain space \f$X\f$.
      std::shared_ptr<AbstractVectorSpace> sharedDomain() const;

    protected:
      virtual double d0(const AbstractVector&) const = 0;

      friend class Hessian;

      virtual std::unique_ptr<Hessian> makeHessian(const AbstractVector& x) const;
      virtual std::unique_ptr<AbstractLinearSolver> makeSolver() const;

    private:
      std::shared_ptr<AbstractVectorSpace> domain_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH
