#ifndef ALGORITHM_BANACH_SPACE_HH
#define ALGORITHM_BANACH_SPACE_HH

#include <memory>
#include <utility>

#include "functionSpaceElement.hh"
#include "norm.hh"

namespace Algorithm
{
  class FunctionSpaceElement;
  class AbstractBanachSpace;

  /**
   * @brief Banach space. Creates function space elements and admits access to norm.
   */
  class BanachSpace
  {
  public:
    /**
     * @brief Construct function space from implementation derived from AbstractBanachSpace.
     */
    explicit BanachSpace(std::shared_ptr<AbstractBanachSpace> implementation);

    /**
     * @brief Access norm.
     */
    const Norm& getNorm() const;

    /**
     * @brief Create new function space element.
     */
    FunctionSpaceElement element() const;

    /**
     * @brief Access unique index of the function space.
     */
    unsigned index() const;

    /**
     * @brief Access implementation.
     */
    AbstractBanachSpace& impl();

    /**
     * @brief Access implementation.
     */
    AbstractBanachSpace const& impl() const;

  private:
    friend class FunctionSpaceElement;
    std::shared_ptr<AbstractBanachSpace> impl_;
    Norm norm_;
  };

  template <class Implementation, class... Args>
  auto makeBanachSpace(Args&&... args)
  {
    return BanachSpace(std::make_shared<Implementation>(std::forward<Args>(args)...));
  }
}
#endif // ALGORITHM_BANACH_SPACE_HH
