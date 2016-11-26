#pragma once

#include <map>
#include <memory>
#include <vector>

namespace Spacy
{
  /// @cond
  class VectorSpace;
  /// @endcond

  namespace ProductSpace
  {
    /// @cond
    class Vector;
    /// @endcond

    /** @addtogroup ProductSpaceGroup
     * @{
     */

    /**
     * @brief Creator for ProductSpace::Vector.
     *
     * Represents a product space \f$X = \{X_0,X_1,\ldots,X_n\}\f$.
     */
    class VectorCreator
    {
    public:
      /**
       * @brief Construct product space \f$ X = \{ X_0 , X_1 , \ldots , X_n \} \f$.
       * @param spaces vector of spaces \f$\{X_k\}_k\f$
       */
      explicit VectorCreator(const std::vector<std::shared_ptr<VectorSpace> >& spaces);

      /**
       * @brief Construct product space \f$ X = \{ X_0 , X_1 , \ldots , X_n \} \f$.
       * @param spaces vector of spaces \f$\{X_k\}_k\f$
       * @param subSpaceIds vector of subspace indices
       */
      VectorCreator(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                    const std::vector<unsigned>& subSpaceIds);

      /// Access sub-spaces.
      const std::vector<std::shared_ptr<VectorSpace> >& subSpaces() const;

      /**
       * @brief Access the sub-space associated with the k-th variable.
       * @param k variable index
       * @return corresponding sub-space \f$X_k\f$
       */
      VectorSpace& subSpace(unsigned k);

      /**
       * @brief Access the sub-space associated with the k-th variable.
       * @param k variable index
       * @return corresponding sub-space \f$X_k\f$
       */
      const VectorSpace& subSpace(unsigned k) const;

      /**
       * @brief Generate product space vector
       * @param space pointer to vector space implementation
       * @return vector associated with space
       */
      Vector operator()(const VectorSpace* space) const;

      /**
       * @brief Maps global space index to local space index in the product space.
       * @param k global space index
       * @return local space index with respect to product space
       */
      unsigned idMap(unsigned k) const;

      /**
       * @brief Maps local space index to global space index.
       * @param k local space index with respect to product space
       * @return global space index
       */
      unsigned inverseIdMap(unsigned k) const;


      /// Check if product space contains a subspace with global index k.
      bool hasId(unsigned k) const;

    private:
      std::vector<std::shared_ptr<VectorSpace> > spaces_ = {};
      std::map<unsigned,unsigned> idMap_ = std::map<unsigned,unsigned>{};
    };

    /**
     * @brief Create product space.
     * @param spaces vector of sub-spaces
     */
    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces);

    /**
     * @brief Create product space.
     * @param spaces vector of sub-spaces
     * @param idMap map relating global space indices with the local indices \f$1,\ldots,n\f$
     */
    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                                 const std::vector<unsigned>& globalIds);

    /**
     * @brief Create primal-dual product space.
     *
     * @param spaces vector of spaces
     * @param primalSubSpaceIds entries of spaces that correspond to primal variables
     * @param dualSubSpaceIds entries of spaces that correspond to dual variables
     */
    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                                 const std::vector<unsigned>& primalSubSpaceIds,
                                 const std::vector<unsigned>& dualSubSpaceIds);
    /** @} */
  }
}
