#ifndef ALGORITHM_FUNCTION_SPACES_PRIMAL_DUAL_PRODUCTSPACE_HH
#define ALGORITHM_FUNCTION_SPACES_PRIMAL_DUAL_PRODUCTSPACE_HH

#include <memory>
#include <vector>

#include "Interface/abstractHilbertSpace.hh"
#include "FunctionSpaces/ProductSpace/productSpace.hh"

namespace Algorithm
{
  class AbstractNorm;
  class AbstractDualPairing;
  class AbstractFunctionSpaceElement;

  /// Space of real numbers.
  class PrimalDualProductSpace : public AbstractHilbertSpace
  {
  public:
    PrimalDualProductSpace(std::vector<std::unique_ptr<AbstractBanachSpace> >&& primalSpaces);

    PrimalDualProductSpace(std::vector<std::unique_ptr<AbstractBanachSpace> >&& primalSpaces,
                 std::vector<std::unique_ptr<AbstractBanachSpace> >&& dualSpaces);

    ProductSpace& getPrimalProductSpace();

    const ProductSpace& getPrimalProductSpace() const;

    ProductSpace& getDualProductSpace();

    const ProductSpace& getDualProductSpace() const;

  private:
    std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const override;

    ProductSpace primalSpaces_;
    ProductSpace dualSpaces_;
  };


  template <class PrimalSpaces, class DualSpaces=PackSpaces<> >
  std::unique_ptr<PrimalDualProductSpace> makePrimalDualProductSpace()
  {
    return std::make_unique<PrimalDualProductSpace>( ProductSpaceDetail::CreateSpaceVector<PrimalSpaces>::apply(),
                                                     ProductSpaceDetail::CreateSpaceVector<DualSpaces>::apply() );
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_PRIMAL_DUAL_PRODUCTSPACE_HH
