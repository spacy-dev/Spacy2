#ifndef ALGORITHM_ADAPTER_KASKADE_HILBERT_SPACE_HH
#define ALGORITHM_ADAPTER_KASKADE_HILBERT_SPACE_HH

#include <memory>

#include "Interface/abstractHilbertSpace.hh"
#include "Util/Mixins/impl.hh"
#include "Util/create.hh"

#include "l2Product.hh"
#include "vector.hh"

#include "../../hilbertSpace.hh"

namespace Algorithm
{
  /// \cond
  namespace Interface { class AbstractFunctionSpaceElement; }
  /// \endcond

  namespace Kaskade
  {
    template <class Description>
    class HilbertSpace :
        public Interface::AbstractHilbertSpace ,
        public Mixin::Impl< std::decay_t< std::remove_pointer_t< std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Spaces,0>::type> > > >
    {
      using Space = std::decay_t< std::remove_pointer_t< std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Spaces,0>::type> > >;
      using VectorImpl = typename Description::template CoefficientVectorRepresentation<>::type;
    public:
      HilbertSpace(const Space& space)
        : Interface::AbstractHilbertSpace( std::make_shared< l2Product<Description> >() ),
          Mixin::Impl<Space>(space)
      {}

    private:
      std::unique_ptr<Interface::AbstractFunctionSpaceElement> elementImpl() const override
      {
        return std::make_unique< Vector<Description> >(*this);
      }
    };

    template <class Description, class Space>
    auto makeHilbertSpace(const Space& space)
    {
      return createFromSharedImpl< ::Algorithm::HilbertSpace , HilbertSpace<Description> >( space );
    }


    namespace Detail
    {
      template <class Description, int i, int n>
      struct MakeSpaces
      {
        using Spaces = typename Description::Spaces;
        using Variable = std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Variables,i>::type>;

        static void apply(const Spaces& spaces, std::vector<std::shared_ptr<Interface::AbstractBanachSpace> >& newSpaces)
        {
          newSpaces[i] = std::make_shared< HilbertSpace<ExtractDescription_t<Description,i> > >( *boost::fusion::at_c<Variable::spaceIndex>(spaces) );
          MakeSpaces<Description,i+1,n>::apply(spaces,newSpaces);
        }
      };

      template <class Description, int n>
      struct MakeSpaces<Description,n,n>
      {
        using Spaces = typename Description::Spaces;
        static void apply(const Spaces&, std::vector<std::shared_ptr<Interface::AbstractBanachSpace> >&)
        {}
      };

      template <class Description, unsigned i, unsigned j, unsigned n,
                bool doapply = ( i == std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Variables,j>::type>::spaceIndex ) >
      struct ExtractSpace
      {
        using Variables = typename Description::Variables;
        static auto apply(const ProductSpace& spaces)
        {
          return &castTo< HilbertSpace< ExtractDescription_t<Description,j> > >( spaces.subSpace(j)).impl();
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
//        std::cout << "extracting spaces: " << castTo< HilbertSpace< ExtractDescription_t<Description,i> > >( spaces.subSpace(i)).impl().degreesOfFreedom() << std::endl;
//        return &castTo< HilbertSpace< ExtractDescription_t<Description,i> > >( spaces.subSpace(i)).impl();
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

        static Spaces apply(const Interface::AbstractBanachSpace& spaces)
        {
          return Spaces( &castTo< HilbertSpace<Description> >(spaces).impl() );
        }
      };

      template <class Description>
      struct ExtractSingleSpace<Description,true>
      {
        using Spaces = typename Description::Spaces;

        static Spaces apply(const Interface::AbstractBanachSpace&)
        {}
      };
    }


    template <class Description, class Spaces>
    auto makeProductSpace(const Spaces& spaces, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds)
    {
      constexpr int n = boost::fusion::result_of::size<typename Description::Variables>::value;
      std::vector<std::shared_ptr<Interface::AbstractBanachSpace> > newSpaces( n );

      Detail::MakeSpaces<Description,0,n>::apply(spaces,newSpaces);

      return createFromSharedImpl< ::Algorithm::HilbertSpace, ProductSpace>( newSpaces , primalIds , dualIds );
    }

    template <class Description>
    auto extractSpaces(const Interface::AbstractBanachSpace& spaces)
    {
      using Spaces = typename Description::Spaces;

      if( is< HilbertSpace< Description > >(spaces) )
        return Detail::ExtractSingleSpace<Description,Description::noOfVariables!=1>::apply(spaces);

      if( is< ProductSpace >(spaces) )
      {
        const auto& spaces_ = castTo<ProductSpace>(spaces);
        using seq = std::make_integer_sequence<unsigned,boost::fusion::result_of::size<Spaces>::value>;
        return Detail::extractSpaces<Description>(spaces_,seq{});
      }

      assert(false);
    }
  }
}

#endif // ALGORITHM_ADAPTER_KASKADE_HILBERT_SPACE_HH
