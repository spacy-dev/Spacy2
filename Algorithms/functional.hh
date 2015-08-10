#ifndef ALGORITHM_FUNCTIONAL_HH
#define ALGORITHM_FUNCTIONAL_HH

#include <memory>

#include "functionSpace.hh"

#include "Interface/Functional/abstractFunctional.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

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
