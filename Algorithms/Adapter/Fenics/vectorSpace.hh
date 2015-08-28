#ifndef ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH
#define ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH

#include <map>
#include <memory>

#include <dolfin.h>

#include "Util/Mixins/impl.hh"
#include "VectorSpaces/ProductSpace/vectorSpace.hh"

namespace Algorithm
{
  namespace Fenics
  {
    /**
     * @ingroup FenicsGroup
     * @brief Creator for vector space elements for FEniCS.
     * See @ref VectorCreatorAnchor "::Algorithm::VectorCreator", VectorCreatorConceptAnchor "::Algorithm::VectorCreatorConcept".
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
      VectorCreator(const dolfin::FunctionSpace& space, const std::unordered_map<size_t,size_t>& dofmap);

      /**
       * @brief Map global id of a product space to local id in this single space.
       * @param i global id
       * @return local id
       */
      size_t dofmap(size_t i) const;

      /**
       * @brief Map local id this single space to the global id of a product space.
       * @param i local id
       * @return global id
       */
      size_t inverseDofmap(size_t i) const;

      /// Generate vector for FEniCS.
      ::Algorithm::Vector operator()(const VectorSpace* space) const;

    private:
      std::unordered_map<size_t,size_t> dofmap_;
      std::vector<size_t> inverseDofmap_;
    };

    /**
     * @ingroup FenicsGroup
     * @brief Convenient generation of a single vector space from dolfin::FunctionSpace.
     * @param space single dolfin::FunctionSpace (no product space)
     * @return ::Algorithm::makeHilbertSpace( VectorCreator{space} , l2Product{} )
     */
    VectorSpace makeHilbertSpace(const dolfin::FunctionSpace& space);

    /**
     * @ingroup FenicsGroup
     * @brief Convenient generation of a single vector space from dolfin::FunctionSpace.
     * @param space single dolfin::FunctionSpace (no product space)
     * @param dofmap map relating global ids of a product space to local ids in this single space
     * @return ::Algorithm::makeHilbertSpace( VectorCreator{space,dofmap} , l2Product{} )
     */
    VectorSpace makeHilbertSpace(const dolfin::FunctionSpace& space, const std::unordered_map<size_t,size_t>& dofmap);

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

#endif // ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH
