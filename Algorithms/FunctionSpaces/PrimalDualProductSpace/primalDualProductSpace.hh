#ifndef ALGORITHM_FUNCTION_SPACES_PRIMAL_DUAL_PRODUCTSPACE_HH
#define ALGORITHM_FUNCTION_SPACES_PRIMAL_DUAL_PRODUCTSPACE_HH

#include <memory>
#include <vector>

#include "Interface/abstractHilbertSpace.hh"
#include "FunctionSpaces/ProductSpace/productSpace.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractFunctionSpaceElement;
   }

  /// Space of real numbers.
  class PrimalDualProductSpace : public Interface::AbstractHilbertSpace
  {
  public:
    PrimalDualProductSpace(std::vector<std::shared_ptr<Interface::AbstractBanachSpace> >&& primalSpaces);

    PrimalDualProductSpace(std::vector<std::shared_ptr<Interface::AbstractBanachSpace> >&& primalSpaces,
                 std::vector<std::shared_ptr<Interface::AbstractBanachSpace> >&& dualSpaces);

    ProductSpace& getPrimalProductSpace();

    const ProductSpace& getPrimalProductSpace() const;

    std::shared_ptr<Interface::AbstractBanachSpace> getSharedPrimalProductSpace() const;

    ProductSpace& getDualProductSpace();

    const ProductSpace& getDualProductSpace() const;

    std::shared_ptr<Interface::AbstractBanachSpace> getSharedDualProductSpace() const;

  private:
    std::unique_ptr<Interface::AbstractFunctionSpaceElement> elementImpl() const override;

    std::shared_ptr<ProductSpace> primalSpaces_;
    std::shared_ptr<ProductSpace> dualSpaces_;
  };


  template <class PrimalSpaces, class DualSpaces=PackSpaces<> >
  std::shared_ptr<PrimalDualProductSpace> makePrimalDualProductSpace()
  {
    return std::make_shared<PrimalDualProductSpace>( ProductSpaceDetail::CreateSpaceVector<PrimalSpaces>::apply(),
                                                     ProductSpaceDetail::CreateSpaceVector<DualSpaces>::apply() );
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_PRIMAL_DUAL_PRODUCTSPACE_HH
