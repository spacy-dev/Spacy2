#ifndef ALGORITHM_C2_FUNCTIONAL_HH
#define ALGORITHM_C2_FUNCTIONAL_HH

#include "c1Functional.hh"

namespace Algorithm
{
  namespace Interface{ class AbstractC2Functional; }

  class C1Operator;

  /**
   * @brief A twice differentiable functional \f$F\f$.
   */
  class C2Functional : public C1Functional
  {
  public:
    /**
     * @brief Construct twice differentiable functional from implementation.
     */
    C2Functional(std::unique_ptr<Interface::AbstractC2Functional>&& impl);

    /**
     * @brief Compute element of dual space \f$F(x)''dx\f$.
     */
    FunctionSpaceElement d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const;

    /**
     * @brief Compute second directional derivative \f$F(x)''(dx,dy)\f$ at \f$x\f$ in directions \f$dx\f$ and \f$dy\f$.
     */
    double d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;

    /**
     * @brief Get \f$dF(x,dx) = F(x)'dx\f$.
     */
    C1Operator getDerivative() const;

    /**
     * \brief Access implementation.
     */
    Interface::AbstractC2Functional& impl();

    /**
     * \brief Access implementation.
     */
    const Interface::AbstractC2Functional& impl() const;
  };

  /**
   * @brief Convenient generation of twice differentiable functional from implementation arguments.
   * @return C2Functional( std::make_shared<Implementation>(std::forward<Args>(args)...) )
   */
  template <class Implementation, class... Args>
  C2Functional makeC2Functional(Args&&... args)
  {
    return C2Functional( std::make_unique<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_C2_FUNCTIONAL_HH
