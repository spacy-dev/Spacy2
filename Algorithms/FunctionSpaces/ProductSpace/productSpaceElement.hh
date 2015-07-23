#ifndef ALGORITHM_PRODUCT_SPACE_ELEMENT_HH
#define ALGORITHM_PRODUCT_SPACE_ELEMENT_HH

#include <memory>
#include <vector>

#include "productSpace.hh"

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/Mixins/primalDualSwitch.hh"

namespace Algorithm
{
  class ProductSpace;

  std::vector<std::unique_ptr<Interface::AbstractFunctionSpaceElement> > cloneVariables(const std::vector<std::unique_ptr<Interface::AbstractFunctionSpaceElement> >& variables);

  /// Real number.
  class ProductSpaceElement : public Interface::AbstractFunctionSpaceElement , public Mixin::PrimalDualSwitch
  {
  public:
    /**
     * @brief Construct real number with value x.
     * @param x initial value
     * @param space associated function space (RealSpace)
     */
    ProductSpaceElement(const std::vector<std::unique_ptr<Interface::AbstractFunctionSpaceElement> >& variables, const ProductSpace& space);

    /**
     * @brief Construct real number with initial value 0.
     * @param space associated function space (RealSpace)
     */
    explicit ProductSpaceElement(const ProductSpace& space);

    void copyTo(Interface::AbstractFunctionSpaceElement &) const final override;

    /// Print to os.
    void print(std::ostream& os) const final override;

    /// Assignment.
    ProductSpaceElement& operator=(const Interface::AbstractFunctionSpaceElement& y) final override;

    /// In-place summation.
    ProductSpaceElement& operator+=(const Interface::AbstractFunctionSpaceElement& y) final override;

    /// In-place subtraction.
    ProductSpaceElement& operator-=(const Interface::AbstractFunctionSpaceElement& y) final override;

    /// In-place multiplication.
    ProductSpaceElement& operator*=(double) final override;

    /// Get -x.
    std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator- () const final override;

    /// Access value.
    double& coefficient(unsigned) final override;

    /// Access value.
    const double& coefficient(unsigned) const final override;

    /// Number of entries in coefficient vector.
    unsigned size() const final override;

    Interface::AbstractFunctionSpaceElement& variable(unsigned i);

    const Interface::AbstractFunctionSpaceElement& variable(unsigned i) const;

    std::vector<std::unique_ptr<Interface::AbstractFunctionSpaceElement> >& variables();

    const std::vector<std::unique_ptr<Interface::AbstractFunctionSpaceElement> >& variables() const;

    ProductSpaceElement primalElement() const;

    ProductSpaceElement dualElement() const;

    const ProductSpace& space() const;

  private:    
    /**
     * @brief Get a copy of this real number.
     */
    ProductSpaceElement* cloneImpl() const final override;

    /// Apply as dual element.
    double applyAsDualTo(const Interface::AbstractFunctionSpaceElement& y) const final override;

    std::vector<std::unique_ptr<Interface::AbstractFunctionSpaceElement> > variables_;

  };

  bool isProductSpaceElement(const Interface::AbstractFunctionSpaceElement& x);

  ProductSpaceElement& toProductSpaceElement(Interface::AbstractFunctionSpaceElement& x);

  const ProductSpaceElement& toProductSpaceElement(const Interface::AbstractFunctionSpaceElement& x);


  FunctionSpaceElement primalElement(const FunctionSpaceElement& x);
}

#endif // ALGORITHM_PRODUCT_SPACE_ELEMENT_HH
