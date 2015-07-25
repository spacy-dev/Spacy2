#ifndef ALGORITHM_ADAPTER_FENICS_PRODUCT_SPACE_HH
#define ALGORITHM_ADAPTER_FENICS_PRODUCT_SPACE_HH

#include <memory>
#include <vector>

#include "Interface/abstractProductSpace.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractFunctionSpaceElement;
  }

  namespace Fenics
  {
    class ProductSpace : public Interface::AbstractProductSpace
    {
    public:
      ProductSpace(const dolfin::FunctionSpace& space,
                   const std::vector<size_t>& primalSpaceIds,
                   const std::vector<size_t>& dualSpaceIds_ = {},
                   const dolfin::Function& dummy);

      std::unique_ptr<AbstractBanachSpace> subSpace(unsigned i) const override;

      std::unique_ptr<AbstractProductSpace> primalSubSpace() const override;

      std::unique_ptr<AbstractProductSpace> dualSubSpace() const override;


    private:
      std::unique_ptr<Interface::AbstractFunctionSpaceElement> elementImpl() const override;

      dolfin::FunctionSpace space_;
      std::vector<size_t> primalSpaceIds_, dualSpaceIds_;
      dolfin::Function dummy_;
    };


    template <class PrimalSpaces, class DualSpaces=PackSpaces<> >
    std::shared_ptr<PrimalDualProductSpace> makePrimalDualProductSpace()
    {
      return std::make_shared<PrimalDualProductSpace>( ProductSpaceDetail::CreateSpaceVector<PrimalSpaces>::apply(),
                                                       ProductSpaceDetail::CreateSpaceVector<DualSpaces>::apply() );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_PRODUCT_SPACE_HH
