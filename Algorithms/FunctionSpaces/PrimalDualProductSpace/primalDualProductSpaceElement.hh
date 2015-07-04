#ifndef ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_ELEMENT_HH
#define ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_ELEMENT_HH

#include <memory>
#include <vector>

#include "Interface/abstractFunctionSpaceElement.hh"
#include "functionSpaceElement.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

namespace Algorithm
{
  class PrimalDualProductSpace;

  /// Real number.
  class PrimalDualProductSpaceElement : public Interface::AbstractFunctionSpaceElement
  {
  public:
    /**
     * @brief Construct real number with value x.
     * @param x initial value
     * @param space associated function space (RealSpace)
     */
    PrimalDualProductSpaceElement(const std::vector<std::unique_ptr<Interface::AbstractFunctionSpaceElement> >& primalVariables, const Interface::AbstractBanachSpace& space);

    PrimalDualProductSpaceElement(const std::vector<std::unique_ptr<Interface::AbstractFunctionSpaceElement> >& primalVariables,
                        const std::vector<std::unique_ptr<Interface::AbstractFunctionSpaceElement> >& dualVariables, const Interface::AbstractBanachSpace& space);

    /**
     * @brief Construct real number with initial value 0.
     * @param space associated function space (RealSpace)
     */
    explicit PrimalDualProductSpaceElement(const PrimalDualProductSpace& space);

    /**
     * @brief Get a copy of this real number.
     */
    PrimalDualProductSpaceElement* cloneImpl() const final override;

    void copyTo(Interface::AbstractFunctionSpaceElement &) const final override;

    /// Print to os.
    void print(std::ostream& os) const final override;

    /// Assignment.
    PrimalDualProductSpaceElement& operator=(const Interface::AbstractFunctionSpaceElement& y) final override;

    /// In-place summation.
    PrimalDualProductSpaceElement& operator+=(const Interface::AbstractFunctionSpaceElement& y) final override;

    /// In-place subtraction.
    PrimalDualProductSpaceElement& operator-=(const Interface::AbstractFunctionSpaceElement& y) final override;

    /// In-place multiplication.
    PrimalDualProductSpaceElement& operator*=(double) final override;

    /// Get -x.
    std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator- () const final override;

    /// Access value.
    double& coefficient(unsigned) final override;

    /// Access value.
    const double& coefficient(unsigned) const final override;

    /// Number of entries in coefficient vector (=1).
    unsigned size() const final override;

    unsigned primalVariableSize() const;

    unsigned dualVariableSize() const;

    void reset() const;

    void disablePrimal() const;

    void disableDual() const;

    bool isPrimalEnabled() const;

    bool isDualEnabled() const;

//    FunctionSpaceElement primalElement() const
//    {
//      return FunctionSpaceElement( PrimalDualProductSpaceElement(primalVariables_,{},this->getSpace()).clone() );
//    }


    Interface::AbstractFunctionSpaceElement& primalVariable(unsigned i)
    {
      return primalProductSpaceElement_.variable(i);
    }

    const Interface::AbstractFunctionSpaceElement& primalVariable(unsigned i) const
    {
      return primalProductSpaceElement_.variable(i);
    }

//    FunctionSpaceElement dualElement() const
//    {
//      return FunctionSpaceElement( PrimalDualProductSpaceElement({},dualVariables_,this->getSpace()).clone() ) ;
//    }

    const ProductSpaceElement& primalElement() const
    {
      return primalProductSpaceElement_;
    }

    const ProductSpaceElement& dualElement() const
    {
      return dualProductSpaceElement_;
    }

    Interface::AbstractFunctionSpaceElement& dualVariable(unsigned i)
    {
      return dualProductSpaceElement_.variable(i);
    }

    const Interface::AbstractFunctionSpaceElement& dualVariable(unsigned i) const
    {
      return dualProductSpaceElement_.variable(i);
    }

  private:
    /// Apply as dual element.
    double applyAsDualTo(const Interface::AbstractFunctionSpaceElement& y) const final override;

    ProductSpaceElement primalProductSpaceElement_;
    ProductSpaceElement dualProductSpaceElement_;
    mutable bool disablePrimal_ = false;
    mutable bool disableDual_ = false;
  };

  bool isPrimalDualProductSpaceElement(const Interface::AbstractFunctionSpaceElement& x);


  FunctionSpaceElement& primal(FunctionSpaceElement& x);

  const FunctionSpaceElement& primal(const FunctionSpaceElement& x);

  FunctionSpaceElement& dual(FunctionSpaceElement& x);

  const FunctionSpaceElement& dual(const FunctionSpaceElement& x);
}

#endif // ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_ELEMENT_HH
