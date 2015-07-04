#ifndef ALGORITHM_C1_FUNCTIONAL_HH
#define ALGORITHM_C1_FUNCTIONAL_HH

#include "functional.hh"

namespace Algorithm
{
  namespace Interface { class AbstractC1Functional; }

  class FunctionSpaceElement;

  /**
   * @brief A differentiable functional \f$F:\mathrm{x}\rightarrow\mathbb{R}.
   */
  class C1Functional : public Functional
  {
  public:
    /**
     * @brief Construct differentiable functional from implementation.
     */
    C1Functional(std::unique_ptr<Interface::AbstractC1Functional>&& impl);

    /**
     * @brief Compute first directional derivative \f$F(x)'dx\f$ at \f$x\f$ in direction \f$dx\f$.
     */
    double d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const;

    /**
     * \brief Access implementation.
     */
    Interface::AbstractC1Functional& impl();

    /**
     * \brief Access implementation.
     */
    const Interface::AbstractC1Functional& impl() const;
  };

  /**
   * @brief Convenient generation of differentiable functional from implementation arguments.
   * @return C1Functional( std::make_shared<Implementation>(std::forward<Args>(args)...) )
   */
  template <class Implementation, class... Args>
  C1Functional makeC1Functional(Args&&... args)
  {
    return C1Functional( std::make_unique<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_C1_FUNCTIONAL_HH
