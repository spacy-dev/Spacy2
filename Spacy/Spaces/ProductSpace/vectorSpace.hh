#ifndef SPACY_SPACES_PRODUCT_SPACE_HH
#define SPACY_SPACES_PRODUCT_SPACE_HH

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

      void setIdMap(const std::map<unsigned,unsigned>& map);

      bool hasId(unsigned k) const;

    private:
      std::vector<std::shared_ptr<VectorSpace> > spaces_ = {};
      std::map<unsigned,unsigned> idMap_ = std::map<unsigned,unsigned>{};
    };

    /**
     * @ingroup ProductSpaceGroup
     * @brief Create product space.
     * @param spaces vector of sub-spaces
     */
    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces);
  }
}

#endif // SPACY_SPACES_PRODUCT_SPACE_HH
