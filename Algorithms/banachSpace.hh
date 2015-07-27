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
   * @brief Banach space \f$(X,\|\cdot\|)\f$. Creates function space elements and admits access to norm.
   */
  class BanachSpace : public Mixin::SharedImpl<Interface::AbstractBanachSpace>
  {
  public:
    /**
     * @brief Construct function space from implementation derived from AbstractBanachSpace.
     */
    explicit BanachSpace(std::shared_ptr<Interface::AbstractBanachSpace> implementation);

    virtual ~BanachSpace() = default;

    BanachSpace(const BanachSpace&) = delete;
    BanachSpace& operator=(const BanachSpace&) = delete;

    BanachSpace(BanachSpace&&) = default;
    BanachSpace& operator=(BanachSpace&&) = default;

    /**
     * @brief Change norm of space.
     */
    void setNorm(const Norm& norm);

    /**
     * @brief Access norm.
     */
    Norm norm() const;

    /**
     * @brief Create new function space element.
     */
    FunctionSpaceElement element() const;

    /**
     * @brief Access unique index of the function space.
     */
    unsigned index() const;
  };
}
#endif // ALGORITHM_BANACH_SPACE_HH
