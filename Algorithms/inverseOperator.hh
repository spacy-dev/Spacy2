#ifndef ALGORITHM_INVERSE_OPERATOR_HH
#define ALGORITHM_INVERSE_OPERATOR_HH

#include <memory>

namespace Algorithm
{
  class FunctionSpaceElement;
  class AbstractLinearSolver;

  /**
   * @brief Models \f$F^{-1}\f$.
   */
  class InverseOperator
  {
  public:
    InverseOperator(std::unique_ptr<AbstractLinearSolver>&& impl);

    /**
     * @brief Compute \f$F^{-1}y\f$ from initial guess \f$x\f$.
     */
    FunctionSpaceElement operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const;

    /**
     * @brief Compute \f$F^{-1}y\f$.
     */
    FunctionSpaceElement operator()(const FunctionSpaceElement& y) const;

  private:
    std::shared_ptr<AbstractLinearSolver> impl_;
  };
}

#endif // ALGORITHM_INVERSE_OPERATOR_HH
