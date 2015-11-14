#ifndef SPACY_SPACES_PRIMAL_DUAL_PRODUCT_SPACE_HH
#define SPACY_SPACES_PRIMAL_DUAL_PRODUCT_SPACE_HH

#include <map>
#include <memory>
#include <vector>

namespace Spacy
{
  /// @cond
  class VectorSpace;
  /// @endcond

  /** @addtogroup PrimalDualProductSpaceGroup
   * @{
   */
  namespace PrimalDualProductSpace
  {
    /// @cond
    class Vector;
    /// @endcond

    /**
     * @brief Creator for PrimalDualProductSpace::Vector.
     *
     * Represents a primal-dual product space \f$X = \{X_p,X_d\}\f$.
     */
    class VectorCreator
    {
    public:
      /// Construct product space \f$ X = \{ X_p , X_d \}\f$, where \f$X_p\f$ and \f$X_d\f$ are spaces corresponding to primal resp. dual variables.
      explicit VectorCreator(const std::vector<std::shared_ptr<VectorSpace> >& spaces);

      /**
       * @brief Construct product space \f$ X = \{ X_p , X_d \}\f$, where \f$X_p\f$ and \f$X_d\f$ are spaces corresponding to primal resp. dual variables.
       * @param primalSubSpaceIds indices \f$I\f$ of spaces \f$X_i\f$ associated with primal variables
       * @param dualSubSpaceIds indices \f$J\f$ of spaces \f$X_j\f$ associated with dual variables
       */
      VectorCreator(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                   const std::vector<unsigned>& primalSubSpaceIds,
                   const std::vector<unsigned>& dualSubSpaceIds);

      /**
       * @brief Access sub-spaces.
       *
       * If this space is a primal-dual product space then it has up to two sub-spaces which are itself
       * product space vectors.
       *
       * @return vector of sub-spaces.
       */
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
       * @brief Access primal subspace as product space.
       * @return \f$X_p\f$
       */
      VectorSpace& primalSubSpace();

      /**
       * @brief Access primal subspace as product space.
       * @return \f$X_p\f$
       */
      const VectorSpace& primalSubSpace() const;

      /**
       * @brief Access dual subspace as product space.
       * @return \f$X_d\f$
       */
      VectorSpace& dualSubSpace();

      /**
       * @brief Access dual subspace as product space.
       * @return \f$X_d\f$
       */
      const VectorSpace& dualSubSpace() const;

      /**
       * @brief Access ids of primal sub-spaces.
       * @return vector of ids of primal sub-spaces.
       */
      const std::vector<unsigned>& primalSubSpaceIds() const;

      /**
       * @brief Access ids of dual sub-spaces.
       * @return vector of ids of dual sub-spaces.
       */
      const std::vector<unsigned>& dualSubSpaceIds() const;

      /**
       * @brief Checks whether the k-th sub-space is a primal space.
       * @param k variable index
       * @return true if k-th subpsace is primal, else false
       */
      bool isPrimalSubSpaceId(unsigned k) const;

      /**
       * @brief Checks whether the k-th sub-space is a dual space.
       * @param k variable index
       * @return true if k-th subpsace is dual, else false
       */
      bool isDualSubSpaceId(unsigned k) const;

      /**
       * @brief Maps global space index to local space index in the primal product space.
       * @param k global space index
       * @return local space index with respect to primal product space
       */
      unsigned primalIdMap(unsigned k) const;

      /**
       * @brief Maps global space index to local space index in the dual product space.
       * @param k global space index
       * @return local space index with respect to dual product space
       */
      unsigned dualIdMap(unsigned k) const;

      /**
       * @brief Generate product space vector
       * @param space pointer to vector space implementation
       * @return vector associated with space
       */
      Vector operator()(const VectorSpace* space) const;

    private:
      std::vector<std::shared_ptr<VectorSpace> > spaces_ = {};
      std::vector<unsigned> primalSubSpaceIds_ = {}, dualSubSpaceIds_ = {};
      std::map<unsigned,unsigned> primalMap_ = std::map<unsigned,unsigned>{}, dualMap_ = std::map<unsigned,unsigned>{};
    };

    /**
     * @brief Create product space from spaces.
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
  /** @} */
}

#endif // SPACY_SPACES_PRIMAL_DUAL_PRODUCT_SPACE_HH
