#ifndef ALGORITHM_FUNCTION_SPACE_HH
#define ALGORITHM_FUNCTION_SPACE_HH

#include <memory>
#include <utility>

#include "norm.hh"
#include "scalarProduct.hh"

#include "Interface/abstractFunctionSpace.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  /**
   * @brief Function space \f$(X,\|\cdot\|)\f$.
   */
  class FunctionSpace : public Mixin::SharedImpl<Interface::AbstractFunctionSpace>
  {
  public:
    /**
     * @brief Construct function space from implementation derived from AbstractFunctionSpace.
     */
    explicit FunctionSpace(std::shared_ptr<Interface::AbstractFunctionSpace> implementation);

//    FunctionSpace(const FunctionSpace&) = delete;
//    FunctionSpace& operator=(const FunctionSpace&) = delete;

//    FunctionSpace(FunctionSpace&&) = default;
//    FunctionSpace& operator=(FunctionSpace&&) = default;

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

  void connectPrimalDual(FunctionSpace& primalSpace, FunctionSpace& dualSpace);

}
#endif // ALGORITHM_FUNCTION_SPACE_HH
