#ifndef ALGORITHM_HILBERT_SPACE_HH
#define ALGORITHM_HILBERT_SPACE_HH

#include <memory>

#include "banachSpace.hh"
#include "functionSpaceElement.hh"
#include "norm.hh"
#include "scalarProduct.hh"

namespace Algorithm
{
  namespace Interface { class AbstractHilbertSpace; }

  class FunctionSpaceElement;

  /**
   * @brief A function space. Creates function space elements and admits access to norm and, for Hilbert spaces, scalar product.
   */
  class HilbertSpace : public BanachSpace
  {
  public:
    /**
     * @brief Construct function space from implementation derived from Interface::AbstractHilbertSpace.
     */
    explicit HilbertSpace(std::shared_ptr<Interface::AbstractHilbertSpace> implementation);

    /**
     * @brief Change scalar product.
     */
    void setScalarProduct(const ScalarProduct& sp);

    /**
     * @brief Change scalar product.
     */
    void setScalarProduct(ScalarProduct&& sp);

    /**
     * @brief Access scalar product.
     */
    ScalarProduct scalarProduct() const;
  };
}
#endif // ALGORITHM_HILBERT_SPACE_HH
