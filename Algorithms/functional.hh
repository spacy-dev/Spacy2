#ifndef ALGORITHM_FUNCTIONAL_HH
#define ALGORITHM_FUNCTIONAL_HH

#include <memory>

#include "functionSpace.hh"
#include "functionSpaceElement.hh"
#include "linearOperator.hh"

#include "Interface/abstractFunctional.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  /**
   * @brief A functional \f$F\f$.
   */
  class Functional : public Mixin::UniqueImpl<Interface::AbstractFunctional>
  {
  public:
    /**
     * @brief Construct functional from implementation.
     */
    Functional(std::unique_ptr<Interface::AbstractFunctional>&& implementation);

    /**
     * @brief Evaluate functional \f$F\f$ at \f$x\f$.
     */
    double operator()(const FunctionSpaceElement& x) const;

    /**
     * @brief Compute first directional derivative \f$F(x)'dx\f$ at \f$x\f$ in direction \f$dx\f$.
     */
    double d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const;

    /**
     * @brief Compute first directional derivative \f$F(x)': X \rightarrow X^*\f$ at \f$x\f$.
     */
    FunctionSpaceElement d1(const FunctionSpaceElement &x) const;

    /**
     * @brief Compute element of dual space \f$F(x)''dx\f$.
     */
    FunctionSpaceElement d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const;

    /**
     * @brief Compute second directional derivative \f$F(x)''(dx,dy)\f$ at \f$x\f$ in directions \f$dx\f$ and \f$dy\f$.
     */
    double d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;


    /**
     * @brief Get hessian as linear operator \f$H: X \rightarrow X^* \f$.
     */
    LinearOperator hessian(const FunctionSpaceElement& x) const;

    /**
     * @brief Access underlying domain.
     */
    FunctionSpace& domain();

    /**
     * @brief Access underlying domain.
     */
    const FunctionSpace& domain() const;

  private:
    FunctionSpace domain_;
  };
}
#endif // ALGORITHM_FUNCTIONAL_HH
