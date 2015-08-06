#ifndef ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH
#define ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH

#include <memory>
#include <vector>

#include "hilbertSpace.hh"
#include "Interface/abstractHilbertSpace.hh"
#include "Util/create.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractNorm;
    class AbstractFunctionSpaceElement;
  }


  class ProductSpace : public Interface::AbstractHilbertSpace
  {
  public:
    explicit ProductSpace(const std::vector<std::shared_ptr<Interface::AbstractBanachSpace> >& spaces);

    ProductSpace(const std::vector<std::shared_ptr<Interface::AbstractBanachSpace> >& spaces,
                 const std::vector<unsigned>& primalSubSpaceIds,
                 const std::vector<unsigned>& dualSubSpaceIds);

    const std::vector<std::shared_ptr<Interface::AbstractBanachSpace> >& subSpaces() const;

    const Interface::AbstractBanachSpace& subSpace(unsigned i) const;

    std::shared_ptr<Interface::AbstractBanachSpace> sharedSubSpace(unsigned i) const;

    const ProductSpace& primalSubSpace() const;

    const ProductSpace& dualSubSpace() const;

    std::shared_ptr<ProductSpace> sharedPrimalSubSpace() const;

    std::shared_ptr<ProductSpace> sharedDualSubSpace() const;

    const std::vector<unsigned>& primalSubSpaceIds() const;

    const std::vector<unsigned>& dualSubSpaceIds() const;

    bool isPrimalSubSpaceId(unsigned i) const;

    bool isDualSubSpaceId(unsigned i) const;

  private:
    std::unique_ptr<Interface::AbstractFunctionSpaceElement> elementImpl() const override;

    std::vector<std::shared_ptr<Interface::AbstractBanachSpace> > spaces_;
    std::vector<unsigned> primalSubSpaceIds_, dualSubSpaceIds_;
    std::shared_ptr<ProductSpace> primalSpace_, dualSpace_;
  };


  template <class... Spaces_> struct PackSpaces {};

  namespace ProductSpaceDetail
  {
    template <class...> struct CreateSpaceVectorImpl;

    template <>
    struct CreateSpaceVectorImpl<>
    {
      static std::vector<std::shared_ptr<Interface::AbstractBanachSpace> > apply()
      {
        return std::vector<std::shared_ptr<Interface::AbstractBanachSpace> >();
      }
    };

    template <class Space, class... Spaces>
    struct CreateSpaceVectorImpl<Space,Spaces...>
    {
      static std::vector<std::shared_ptr<Interface::AbstractBanachSpace> > apply()
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


  HilbertSpace makeProductSpace(const std::vector<std::shared_ptr<Interface::AbstractBanachSpace> >& spaces);

  HilbertSpace makeProductSpace(const std::vector<std::shared_ptr<Interface::AbstractBanachSpace> >& spaces,
                                const std::vector<unsigned>& primalSubSpaceIds,
                                const std::vector<unsigned>& dualSubSpaceIds = {});
}

#endif // ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH
