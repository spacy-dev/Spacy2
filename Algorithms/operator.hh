#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <memory>

#include "functionSpace.hh"

#include "Interface/Operator/abstractOperator.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class FunctionSpaceElement;
  class LinearOperator;

  class Operator : public Mixin::UniqueImpl<Interface::AbstractOperator>
  {
  public:
    Operator(std::unique_ptr<Interface::AbstractOperator>&& implementation);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

    /**
     * @brief Compute first directional derivative \f$A(x)'dx\f$ at \f$x\f$ in direction \f$dx\f$.
     */
    FunctionSpaceElement d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const;

    /**
     * @brief Compute second directional derivative \f$A(x)'(dx,dy)\f$ at \f$x\f$ in direction \f$dx\f$ and \f$dy\f$.
     */
    FunctionSpaceElement d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;

    /**
     * @brief Get linearization \f$A(x)'\f$ at \f$x\f$.
     */
    LinearOperator linearization(const FunctionSpaceElement& x) const;

    /**
     * @brief Access underlying domain.
     */
    FunctionSpace& domain();

    /**
     * @brief Access range space.
     */
    FunctionSpace& range();

    /**
     * @brief Access underlying domain.
     */
    const FunctionSpace& domain() const;

    /**
     * @brief Access range space
     */
    const FunctionSpace& range() const;

  private:
    FunctionSpace domain_, range_;
  };
}
#endif // ALGORITHM_OPERATOR_HH
