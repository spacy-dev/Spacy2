#ifndef ALGORITHM_C1_OPERATOR_HH
#define ALGORITHM_C1_OPERATOR_HH

#include <utility>

#include "linearOperator.hh"
#include "operator.hh"

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
  };

  /**
   * @brief Convenient generation of differentiable operator from implementation arguments.
   * @return C1Operator( std::make_shared<Implementation>(std::forward<Args>(args)...) )
   */
  template <class Implementation, class... Args>
  C1Operator makeC1Operator(Args&&... args)
  {
    return C1Operator( std::make_unique<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_C1_OPERATOR_HH
