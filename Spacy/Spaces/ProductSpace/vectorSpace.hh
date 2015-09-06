#ifndef SPACY_FUNCTION_SPACES_PRODUCTSPACE_HH
#define SPACY_FUNCTION_SPACES_PRODUCTSPACE_HH

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

    /**
     * @ingroup ProductSpaceGroup
     * @brief Creator for ProductSpace::Vector.
     *
     * Represents a product space \f$X = \{X_0,X_1,\ldots,X_n\}\f$. This structure may be further restricted by specifying if variables
     * act as "primal" or "dual" variables. Then we get
     * \f$X=\{ X_p, X_d \}\f$, where \f$X_p=\{X_i\}_i,\ i\in I \f$ is associated to the primal variables and \f$X_d=\{X_j\}_j,\ j\in J\f$
     * is associated to the dual variables.
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
       * @brief Construct product space \f$ X = \{ X_p , X_d \}\f$.
       *
       * \f$ \{X_i\}_{i\in I} \f$ are spaces associated with primal variables.
       * \f$ \{X_j\}_{j\in J} \f$ are spaces associated with dual variables.
       *
       * @param spaces shared spaces to build the product space from
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
      const VectorSpace& subSpace(unsigned k) const;

      /**
       * @brief Access shared pointer to the sub-space associated with the k-th variable.
       * @param k variable index
       * @return std::shared_ptr to the corresponding sub-space \f$X_k\f$
       */
      std::shared_ptr<VectorSpace> sharedSubSpace(unsigned k) const;

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

      /// Access shared pointer to primal subspace as product space.
      std::shared_ptr<VectorSpace> sharedPrimalSubSpace() const;

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
       * @brief Access shared pointer to dual sub-space as product space.
       * @return shared pointer to dual sub-space
       */
      std::shared_ptr<VectorSpace> sharedDualSubSpace() const;

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
       * @brief Checks whether this product space has primal-dual structure or is a simple product space.
       * @return true if this product space has primal-dual structure, else false
       */
      bool isPrimalDual() const;

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
      bool isPrimalDual_ = false;
    };

    /**
     * @ingroup ProductSpaceGroup
     * @brief Create product space from spaces.
     * @param spaces vector of spaces
     *
     * Creates a simple product space with only primal variables.
     */
    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces);

    /**
     * @ingroup ProductSpaceGroup
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

#endif // SPACY_FUNCTION_SPACES_PRODUCTSPACE_HH
