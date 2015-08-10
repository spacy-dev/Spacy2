#ifndef ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH

#include "abstractC1Functional.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// \cond
    class AbstractLinearSolver;
    class Hessian;
    /// \endcond

    /**
     * @brief Abstract interface for twice differentiable functionals \f$f:\ X\rightarrow \mathbb{R}\f$.
     */
    class AbstractC2Functional : public AbstractC1Functional
    {
    public:
      /**
       * @brief Constructor.
       * @param domain Domain space \f$X\f$.
       */
      AbstractC2Functional(std::shared_ptr<AbstractFunctionSpace> domain);

      /// Get second derivative \f$f''(x)': X \rightarrow X^*\f$.
      std::unique_ptr<Hessian> hessian(const AbstractFunctionSpaceElement& x) const;

      /// Apply second derivative of functional, i.e. compute \f$f''(x)dx \in X^* \f$.
      virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx) const = 0;

      virtual void setOrigin(const AbstractFunctionSpaceElement& x) const;

    protected:
      friend class Hessian;

      virtual std::unique_ptr<Hessian> makeHessian(const AbstractFunctionSpaceElement& x) const;
      virtual std::unique_ptr<AbstractLinearSolver> makeSolver() const;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH
