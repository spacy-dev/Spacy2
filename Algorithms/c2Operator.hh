#ifndef ALGORITHM_C2_OPERATOR_HH
#define ALGORITHM_C2_OPERATOR_HH

#include <utility>

#include "functionSpaceElement.hh"
#include "linearOperator.hh"
#include "c1Operator.hh"

namespace Algorithm
{
  class AbstractC2Operator;
  class FunctionSpace;

  /**
   * @brief A twice differentiable operator \f$A\f$.
   */
  class C2Operator : public C1Operator
  {
  public:
    /**
     * @brief Construct twice differentiable operator \f$A\f$ from implementation.
     */
    C2Operator(std::shared_ptr<AbstractC2Operator> impl);

    /**
     * @brief Compute second directional derivative \f$A(x)'(dx,dy)\f$ at \f$x\f$ in direction \f$dx\f$ and \f$dy\f$.
     */
    FunctionSpaceElement d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;
  };

  /**
   * @brief Convenient generation of twice differentiable operator from implementation arguments.
   * @return C2Operator( std::make_shared<Implementation>(std::forward<Args>(args)...) )
   */
  template <class Implementation, class... Args>
  C2Operator makeC2Operator(Args&&... args)
  {
    return C2Operator( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_C2_OPERATOR_HH
