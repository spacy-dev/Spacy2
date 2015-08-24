#ifndef ALGORITHM_ADAPTER_KASKADE_VECTOR_SPACE_HH
#define ALGORITHM_ADAPTER_KASKADE_VECTOR_SPACE_HH

#include <type_traits>
#include <memory>

#include "Util/Mixins/impl.hh"
#include "Util/cast.hh"

#include "l2Product.hh"
#include "vector.hh"

#include "VectorSpaces/productSpace.hh"

#include "../../vector.hh"
#include "../../vectorSpace.hh"


namespace Algorithm
{
  namespace Kaskade
  {
    template <class Description>
    class VectorCreator :
        public Mixin::Impl< std::decay_t< std::remove_pointer_t< std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Spaces,0>::type> > > >
    {
      using Space = std::decay_t< std::remove_pointer_t< std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Spaces,0>::type> > >;
    public:
      VectorCreator(const Space& space)
        : Mixin::Impl<Space>{space}
      {}

      ::Algorithm::Vector operator()(const ::Algorithm::VectorSpace* space) const
      {
        return Vector<Description>{*space};
      }
    };

    template <class Description, class Space>
    auto makeHilbertSpace(const Space& space)
    {
      return ::Algorithm::makeHilbertSpace( Kaskade::VectorCreator<Description>{space} , l2Product<Description>{} );
    }


    namespace Detail
    {
      template <class Description, int i, int n>
      struct MakeSpaces
      {
        using Spaces = typename Description::Spaces;
        using Variable = std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Variables,i>::type>;

        static void apply(const Spaces& spaces, std::vector<std::shared_ptr< ::Algorithm::VectorSpace > >& newSpaces)
        {
          newSpaces[i] = std::make_shared< ::Algorithm::VectorSpace >( ::Algorithm::makeHilbertSpace( VectorCreator< ExtractDescription_t<Description,i> >{ *boost::fusion::at_c<Variable::spaceIndex>(spaces) } , l2Product< ExtractDescription_t<Description,i> >{} ) );
          MakeSpaces<Description,i+1,n>::apply(spaces,newSpaces);
        }
      };

      template <class Description, int n>
      struct MakeSpaces<Description,n,n>
      {
        using Spaces = typename Description::Spaces;
        static void apply(const Spaces&, std::vector<std::shared_ptr< ::Algorithm::VectorSpace > >&)
        {}
      };

      template <class Description, unsigned i, unsigned j, unsigned n,
                bool doapply = ( i == std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Variables,j>::type>::spaceIndex ) >
      struct ExtractSpace
      {
        using Variables = typename Description::Variables;
        static auto apply(const ProductSpace::VectorCreator& spaces)
        {
          return &cast_ref< VectorCreator< ExtractDescription_t<Description,j> > >( spaces.subSpace(j).impl()).impl();
        }
      };

      template <class Description, unsigned i, unsigned j, unsigned n>
      struct ExtractSpace<Description,i,j,n,false>
      {
        static auto apply(const ProductSpace::VectorCreator& spaces)
        {
          return ExtractSpace<Description,i,j+1,n>::apply(spaces);
        }
      };

      template <class Description, unsigned i,  unsigned n, bool doApply>
      struct ExtractSpace<Description,i,n,n,doApply>
      {
        static auto apply(const ProductSpace::VectorCreator&)
        {}
      };


      template <class Description, unsigned i>
      auto extractSpace(const ProductSpace::VectorCreator& spaces)
      {
        return ExtractSpace<Description,i,0,boost::fusion::result_of::size<typename Description::Spaces>::value>::apply(spaces);
      }

      template <class Description, unsigned... is>
      auto extractSpaces(const ProductSpace::VectorCreator &spaces, std::integer_sequence<unsigned,is...>)
      {
        return typename Description::Spaces( extractSpace<Description,is>(spaces)... );
      }

      template <class Description, bool isProductSpace>
      struct ExtractSingleSpace
      {
        using Spaces = typename Description::Spaces;

        static Spaces apply(const  ::Algorithm::VectorSpace & spaces)
        {
          return Spaces{ &cast_ref< VectorCreator<Description> >(spaces.impl()).impl() };
        }
      };

      template <class Description>
      struct ExtractSingleSpace<Description,true>
      {
        using Spaces = typename Description::Spaces;

        static Spaces apply(const ::Algorithm::VectorSpace&)
        {
          return Spaces{};
        }
      };
    }


    /**
     * @brief Create product space with hilbert space structure for Kaskade7.s
     */
    template <class Description, class Spaces>
    auto makeHilbertSpace(const Spaces& spaces, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds = {})
    {
      constexpr int n = boost::fusion::result_of::size<typename Description::Variables>::value;
      std::vector<std::shared_ptr< ::Algorithm::VectorSpace > > newSpaces( n );

      Detail::MakeSpaces<Description,0,n>::apply(spaces,newSpaces);

      return ::Algorithm::ProductSpace::makeHilbertSpace( newSpaces , primalIds , dualIds );
    }

    template <class Description>
    auto extractSpaces(const ::Algorithm::VectorSpace& spaces)
    {
      using Spaces = typename Description::Spaces;

      if( isAny< VectorCreator<Description> >(spaces.impl()) )
        return Detail::ExtractSingleSpace<Description,Description::noOfVariables!=1>::apply(spaces);

      if( isAny<ProductSpace::VectorCreator>(spaces.impl()) )
      {
        const auto& spaces_ = cast_ref<ProductSpace::VectorCreator>(spaces.impl());
        using seq = std::make_integer_sequence<unsigned,boost::fusion::result_of::size<Spaces>::value>;
        return Detail::extractSpaces<Description>(spaces_,seq{});
      }

      assert(false);
    }
  }
}

#endif // ALGORITHM_ADAPTER_KASKADE_VECTOR_SPACE_HH
