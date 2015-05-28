#ifndef ALGORITHM_PRODUCT_SPACE_PRIMAL_ELEMENT_HH
#define ALGORITHM_PRODUCT_SPACE_PRIMAL_ELEMENT_HH

#include <memory>
#include <vector>

#include "../../Interface/abstractFunctionSpaceElement.hh"
#include "../../functionSpaceElement.hh"
#include "productSpaceElement.hh"

namespace Algorithm
{

  class ProductSpaceElement_PrimalReference : public AbstractFunctionSpaceElement
  {
  public:
    /**
     * @brief Construct real number with value x.
     * @param x initial value
     * @param space associated function space (RealSpace)
     */
    ProductSpaceElement_PrimalReference(ProductSpaceElement& element);

    /**
     * @brief Get a copy of this real number.
     */
    std::unique_ptr<AbstractFunctionSpaceElement> clone() const final override;

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

  private:
    unsigned primalSize() const;

    friend class ProductSpaceProduct;
    friend const AbstractFunctionSpaceElement& primalVariable(const AbstractFunctionSpaceElement& x, unsigned i);
    friend const AbstractFunctionSpaceElement& dualVariable(const AbstractFunctionSpaceElement& x, unsigned i);
    friend unsigned primalVariableSize(const AbstractFunctionSpaceElement& x);
    friend unsigned dualVariableSize(const AbstractFunctionSpaceElement& x);
    ProductSpaceElement& productSpaceElement_;
  };


  class ProductSpaceElement_PrimalConstReference : public AbstractFunctionSpaceElement
  {
  public:
    /**
     * @brief Construct real number with value x.
     * @param x initial value
     * @param space associated function space (RealSpace)
     */
    ProductSpaceElement_PrimalConstReference(const ProductSpaceElement& element);

    /**
     * @brief Get a copy of this real number.
     */
    std::unique_ptr<AbstractFunctionSpaceElement> clone() const final override;

    /// Print to os.
    void print(std::ostream& os) const final override;

    /// In-place summation.
    ProductSpaceElement_PrimalConstReference& operator+=(const AbstractFunctionSpaceElement& y) final override;

    /// In-place subtraction.
    ProductSpaceElement_PrimalConstReference& operator-=(const AbstractFunctionSpaceElement& y) final override;

    /// In-place multiplication.
    ProductSpaceElement_PrimalConstReference& operator*=(double) final override;

    /// Get -x.
    std::unique_ptr<AbstractFunctionSpaceElement> operator- () const final override;

    /// Access value.
    double& coefficient(unsigned) final override;

    /// Access value.
    const double& coefficient(unsigned) const final override;

    /// Number of entries in coefficient vector (=1).
    unsigned size() const final override;

  private:
    unsigned primalSize() const;

    friend class ProductSpaceProduct;
    friend const AbstractFunctionSpaceElement& primalVariable(const AbstractFunctionSpaceElement& x, unsigned i);
    friend const AbstractFunctionSpaceElement& dualVariable(const AbstractFunctionSpaceElement& x, unsigned i);
    friend unsigned primalVariableSize(const AbstractFunctionSpaceElement& x);
    friend unsigned dualVariableSize(const AbstractFunctionSpaceElement& x);
    const ProductSpaceElement& productSpaceElement_;
  };

  bool isPrimalProductSpaceElement(const AbstractFunctionSpaceElement& x);

  bool isProductSpaceElement_PrimalReference(const AbstractFunctionSpaceElement& x);

  bool isProductSpaceElement_PrimalConstReference(const AbstractFunctionSpaceElement& x);
}

#endif // ALGORITHM_PRODUCT_SPACE_PRIMAL_ELEMENT_HH
