#ifndef ALGORITHM_HILBERT_SPACE_HH
#define ALGORITHM_HILBERT_SPACE_HH

#include <memory>

#include "banachSpace.hh"
#include "functionSpaceElement.hh"
#include "norm.hh"
#include "scalarProduct.hh"

namespace Algorithm
{
  class FunctionSpaceElement;
  class AbstractHilbertSpace;

  /**
   * @brief A function space. Creates function space elements and admits access to norm and, for Hilbert spaces, scalar product.
   */
  class HilbertSpace : public BanachSpace
  {
  public:
    /**
     * @brief Construct function space from implementation derived from AbstractBanachSpace.
     */
    explicit HilbertSpace(std::shared_ptr<AbstractHilbertSpace> implementation);

    /**
     * @brief Change scalar product.
     */
    void setScalarProduct(const ScalarProduct& sp);

    /**
     * @brief Access scalar product.
     */
    const ScalarProduct& getScalarProduct() const;

    /**
     * @brief Access implementation.
     */
    AbstractHilbertSpace& impl();

    /**
     * @brief Access implementation.
     */
    AbstractHilbertSpace const& impl() const;

  private:
    friend class FunctionSpaceElement;
    Norm norm_;
    ScalarProduct sp_;
  };

  template <class Implementation, class... Args>
  auto makeHilbertSpace(Args&&... args)
  {
    return HilbertSpace(std::make_unique<Implementation>(std::forward<Args>(args)...));
  }
}
#endif // ALGORITHM_HILBERT_SPACE_HH
