#ifndef ALGORITHM_VECTOR_SPACE_HH
#define ALGORITHM_VECTOR_SPACE_HH

#include <memory>
#include <utility>

#include "norm.hh"
#include "scalarProduct.hh"

#include "Interface/abstractVectorSpace.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class Vector;

  /**
   * @brief Function space \f$(X,\|\cdot\|)\f$.
   */
  class VectorSpace : public Mixin::SharedImpl<Interface::AbstractVectorSpace>
  {
  public:
    /**
     * @brief Construct function space from implementation derived from AbstractVectorSpace.
     */
    explicit VectorSpace(std::shared_ptr<Interface::AbstractVectorSpace> implementation);

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
    Vector element() const;

    /**
     * @brief Access unique index of the function space.
     */
    unsigned index() const;

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

  /**
   * @brief Relate function spaces.
   */
  void connectPrimalDual(VectorSpace& primalSpace, VectorSpace& dualSpace);

}
#endif // ALGORITHM_VECTOR_SPACE_HH
