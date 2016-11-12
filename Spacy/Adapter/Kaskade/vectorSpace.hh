#pragma once

#include <type_traits>

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/productSpace.hh>
#include <Spacy/Util/Mixins/get.hh>

#include "l2Product.hh"
#include "vector.hh"
#include "util.hh"

namespace Spacy
{
  /** @addtogroup KaskadeGroup @{ */
  namespace Kaskade
  {
    /// Creator for vector space elements for %Kaskade 7
    template <class Description>
    class VectorCreator :
        public Mixin::Get<Description>
    {
    public:
      /**
       * @ingroup VectorSpaceGroup
       * @brief Create from %Kaskade 7 function space.
       * @param space single %Kaskade 7 function space (no product space)
       */
      template <class... Args,
                class = std::enable_if_t<std::is_constructible<Description,Args...>::value> >
      VectorCreator(Args&&... args)
        : Mixin::Get<Description>( std::forward<Args>(args)... )
      {}

      /// Generate vector for %Kaskade 7.
      Vector<Description> operator()(const VectorSpace* space) const
      {
        return Vector<Description>{*space};
      }
    };

    /**
     * @ingroup VectorSpaceGroup
     * @brief Create single space with hilbert space structure for %Kaskade 7.
     * @param space single %Kaskade 7 function space (no product space)
     */
    template <class Description>
    auto makeHilbertSpace(const Description& description)
    {
      return ::Spacy::makeHilbertSpace( Kaskade::VectorCreator<Description>{description} , l2Product<Description>{} );
    }

    /**
     * @ingroup VectorSpaceGroup
     * @brief Create product space with hilbert space structure for %Kaskade 7.
     * @param spaces boost fusion forward sequence of const pointers to %Kaskade 7 function spaces
     * @param primalIds ids of primal variables
     * @param dualIds ids of dual variables
     */
    template <class Description>
    auto makeHilbertSpace(const Description& description, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds = {})
    {
      constexpr int n = boost::fusion::result_of::size<typename Description::Variables>::value;
      std::vector<std::shared_ptr< VectorSpace > > newSpaces( n );

      std::cout << "create space with " << n << " variables." << std::endl;
      Detail::MakeSpaces<Description,0,n>::apply(description,newSpaces);

      if( dualIds.size() > 0)
        return ::Spacy::ProductSpace::makeHilbertSpace( newSpaces , primalIds , dualIds );

      return ::Spacy::ProductSpace::makeHilbertSpace( newSpaces , primalIds );
    }
  }
  /** @} */
}
