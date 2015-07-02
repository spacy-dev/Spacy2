#ifndef ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH
#define ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH

#include <memory>
#include <vector>

#include "Interface/abstractHilbertSpace.hh"

namespace Algorithm
{
  class AbstractNorm;
  class AbstractScalarProduct;
  class AbstractFunctionSpaceElement;

  /// Space of real numbers.
  class ProductSpace : public AbstractHilbertSpace
  {
  public:
    ProductSpace(const std::vector<std::shared_ptr<AbstractBanachSpace> >& spaces);

    const std::vector<std::shared_ptr<AbstractBanachSpace> >& getSpaces() const;

  private:
    std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const override;

    std::vector<std::shared_ptr<AbstractBanachSpace> > spaces_;
  };

  template <class... Spaces_> struct PackSpaces {};

  namespace ProductSpaceDetail
  {
    template <class...> struct CreateSpaceVectorImpl;

    template <>
    struct CreateSpaceVectorImpl<>
    {
      static std::vector<std::shared_ptr<AbstractBanachSpace> > apply()
      {
        return std::vector<std::shared_ptr<AbstractBanachSpace> >();
      }
    };

    template <class Space, class... Spaces>
    struct CreateSpaceVectorImpl<Space,Spaces...>
    {
      static std::vector<std::shared_ptr<AbstractBanachSpace> > apply()
      {
        auto spaces = CreateSpaceVectorImpl<Spaces...>::apply();
        spaces.push_back(std::make_shared<Space>());
        return spaces;
      }
    };

    template <class> struct CreateSpaceVector;

    template <class... Spaces_>
    struct CreateSpaceVector< PackSpaces<Spaces_...> > : CreateSpaceVectorImpl<Spaces_...> {};
  }


  template <class PrimalSpaces, class DualSpaces=PackSpaces<> >
  std::shared_ptr<ProductSpace> makeProductSpace()
  {
    return std::make_shared<ProductSpace>( ProductSpaceDetail::CreateSpaceVector<PrimalSpaces>::apply() );
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH
