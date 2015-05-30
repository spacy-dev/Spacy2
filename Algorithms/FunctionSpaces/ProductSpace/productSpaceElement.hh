#ifndef ALGORITHM_PRODUCT_SPACE_ELEMENT_HH
#define ALGORITHM_PRODUCT_SPACE_ELEMENT_HH

#include <memory>
#include <vector>

#include "../../Interface/abstractFunctionSpaceElement.hh"
#include "../../functionSpaceElement.hh"

namespace Algorithm
{
  std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > cloneVariables(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& variables);

  /// Real number.
  class ProductSpaceElement : public AbstractFunctionSpaceElement
  {
  public:
    /**
     * @brief Construct real number with value x.
     * @param x initial value
     * @param space associated function space (RealSpace)
     */
    ProductSpaceElement(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& variables, const AbstractBanachSpace& space);

    /**
     * @brief Construct real number with initial value 0.
     * @param space associated function space (RealSpace)
     */
    explicit ProductSpaceElement(const AbstractBanachSpace& space);

    /**
     * @brief Get a copy of this real number.
     */
    std::unique_ptr<AbstractFunctionSpaceElement> clone() const final override;

    void copyTo(AbstractFunctionSpaceElement &) const final override;

    /// Print to os.
    void print(std::ostream& os) const final override;

    /// In-place summation.
    ProductSpaceElement& operator+=(const AbstractFunctionSpaceElement& y) final override;

    /// In-place subtraction.
    ProductSpaceElement& operator-=(const AbstractFunctionSpaceElement& y) final override;

    /// In-place multiplication.
    ProductSpaceElement& operator*=(double) final override;

    /// Get -x.
    std::unique_ptr<AbstractFunctionSpaceElement> operator- () const final override;

    /// Access value.
    double& coefficient(unsigned) final override;

    /// Access value.
    const double& coefficient(unsigned) const final override;

    /// Number of entries in coefficient vector (=1).
    unsigned size() const final override;

    AbstractFunctionSpaceElement& variable(unsigned i);

    const AbstractFunctionSpaceElement& variable(unsigned i) const;

    std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& variables();

    const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& variables() const;

  private:
    std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > variables_;

  };

  bool isProductSpaceElement(const AbstractFunctionSpaceElement& x);
}

#endif // ALGORITHM_PRODUCT_SPACE_ELEMENT_HH
