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

    void reset() const
    {
      disablePrimal_ = disableDual_ = false;
    }

    void disablePrimal() const { disablePrimal_ = true; }
    void disableDual() const { disableDual_ = true; }

  private:
    unsigned primalSize() const;
    unsigned dualSize() const;

    friend class ProductSpaceProduct;
    friend const AbstractFunctionSpaceElement& primalVariable(const AbstractFunctionSpaceElement& x, unsigned i);
    friend const AbstractFunctionSpaceElement& dualVariable(const AbstractFunctionSpaceElement& x, unsigned i);

    std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > primalVariables_;
    std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > dualVariables_;
    mutable bool disablePrimal_ = false;
    mutable bool disableDual_ = false;
  };

  bool isProductSpaceElement(const AbstractFunctionSpaceElement& x);


  FunctionSpaceElement& primal(FunctionSpaceElement& x);

  const FunctionSpaceElement& primal(const FunctionSpaceElement& x);

  FunctionSpaceElement& dual(FunctionSpaceElement& x);

  const FunctionSpaceElement& dual(const FunctionSpaceElement& x);
}

#endif // ALGORITHM_PRODUCT_SPACE_ELEMENT_HH
