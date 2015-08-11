#ifndef ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH
#define ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH

#include <memory>
#include <vector>

#include "vectorSpace.hh"
#include "Interface/abstractVectorSpace.hh"
#include "Util/create.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractNorm;
    class AbstractVector;
  }


  class ProductSpace : public Interface::AbstractVectorSpace
  {
  public:
    explicit ProductSpace(const std::vector<std::shared_ptr<Interface::AbstractVectorSpace> >& spaces);

    ProductSpace(const std::vector<std::shared_ptr<Interface::AbstractVectorSpace> >& spaces,
                 const std::vector<unsigned>& primalSubSpaceIds,
                 const std::vector<unsigned>& dualSubSpaceIds);

    const std::vector<std::shared_ptr<Interface::AbstractVectorSpace> >& subSpaces() const;

    const Interface::AbstractVectorSpace& subSpace(unsigned i) const;

    std::shared_ptr<Interface::AbstractVectorSpace> sharedSubSpace(unsigned i) const;

    const ProductSpace& primalSubSpace() const;

    const ProductSpace& dualSubSpace() const;

    std::shared_ptr<ProductSpace> sharedPrimalSubSpace() const;

    std::shared_ptr<ProductSpace> sharedDualSubSpace() const;

    const std::vector<unsigned>& primalSubSpaceIds() const;

    const std::vector<unsigned>& dualSubSpaceIds() const;

    bool isPrimalSubSpaceId(unsigned i) const;

    bool isDualSubSpaceId(unsigned i) const;

  private:
    std::unique_ptr<Interface::AbstractVector> elementImpl() const override;

    std::vector<std::shared_ptr<Interface::AbstractVectorSpace> > spaces_;
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
      static std::vector<std::shared_ptr<Interface::AbstractVectorSpace> > apply()
      {
        return std::vector<std::shared_ptr<Interface::AbstractVectorSpace> >();
      }
    };

    template <class Space, class... Spaces>
    struct CreateSpaceVectorImpl<Space,Spaces...>
    {
      static std::vector<std::shared_ptr<Interface::AbstractVectorSpace> > apply()
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


  VectorSpace makeProductSpace(const std::vector<std::shared_ptr<Interface::AbstractVectorSpace> >& spaces);

  VectorSpace makeProductSpace(const std::vector<std::shared_ptr<Interface::AbstractVectorSpace> >& spaces,
                               const std::vector<unsigned>& primalSubSpaceIds,
                               const std::vector<unsigned>& dualSubSpaceIds = {});
}

#endif // ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH
