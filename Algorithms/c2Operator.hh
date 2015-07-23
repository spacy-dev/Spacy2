#ifndef ALGORITHM_C2_OPERATOR_HH
#define ALGORITHM_C2_OPERATOR_HH

#include <utility>

#include "functionSpaceElement.hh"
#include "linearOperator.hh"
#include "c1Operator.hh"

namespace Algorithm
{
  namespace Interface { class AbstractC2Operator; }

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
    C2Operator(std::unique_ptr<Interface::AbstractC2Operator>&& impl);

    /**
     * @brief Compute second directional derivative \f$A(x)'(dx,dy)\f$ at \f$x\f$ in direction \f$dx\f$ and \f$dy\f$.
     */
    FunctionSpaceElement d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;
  };
}
#endif // ALGORITHM_C2_OPERATOR_HH
