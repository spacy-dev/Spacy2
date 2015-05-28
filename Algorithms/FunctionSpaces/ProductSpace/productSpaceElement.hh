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
    ProductSpaceElement(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& primalVariables,
                        const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& dualVariables, const AbstractBanachSpace& space);

    /**
     * @brief Construct real number with initial value 0.
     * @param space associated function space (RealSpace)
     */
    explicit ProductSpaceElement(const AbstractBanachSpace& space);

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
    unsigned dualSize() const;

    friend class ProductSpaceProduct;
    friend class ProductSpaceElement_PrimalReference;
    friend class ProductSpaceElement_PrimalConstReference;
    friend class ProductSpaceElement_DualReference;
    friend class ProductSpaceElement_DualConstReference;
    friend const AbstractFunctionSpaceElement& primalVariable(const AbstractFunctionSpaceElement& x, unsigned i);
    friend const AbstractFunctionSpaceElement& dualVariable(const AbstractFunctionSpaceElement& x, unsigned i);
    friend unsigned primalVariableSize(const AbstractFunctionSpaceElement& x);
    friend unsigned dualVariableSize(const AbstractFunctionSpaceElement& x);
    std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > primalVariables_;
    std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > dualVariables_;
  };

  bool isAnyProductSpaceElement(const AbstractFunctionSpaceElement& x);

  bool isProductSpaceElement(const AbstractFunctionSpaceElement& x);

  const AbstractFunctionSpaceElement& primalVariable(const AbstractFunctionSpaceElement& x, unsigned i);

  const AbstractFunctionSpaceElement& dualVariable(const AbstractFunctionSpaceElement& x, unsigned i);

  unsigned primalVariableSize(const AbstractFunctionSpaceElement& x);

  unsigned dualVariableSize(const AbstractFunctionSpaceElement& x);

  FunctionSpaceElement primal(FunctionSpaceElement& x);

  FunctionSpaceElement primal(const FunctionSpaceElement& x);

  FunctionSpaceElement dual(FunctionSpaceElement& x);

  FunctionSpaceElement dual(const FunctionSpaceElement& x);
}

#endif // ALGORITHM_PRODUCT_SPACE_ELEMENT_HH
