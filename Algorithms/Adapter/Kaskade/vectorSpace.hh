#ifndef ALGORITHM_ADAPTER_KASKADE_VECTOR_SPACE_HH
#define ALGORITHM_ADAPTER_KASKADE_VECTOR_SPACE_HH

#include <memory>

#include "Util/Mixins/impl.hh"
#include "Util/create.hh"
#include "Util/castTo.hh"

#include "l2Product.hh"
#include "vector.hh"

#include "FunctionSpaces/ProductSpace/productSpace.hh"
#include "FunctionSpaces/ProductSpace/productSpaceProduct.hh"

#include "../../vector.hh"
#include "../../vectorSpace.hh"


namespace Algorithm
{
  namespace Kaskade
  {
    template <class Description>
    class VectorSpace :
        public Mixin::Impl< std::decay_t< std::remove_pointer_t< std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Spaces,0>::type> > > >
    {
      using Space = std::decay_t< std::remove_pointer_t< std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Spaces,0>::type> > >;
    public:
      VectorSpace(const Space& space)
        : Mixin::Impl<Space>(space)
      {}

      ::Algorithm::Vector element(const ::Algorithm::VectorSpace* space) const
      {
        return Vector<Description>{*space};
      }
    };

    template <class Description, class Space>
    auto makeVectorSpace(const Space& space)
    {
      return ::Algorithm::VectorSpace( Kaskade::VectorSpace<Description>(space) , l2Product<Description>() );
//      return createFromSharedImpl< ::Algorithm::VectorSpace , VectorSpace<Description> >( space );
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
          newSpaces[i] = std::make_shared< ::Algorithm::VectorSpace >( VectorSpace< ExtractDescription_t<Description,i> >( *boost::fusion::at_c<Variable::spaceIndex>(spaces) ) , l2Product< ExtractDescription_t<Description,i> >() );
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
        static auto apply(const ProductSpace& spaces)
        {
          return &castAny< VectorSpace< ExtractDescription_t<Description,j> > >( spaces.subSpace(j).impl()).impl();
        }
      };

      template <class Description, unsigned i, unsigned j, unsigned n>
      struct ExtractSpace<Description,i,j,n,false>
      {
        static auto apply(const ProductSpace& spaces)
        {
          return ExtractSpace<Description,i,j+1,n>::apply(spaces);
        }
      };

      template <class Description, unsigned i,  unsigned n, bool doApply>
      struct ExtractSpace<Description,i,n,n,doApply>
      {
        static auto apply(const ProductSpace&)
        {}
      };


      template <class Description, unsigned i>
      auto extractSpace(const ProductSpace& spaces)
      {
        return ExtractSpace<Description,i,0,boost::fusion::result_of::size<typename Description::Spaces>::value>::apply(spaces);
//        constexpr int j = std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Variables,i>::type>::spaceIndex;
//        std::cout << "extracting spaces: " << castTo< VectorSpace< ExtractDescription_t<Description,i> > >( spaces.subSpace(i)).impl().degreesOfFreedom() << std::endl;
//        return &castTo< VectorSpace< ExtractDescription_t<Description,i> > >( spaces.subSpace(i)).impl();
      }

      template <class Description, unsigned... is>
      auto extractSpaces(const ProductSpace &spaces, std::integer_sequence<unsigned,is...>)
      {
        return typename Description::Spaces( extractSpace<Description,is>(spaces)... );
      }

      template <class Description, bool isProductSpace>
      struct ExtractSingleSpace
      {
        using Spaces = typename Description::Spaces;

        static Spaces apply(const  ::Algorithm::VectorSpace & spaces)
        {
          return Spaces( &castAny< VectorSpace<Description> >(spaces.impl()).impl() );
//          return Spaces( &castTo< VectorSpace<Description> >(spaces).impl() );
        }
      };

      template <class Description>
      struct ExtractSingleSpace<Description,true>
      {
        using Spaces = typename Description::Spaces;

        static Spaces apply(const ::Algorithm::VectorSpace&)
        {}
      };
    }


    template <class Description, class Spaces>
    auto makeProductSpace(const Spaces& spaces, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds)
    {
      constexpr int n = boost::fusion::result_of::size<typename Description::Variables>::value;
      std::vector<std::shared_ptr< ::Algorithm::VectorSpace > > newSpaces( n );

      Detail::MakeSpaces<Description,0,n>::apply(spaces,newSpaces);

      return ::Algorithm::VectorSpace( ProductSpace( newSpaces , primalIds , dualIds ) , ProductSpaceProduct() );
    }

    template <class Description>
    auto extractSpaces(const ::Algorithm::VectorSpace& spaces)
    {
      using Spaces = typename Description::Spaces;

//      if( is< VectorSpace< Description > >(spaces) )
      if( isAny< VectorSpace<Description> >(spaces.impl()) )
        return Detail::ExtractSingleSpace<Description,Description::noOfVariables!=1>::apply(spaces);

      if( isAny<ProductSpace>(spaces.impl()) )
//      if( is< ProductSpace >(spaces) )
      {
        const auto& spaces_ = boost::type_erasure::any_cast<const ProductSpace&>(spaces.impl());
        using seq = std::make_integer_sequence<unsigned,boost::fusion::result_of::size<Spaces>::value>;
        return Detail::extractSpaces<Description>(spaces_,seq{});
      }

      assert(false);
    }
  }
}

#endif // ALGORITHM_ADAPTER_KASKADE_VECTOR_SPACE_HH
