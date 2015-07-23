#ifndef ALGORITHM_C1_OPERATOR_HH
#define ALGORITHM_C1_OPERATOR_HH

#include <utility>

#include "linearOperator.hh"
#include "operator.hh"
#include "Interface/Operator/abstractC1Operator.hh"

namespace Algorithm
{
  namespace Interface { class AbstractC1Operator; }

  class FunctionSpace;

  /**
   * @brief A differentiable operator \f$A:\mathrm{X}\rightarrow\mathrm{Y}\f$.
   */
  class C1Operator : public Operator
  {
  public:
    /**
     * @brief Construct differentiable operator \f$A\f$ from implementation.
     */
    C1Operator(std::unique_ptr<Interface::AbstractC1Operator>&& impl);

    /**
     * @brief Compute first directional derivative \f$A(x)'dx\f$ at \f$x\f$ in direction \f$dx\f$.
     */
    FunctionSpaceElement d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const;

    /**
     * @brief Get linearization \f$A(x)'\f$ at \f$x\f$.
     */
    LinearOperator getLinearization(const FunctionSpaceElement& x) const;

    /**
     * \brief Access implementation.
     */
    Interface::AbstractC1Operator& impl();

    /**
     * \brief Access implementation.
     */
    const Interface::AbstractC1Operator& impl() const;
  };
}
#endif // ALGORITHM_C1_OPERATOR_HH
