#ifndef ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_ELEMENT_HH
#define ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_ELEMENT_HH

#include <memory>
#include <vector>

#include "Interface/abstractFunctionSpaceElement.hh"
#include "functionSpaceElement.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "Util/Mixins/impl.hh"
#include "Util/Mixins/primalDualSwitch.hh"

namespace Algorithm
{
  namespace Fenics
  {
    class ProductSpace;

    /// Real number.
    class ProductSpaceElement :
        public Interface::AbstractFunctionSpaceElement ,
        public Mixin::Impl<dolfin::Function> ,
        public Mixin::PrimalDualSwitch
    {
    public:
      /**
     * @brief Construct product space for Fenics product spaces.
     * @param v initial value
     * @param space associated function space (RealSpace)
     */
      ProductSpaceElement(const dolfin::Function& v, const Interface::AbstractBanachSpace& space);

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

      /// Number of entries in coefficient vector (=1).
      unsigned size() const final override;

      unsigned primalVariableSize() const;

      unsigned dualVariableSize() const;


      Interface::AbstractFunctionSpaceElement& primalVariable(unsigned i)
      {
        return primalProductSpaceElement_.variable(i);
      }

      const Interface::AbstractFunctionSpaceElement& primalVariable(unsigned i) const
      {
        return primalProductSpaceElement_.variable(i);
      }

      ProductSpaceElement& primalElement()
      {
        return primalProductSpaceElement_;
      }

      const ProductSpaceElement& primalElement() const
      {
        return primalProductSpaceElement_;
      }

      ProductSpaceElement& dualElement()
      {
        return dualProductSpaceElement_;
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
      /// Get a copy of this real number.
      ProductSpaceElement* cloneImpl() const final override;

      /// Apply as dual element.
      double applyAsDualTo(const Interface::AbstractFunctionSpaceElement& y) const final override;

      const ProductSpace&
    };

    bool isProductSpaceElement(const Interface::AbstractFunctionSpaceElement& x);

    ProductSpaceElement& toProductSpaceElement(Interface::AbstractFunctionSpaceElement& x);

    const ProductSpaceElement& toProductSpaceElement(const Interface::AbstractFunctionSpaceElement& x);

  }
}

#endif // ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_ELEMENT_HH
