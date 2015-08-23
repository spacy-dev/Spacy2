#ifndef ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH
#define ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH

#include <map>
#include <memory>
#include <vector>

#include "../../vector.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace ProductSpace
  {
    class SpaceCreator
    {
    public:
      /**
       * @brief Construct product space \f$ X = \{ Y_0 , Y_1 , \ldots \} \f$.
       */
      explicit SpaceCreator(const std::vector<std::shared_ptr<VectorSpace> >& spaces);

      /**
       * @brief Construct product space \f$ X = \{ \{ Y_0 , Y_1 , \ldots \} , \{ Z_0 , \ldots \} \}\f$.
       *
       * \f$ \{Y_i\}_i \f$ are spaces associated with primal variables.
       * \f$ \{Z_i\}_i \f$ are spaces associated with dual variables.
       *
       * @param spaces shared spaces to build the product space from
       * @param primalSubSpaceIds indices of spaces associated with primal variables
       * @param dualSubSpaceIds indices of spaces associated with dual variables
       */
      SpaceCreator(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                   const std::vector<unsigned>& primalSubSpaceIds,
                   const std::vector<unsigned>& dualSubSpaceIds);

      /// Access vector of subspaces.
      const std::vector<std::shared_ptr<VectorSpace> >& subSpaces() const;

      /// Access i-th subspace.
      const VectorSpace& subSpace(unsigned i) const;

      /// Access pointer to i-th subspace.
      VectorSpace* subSpace_ptr(unsigned i) const;

      /// Access shared pointer to i-th subspace.
      std::shared_ptr<VectorSpace> sharedSubSpace(unsigned i) const;

      /// Access primal subspace as product space.
      VectorSpace& primalSubSpace();

      /// Access primal subspace as product space.
      const VectorSpace& primalSubSpace() const;

      /// Access pointer to primal subspace as product space.
      VectorSpace* primalSubSpace_ptr() const;

      /// Access shared pointer to primal subspace as product space.
      std::shared_ptr<VectorSpace> sharedPrimalSubSpace() const;

      /// Access dual subspace as product space.
      VectorSpace& dualSubSpace();

      /// Access dual subspace as product space.
      const VectorSpace& dualSubSpace() const;

      /// Access pointer to dual subspace as product space.
      VectorSpace* dualSubSpace_ptr() const;

      /// Access shared pointer to dual subspace as product space.
      std::shared_ptr<VectorSpace> sharedDualSubSpace() const;

      /// Access ids of primal subspaces.
      const std::vector<unsigned>& primalSubSpaceIds() const;

      /// Access ids of dual subspaces.
      const std::vector<unsigned>& dualSubSpaceIds() const;

      /// Checks whether the i-th subspace a primal space.
      bool isPrimalSubSpaceId(unsigned i) const;

      /// Checks whether the i-th subspace a dual space.
      bool isDualSubSpaceId(unsigned i) const;

      /**
       * @brief Maps global space index to local space index in the primal product space.
       * @param i global space index
       * @return local space index with respect to primal product space
       */
      unsigned primalIdMap(unsigned i) const;

      /**
       * @brief Maps global space index to local space index in the dual product space.
       * @param i global space index
       * @return local space index with respect to dual product space
       */
      unsigned dualIdMap(unsigned i) const;

      /**
       * @brief Checks whether this product space has primal-dual structure or is a simple product space.
       * @return true if this product space has primal-dual structure, else false
       */
      bool isPrimalDualProductSpace() const;

      /// Generate (default) product space element.
      ::Algorithm::Vector operator()(const VectorSpace* space) const;

    private:
      std::vector<std::shared_ptr<VectorSpace> > spaces_;
      std::vector<unsigned> primalSubSpaceIds_, dualSubSpaceIds_;
      std::map<unsigned,unsigned> primalMap_, dualMap_;
      bool isPrimalDualProductSpace_ = false;
    };


    /// \cond
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
    /// \endcond


    /**
     * @brief Create product space from spaces.
     *
     * @param spaces vector of spaces
     *
     * Creates a simple product space with only primal variables.
     */
    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces);

    /**
     * @brief Create product space from spaces.
     *
     * @param spaces vector of spaces
     * @param primalSubSpaceIds entries of spaces that correspond to primal variables
     * @param dualSubSpaceIds entries of spaces that correspond to dual variables
     */
    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                                 const std::vector<unsigned>& primalSubSpaceIds,
                                 const std::vector<unsigned>& dualSubSpaceIds = {});
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_PRODUCTSPACE_HH
