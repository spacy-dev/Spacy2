#ifndef ALGORITHM_BANACH_SPACE_HH
#define ALGORITHM_BANACH_SPACE_HH

#include <memory>
#include <utility>

#include "norm.hh"
#include "Interface/abstractBanachSpace.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  /**
   * @brief Banach space \f$X\f$. Creates function space elements and admits access to norm.
   */
  class BanachSpace : public Mixin::SharedImpl<Interface::AbstractBanachSpace>
  {
  public:
    /**
     * @brief Construct function space from implementation derived from AbstractBanachSpace.
     */
    explicit BanachSpace(std::shared_ptr<Interface::AbstractBanachSpace> implementation);

    BanachSpace(const BanachSpace&) = delete;
    BanachSpace& operator=(const BanachSpace&) = delete;

    BanachSpace(BanachSpace&&) = default;
    BanachSpace& operator=(BanachSpace&&) = default;

    /**
     * @brief Change norm of space.
     */
    void setNorm(Norm&& norm);

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

  private:
    friend class FunctionSpaceElement;
    Norm norm_;
  };

  /**
   * @brief Convenient generation of banach space from implementation arguments.
   * @return BanachSpace(std::make_shared<Implementation>(std::forward<Args>(args)...))
   */
  template <class Implementation, class... Args>
  auto makeBanachSpace(Args&&... args)
  {
    return BanachSpace(std::make_shared<Implementation>(std::forward<Args>(args)...));
  }
}
#endif // ALGORITHM_BANACH_SPACE_HH
