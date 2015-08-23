#ifndef ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH
#define ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH

#include <map>
#include <memory>
#include <vector>

#include "vectorSpace.hh"

namespace Algorithm
{
  /// \cond
  namespace Interface
  {
    class AbstractNorm;
    class AbstractVector;
  }
  /// \endcond


  class ProductSpace
  {
  public:
    /**
     * @brief Construct product space \f$ X = \{ Y_0 , Y_1 , \ldots \} \f$.
     */
    explicit ProductSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces);

    /**
     * @brief Construct product space \f$ X = \{ \{ Y_0 , Y_1 , \ldots \} , \{ Z_0 , \ldots \} \}\f$.
     *
     * \f$ \{Y_i\}_i \f$ are spaces associated with primal variables.
     * \f$ \{Z_i\}_i \f$ are spaces associated with dual variables.
     *
     * @param spaces spaces to build the product space from
     * @param primalSubSpaceIds indices of spaces associated with primal variables
     * @param dualSubSpaceIds indices of spaces associated with dual variables
     */
    ProductSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                 const std::vector<unsigned>& primalSubSpaceIds,
                 const std::vector<unsigned>& dualSubSpaceIds);

    std::vector<std::shared_ptr<VectorSpace> > subSpaces() const;

    const VectorSpace& subSpace(unsigned i) const;

    std::shared_ptr<VectorSpace> sharedSubSpace(unsigned i) const;

    VectorSpace& primalSubSpace();

    const VectorSpace& primalSubSpace() const;

    VectorSpace& dualSubSpace();

    const VectorSpace& dualSubSpace() const;

    std::shared_ptr<VectorSpace> sharedPrimalSubSpace() const;

    std::shared_ptr<VectorSpace> sharedDualSubSpace() const;

    const std::vector<unsigned>& primalSubSpaceIds() const;

    const std::vector<unsigned>& dualSubSpaceIds() const;

    bool isPrimalSubSpaceId(unsigned i) const;

    bool isDualSubSpaceId(unsigned i) const;

    unsigned primalIdMap(unsigned i) const;

    unsigned dualIdMap(unsigned i) const;

    bool isPrimalDualProductSpace() const;

    Vector element(const VectorSpace* space) const;

  private:

    std::vector<std::shared_ptr<VectorSpace> > spaces_;
    std::vector<unsigned> primalSubSpaceIds_, dualSubSpaceIds_;
    std::map<unsigned,unsigned> primalMap_, dualMap_;
    bool isPrimalDualProductSpace_ = false;
  };


  template <class... Spaces_> struct PackSpaces {};

  namespace ProductSpaceDetail
  {
    template <class...> struct CreateSpaceVectorImpl;

    template <>
    struct CreateSpaceVectorImpl<>
    {
      static std::vector<std::shared_ptr<VectorSpace> > apply()
      {
        return std::vector<std::shared_ptr<VectorSpace> >();
      }
    };

    template <class Space, class... Spaces>
    struct CreateSpaceVectorImpl<Space,Spaces...>
    {
      static std::vector<std::shared_ptr<VectorSpace> > apply()
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


  VectorSpace makeProductSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces);

  VectorSpace makeProductSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                               const std::vector<unsigned>& primalSubSpaceIds,
                               const std::vector<unsigned>& dualSubSpaceIds = {});
}

#endif // ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH
