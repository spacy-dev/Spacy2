#ifndef ALGORITHM_FUNCTION_SPACE_HH
#define ALGORITHM_FUNCTION_SPACE_HH

#include <memory>

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
  class FunctionSpace
  {
  public:
    /**
     * @brief Construct function space from implementation derived from AbstractHilbertSpace.
     */
    explicit FunctionSpace(std::unique_ptr<AbstractHilbertSpace>&& implementation);

//    explicit FunctionSpace(const AbstractHilbertSpace& implementation);

    /**
     * @brief Change scalar product.
     */
    void setScalarProduct(const ScalarProduct& sp);

    /**
     * @brief Access scalar product.
     */
    const ScalarProduct& getScalarProduct() const;

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
    AbstractHilbertSpace const& impl() const;

  private:
    friend class FunctionSpaceElement;
    std::unique_ptr<AbstractHilbertSpace> impl_;
    Norm norm_;
    ScalarProduct sp_;
  };
}
#endif // ALGORITHM_FUNCTION_SPACE_HH
