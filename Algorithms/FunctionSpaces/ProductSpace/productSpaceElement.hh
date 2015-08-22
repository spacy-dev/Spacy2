#ifndef ALGORITHM_PRODUCT_SPACE_ELEMENT_HH
#define ALGORITHM_PRODUCT_SPACE_ELEMENT_HH

#include <memory>
#include <vector>

#include "vector.hh"
#include "Interface/vectorBase.hh"
#include "Util/Mixins/primalDualSwitch.hh"

namespace Algorithm
{
  /// \cond
  class ProductSpace;
  /// \endcond

  /**
   * @brief Product space element.
   */
  class ProductSpaceElement : public VectorBase<ProductSpaceElement>, public Mixin::PrimalDualSwitch
  {
  public:
    /**
     * @brief Construct real number with initial value 0.
     * @param space associated function space (RealSpace)
     */
    explicit ProductSpaceElement(const VectorSpace& space);

    ProductSpaceElement(const ProductSpaceElement& other);

//    void copyTo(Vector &) const;

    /// Print to os.
//    void print(std::ostream& os) const;

    /// Assignment.
    ProductSpaceElement& operator=(const ProductSpaceElement& y);

    /// In-place summation.
    ProductSpaceElement& operator+=(const ProductSpaceElement& y);

    /// Axpy-operation \f$x = x + ay\f$.
//    AbstractVector& axpy(double a, const AbstractVector& y);

    /// In-place subtraction.
    ProductSpaceElement& operator-=(const ProductSpaceElement& y);

    /// In-place multiplication.
    ProductSpaceElement& operator*=(double);

    /// Get -x.
    ProductSpaceElement operator- () const;

    bool operator==(const ProductSpaceElement& y) const;

    /// Access value.
    double& coefficient(unsigned);

    /// Access value.
    const double& coefficient(unsigned) const;

    /// Number of entries in coefficient vector.
    unsigned size() const;

    Vector& variable(unsigned i);

    const Vector& variable(unsigned i) const;

    ProductSpaceElement& primalComponent();

    const ProductSpaceElement& primalComponent() const;

    void setPrimalComponent(const Vector& y);

    ProductSpaceElement& dualComponent();

    const ProductSpaceElement& dualComponent() const;

    void setDualComponent(const Vector& y);

    const ProductSpace& productSpace() const;

    bool isPrimalDualProductSpaceElement() const;

    /// Apply as dual element.
    double operator()(const ProductSpaceElement& y) const;

  private:
//    ProductSpaceElement* cloneImpl() const;

    std::vector<Vector> variables_ ;
  };
}

#endif // ALGORITHM_PRODUCT_SPACE_ELEMENT_HH
