#ifndef SPACYS_ADAPTER_FENICS_VECTOR_SPACE_HH
#define SPACYS_ADAPTER_FENICS_VECTOR_SPACE_HH

#include <map>
#include <memory>

#include <dolfin.h>

#include "Spacy/vectorSpace.hh"
#include "Spacy/Util/Mixins/impl.hh"

namespace Spacy
{
  namespace FEniCS
  {
    /// @cond
    class Vector;
    /// @endcond

    /**
     * @ingroup FenicsGroup
     * @brief Creator for vector space elements for %FEniCS.
     * See @ref VectorCreatorAnchor "::Spacy::VectorCreator", @ref VectorCreatorConceptAnchor "::Spacy::VectorCreatorConcept".
     */
    class VectorCreator : public Mixin::Impl<dolfin::FunctionSpace>
    {
    public:
      /**
       * @brief Create from dolfin::FunctionSpace.
       * @param space single dolfin::FunctionSpace (no product space)
       */
      explicit VectorCreator(const dolfin::FunctionSpace& space);

      /**
       * @brief Create from dolfin::FunctionSpace.
       * @param space single dolfin::FunctionSpace (no product space)
       * @param dofmap map relating global ids of a product space to local ids in this single space
       */
      VectorCreator(const dolfin::FunctionSpace& space, const std::unordered_map<std::size_t,std::size_t>& dofmap);

      /**
       * @brief Map global id of a product space to local id in this single space.
       * @param i global id
       * @return local id
       */
      std::size_t dofmap(std::size_t i) const;

      /**
       * @brief Map local id this single space to the global id of a product space.
       * @param i local id
       * @return global id
       */
      std::size_t inverseDofmap(std::size_t i) const;

      /**
       * @brief Generate vector for FEniCS.
       * @param space pointer to vector space implementation
       * @return vector associated with space
       */
      ::Spacy::Vector operator()(const VectorSpace* space) const;

      /// Degrees of freedom.
      unsigned size() const;

    private:
      using map = std::unordered_map<std::size_t,std::size_t>;
      map dofmap_ = map{};
      // the following line does not compile, why?
      //std::unordered_map<std::size_t,std::size_t> dofmap_ = std::unordered_map<std::size_t,std::size_t>{};
      std::vector<std::size_t> inverseDofmap_ = {};
    };

    /**
     * @ingroup FenicsGroup
     * @brief Convenient generation of a single vector space from dolfin::FunctionSpace.
     * @param space single dolfin::FunctionSpace (no product space)
     * @return @ref ::Spacy::makeHilbertSpace() "::Spacy::makeHilbertSpace( VectorCreator{space} , l2Product{} )"
     */
    VectorSpace makeHilbertSpace(const dolfin::FunctionSpace& space);

    /**
     * @ingroup FenicsGroup
     * @brief Convenient generation of a single vector space from dolfin::FunctionSpace.
     * @param space single dolfin::FunctionSpace (no product space)
     * @param dofmap map relating global ids of a product space to local ids in this single space
     * @return @ref ::Spacy::makeHilbertSpace() "::Spacy::makeHilbertSpace( VectorCreator{space,dofmap} , l2Product{} )"
     */
    VectorSpace makeHilbertSpace(const dolfin::FunctionSpace& space, const std::unordered_map<std::size_t,std::size_t>& dofmap);

    /**
     * @ingroup FenicsGroup
     * @brief Convenient generation of a product space from dolfin::FunctionSpace.
     * @param space single dolfin::FunctionSpace (product space)
     * @param primalIds indices of spaces associated with primal variables
     * @param dualIds indices of spaces associated with dual variables
     * @return primal-dual product space
     */
    VectorSpace makeHilbertSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds = {});
  }
}

#endif // SPACYS_ADAPTER_FENICS_VECTOR_SPACE_HH
