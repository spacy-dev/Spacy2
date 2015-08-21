#ifndef ALGORITHM_PRODUCT_SPACE_ELEMENT_HH
#define ALGORITHM_PRODUCT_SPACE_ELEMENT_HH

#include <memory>
#include <vector>

#include "vector.hh"
#include "Interface/abstractVector.hh"
#include "Util/Mixins/primalDualSwitch.hh"

namespace Algorithm
{
  class ProductSpace;

  std::vector<std::unique_ptr<Interface::AbstractVector> > cloneVariables(const std::vector<std::unique_ptr<Interface::AbstractVector> >& variables);

  /**
   * @brief Product space element.
   */
  class ProductSpaceElement : public Interface::AbstractVector , public Mixin::PrimalDualSwitch
  {
  public:
    /**
     * @brief Construct real number with initial value 0.
     * @param space associated function space (RealSpace)
     */
    explicit ProductSpaceElement(const VectorSpace& space);

    ProductSpaceElement(const ProductSpaceElement& other);

    void copyTo(Interface::AbstractVector &) const final override;

    /// Print to os.
    void print(std::ostream& os) const final override;

    /// Assignment.
    ProductSpaceElement& operator=(const Interface::AbstractVector& y) final override;

    /// In-place summation.
    ProductSpaceElement& operator+=(const Interface::AbstractVector& y) final override;

    /// Axpy-operation \f$x = x + ay\f$.
    AbstractVector& axpy(double a, const AbstractVector& y) final override;

    /// In-place subtraction.
    ProductSpaceElement& operator-=(const Interface::AbstractVector& y) final override;

    /// In-place multiplication.
    ProductSpaceElement& operator*=(double) final override;

    /// Get -x.
    std::unique_ptr<Interface::AbstractVector> operator- () const final override;

    /// Access value.
    double& coefficient(unsigned) final override;

    /// Access value.
    const double& coefficient(unsigned) const final override;

    /// Number of entries in coefficient vector.
    unsigned size() const final override;

    Interface::AbstractVector& variable(unsigned i);

    const Interface::AbstractVector& variable(unsigned i) const;

    ProductSpaceElement& primalComponent();

    const ProductSpaceElement& primalComponent() const;

    void setPrimalComponent(std::unique_ptr<Interface::AbstractVector>&& y);

    ProductSpaceElement& dualComponent();

    const ProductSpaceElement& dualComponent() const;

    void setDualComponent(std::unique_ptr<Interface::AbstractVector>&& y);

    const ProductSpace& productSpace() const;

    bool isPrimalDualProductSpaceElement() const;

  private:
    ProductSpaceElement* cloneImpl() const final override;

    /// Apply as dual element.
    double applyAsDualTo(const Interface::AbstractVector& y) const final override;

    std::vector<std::unique_ptr<Interface::AbstractVector> > variables_ ;
  };
}

#endif // ALGORITHM_PRODUCT_SPACE_ELEMENT_HH
