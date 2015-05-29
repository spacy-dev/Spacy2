#ifndef ALGORITHM_FUNCTION_SPACE_HH
#define ALGORITHM_FUNCTION_SPACE_HH

#include <memory>

#include "functionSpaceElement.hh"
#include "norm.hh"
#include "dualPairing.hh"

namespace Algorithm
{
  class FunctionSpaceElement;
  class AbstractBanachSpace;

  /**
   * @brief A function space. Creates function space elements and admits access to norm and, for Hilbert spaces, scalar product.
   */
  class FunctionSpace
  {
  public:
    /**
     * @brief Construct function space from implementation derived from AbstractBanachSpace.
     */
    explicit FunctionSpace(std::shared_ptr<AbstractBanachSpace> implementation);

    FunctionSpace& operator=(const FunctionSpace&) = delete;
//    explicit FunctionSpace(const AbstractBanachSpace& implementation);

    /**
     * @brief Change scalar product.
     */
    void setDualPairing(const DualPairing& sp);

    /**
     * @brief Access scalar product.
     */
    const DualPairing& getDualPairing() const;

    const DualPairing& getScalarProduct() const;

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
    AbstractBanachSpace const& impl() const;

    void setDualSpace(FunctionSpace& Y);

    const FunctionSpace* getDualSpace() const;

  private:
    friend class FunctionSpaceElement;
    std::shared_ptr<AbstractBanachSpace> impl_;
    Norm norm_;
    DualPairing dp_;
    FunctionSpace* dualSpace_ = nullptr;
  };
}
#endif // ALGORITHM_FUNCTION_SPACE_HH
