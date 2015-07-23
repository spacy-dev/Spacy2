#ifndef ALGORITHM_FUNCTIONAL_HH
#define ALGORITHM_FUNCTIONAL_HH

#include <memory>

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
    Functional(std::unique_ptr<Interface::AbstractFunctional>&& impl);

    /**
     * @brief Evaluate functional \f$F\f$ at \f$x\f$.
     */
    double operator()(const FunctionSpaceElement& x) const;
  };
}
#endif // ALGORITHM_FUNCTIONAL_HH
