#ifndef ALGORITHM_FUNCTION_SPACES_PRIMAL_DUAL_PRODUCTSPACE_HH
#define ALGORITHM_FUNCTION_SPACES_PRIMAL_DUAL_PRODUCTSPACE_HH

#include <memory>
#include <vector>

#include "Interface/abstractHilbertSpace.hh"
#include "FunctionSpaces/ProductSpace/productSpace.hh"

namespace Algorithm
{
  class AbstractNorm;
  class AbstractScalarProduct;
  class AbstractFunctionSpaceElement;

  /// Space of real numbers.
  class PrimalDualProductSpace : public AbstractHilbertSpace
  {
  public:
    PrimalDualProductSpace(std::vector<std::shared_ptr<AbstractBanachSpace> >&& primalSpaces);

    PrimalDualProductSpace(std::vector<std::shared_ptr<AbstractBanachSpace> >&& primalSpaces,
                 std::vector<std::shared_ptr<AbstractBanachSpace> >&& dualSpaces);

    ProductSpace& getPrimalProductSpace();

    const ProductSpace& getPrimalProductSpace() const;

    std::shared_ptr<AbstractBanachSpace> getSharedPrimalProductSpace() const;

    ProductSpace& getDualProductSpace();

    const ProductSpace& getDualProductSpace() const;

    std::shared_ptr<AbstractBanachSpace> getSharedDualProductSpace() const;

  private:
    std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const override;

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
