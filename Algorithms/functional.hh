#ifndef ALGORITHM_FUNCTIONAL_HH
#define ALGORITHM_FUNCTIONAL_HH

#include <memory>

#include "Util/impl.hh"

namespace Algorithm
{
  class AbstractFunctional;
  class FunctionSpaceElement;

  /**
   * @brief A functional \f$F\f$.
   */
  class Functional : public SharedImpl<AbstractFunctional>
  {
  public:
    /**
     * @brief Construct functional from implementation.
     */
    Functional(std::shared_ptr<Algorithm::AbstractFunctional> impl);

    /**
     * @brief Evaluate functional \f$F\f$ at \f$x\f$.
     */
    double operator()(const FunctionSpaceElement& x);
  };

  /**
   * @brief Convenient generation of functional from implementation arguments.
   * @return Functional( std::make_shared<Implementation>(std::forward<Args>(args)...) )
   */
  template <class Implementation, class... Args>
  Functional makeFunctional(Args&&... args)
  {
    return Functional( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_FUNCTIONAL_HH
