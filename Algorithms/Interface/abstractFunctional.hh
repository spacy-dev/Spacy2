#ifndef ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH

#include <memory>

#include "Util/Mixins/cloneable.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// \cond
    class AbstractFunctionSpace;
    class AbstractFunctionSpaceElement;
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
      AbstractFunctional(std::shared_ptr<AbstractFunctionSpace> domain);

      virtual ~AbstractFunctional();

      /// Apply functional, i.e. compute \f$f(x)\f$.
      double operator()(const AbstractFunctionSpaceElement& x) const;

      /// Apply derivative of functional, i.e. compute \f$f'(x) \in X^* \f$.
      virtual std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& x) const;

      /// Apply second derivative of functional, i.e. compute \f$f''(x)dx \in X^* \f$.
      virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx) const;

      /// Get second derivative \f$f''(x)': X \rightarrow X^*\f$.
      std::unique_ptr<Hessian> hessian(const AbstractFunctionSpaceElement& x) const;

      /// Access domain space \f$X\f$.
      const AbstractFunctionSpace& domain() const;

      /// Access shared domain space \f$X\f$.
      std::shared_ptr<AbstractFunctionSpace> sharedDomain() const;

    protected:
      virtual double d0(const AbstractFunctionSpaceElement&) const = 0;

      friend class Hessian;

      virtual std::unique_ptr<Hessian> makeHessian(const AbstractFunctionSpaceElement& x) const;
      virtual std::unique_ptr<AbstractLinearSolver> makeSolver() const;

    private:
      std::shared_ptr<AbstractFunctionSpace> domain_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C0_FUNCTIONAL_HH
