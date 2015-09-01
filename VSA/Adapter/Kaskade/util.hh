#ifndef ALGORITHM_KASKADE_UTIL_HH
#define ALGORITHM_KASKADE_UTIL_HH

#include <memory>
#include <type_traits>
#include <vector>

#include "fem/variables.hh"

#include "VSA/vectorSpace.hh"
#include "VSA/Spaces/productSpace.hh"
#include "l2Product.hh"
#include "vector.hh"

namespace VSA
{
  namespace Kaskade
  {
    /// \cond
    template <class> class VectorCreator;

    namespace Detail
    {
      template <class Description, int i>
      struct ExtractDescription
      {
        using Variables = typename Description::Variables;
        using Spaces = typename Description::Spaces;
        using Variable = std::decay_t<typename boost::fusion::result_of::value_at_c<Variables,i>::type>;
        using SpacePtr = std::remove_reference_t<typename boost::fusion::result_of::value_at_c<Spaces,Variable::spaceIndex>::type>;
        using SpaceShiftedVariable = ::Kaskade::Variable< ::Kaskade::VariableId<Variable::id > , ::Kaskade::SpaceIndex<0> , ::Kaskade::Components<Variable::m> >;
        using type = ::Kaskade::VariableSetDescription< boost::fusion::vector< SpacePtr > , boost::fusion::vector< SpaceShiftedVariable > >;
      };

      template <class Description, int i>
      using ExtractDescription_t = typename ExtractDescription<Description,i>::type;

      template <class Description, int i, int n>
      struct MakeSpaces
      {
        using Spaces = typename Description::Spaces;
        using Variable = std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Variables,i>::type>;

        static void apply(const Spaces& spaces, std::vector<std::shared_ptr< VectorSpace> >& newSpaces)
        {
          newSpaces[i] = std::make_shared< VectorSpace>( ::VSA::makeHilbertSpace( VectorCreator< ExtractDescription_t<Description,i> >{ *boost::fusion::at_c<Variable::spaceIndex>(spaces) } , l2Product< ExtractDescription_t<Description,i> >{} ) );
          MakeSpaces<Description,i+1,n>::apply(spaces,newSpaces);
        }
      };

      template <class Description, int n>
      struct MakeSpaces<Description,n,n>
      {
        using Spaces = typename Description::Spaces;
        static void apply(const Spaces&, std::vector<std::shared_ptr< VectorSpace> >&)
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

      template <class Description, bool isProductSpace>
      struct ExtractSingleSpace
      {
        using Spaces = typename Description::Spaces;

        static Spaces apply(const  VectorSpace& spaces)
        {
          return Spaces{ &cast_ref< VectorCreator<Description> >(spaces.impl()).impl() };
        }
      };

      template <class Description>
      struct ExtractSingleSpace<Description,true>
      {
        using Spaces = typename Description::Spaces;

        static Spaces apply(const VectorSpace&)
        {
          return Spaces{};
        }
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


      template <int i, int n>
      struct Copy
      {
        template <class Description>
        static void apply(const ProductSpace::Vector& x, ::Kaskade::VariableSet<Description>& y)
        {
          if( ( x.creator().isPrimalSubSpaceId(i) && x.isPrimalEnabled() ) ||
              ( x.creator().isDualSubSpaceId(i) && x.isDualEnabled() ) )
            boost::fusion::at_c<i>(y.data).coefficients() = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(x.variable(i)).impl().data);
          Copy<i+1,n>::apply(x,y);
        }

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const ProductSpace::Vector& x, CoeffVector& y)
        {
          if( ( x.creator().isPrimalSubSpaceId(i) && x.isPrimalEnabled() ) ||
              ( x.creator().isDualSubSpaceId(i) && x.isDualEnabled() ) )
            boost::fusion::at_c<i>(y.data) = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(x.variable(i)).impl().data);
          Copy<i+1,n>::template toCoefficientVector<Description>(x,y);
        }

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector& x, ProductSpace::Vector& y)
        {
          if( ( y.creator().isPrimalSubSpaceId(i) && y.isPrimalEnabled() ) ||
              ( y.creator().isDualSubSpaceId(i) && y.isDualEnabled() ) )
            boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(y.variable(i)).impl().data) = boost::fusion::at_c<i>(x.data);
          Copy<i+1,n>::template fromCoefficientVector<Description>(x,y);
        }
      };

      template <int n>
      struct Copy<0,n>
      {
        template <class Description>
        static void apply(const ::VSA::Vector& x, ::Kaskade::VariableSet<Description>& y)
        {
          if( is< Vector< Description > >(x) )
          {
            boost::fusion::at_c<0>(y.data).coefficients() = boost::fusion::at_c<0>(cast_ref< Vector< Description > >(x).impl().data);
            return;
          }

          if( is<ProductSpace::Vector>(x))
          {
            const auto& x_ = cast_ref<ProductSpace::Vector>(x);
            if( ( x_.creator().isPrimalSubSpaceId(0) && x_.isPrimalEnabled() ) ||
                ( x_.creator().isDualSubSpaceId(0) && x_.isDualEnabled() ) )
            boost::fusion::at_c<0>(y.data).coefficients() = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,0> > >(x_.variable(0)).impl().data);
            Copy<1,n>::apply(x_,y);
            return;
          }

          assert(false);
        }

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const ::VSA::Vector& x, CoeffVector& y)
        {
          if( is< Vector< Description > >(x) )
          {
            boost::fusion::at_c<0>(y.data) = boost::fusion::at_c<0>(cast_ref< Vector< Description > >(x).impl().data);
            return;
          }

          if( is<ProductSpace::Vector>(x))
          {
            const auto& x_ = cast_ref<ProductSpace::Vector>(x);
            if( ( x_.creator().isPrimalSubSpaceId(0) && x_.isPrimalEnabled() ) ||
                ( x_.creator().isDualSubSpaceId(0) && x_.isDualEnabled() ) )
              boost::fusion::at_c<0>(y.data) = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,0> > >(x_.variable(0)).impl().data);
            Copy<1,n>::template toCoefficientVector<Description>(x_,y);
            return;
          }

          assert(false);
        }

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector& x, ::VSA::Vector& y)
        {
          if( is< Vector< Description > >(y) )
          {
            boost::fusion::at_c<0>(cast_ref< Vector< Description > >(y).impl().data) = boost::fusion::at_c<0>(x.data);
            return;
          }

          if( is<ProductSpace::Vector>(y))
          {
            auto& y_ = cast_ref<ProductSpace::Vector>(y);
            if( ( y_.creator().isPrimalSubSpaceId(0) && y_.isPrimalEnabled() ) ||
                ( y_.creator().isDualSubSpaceId(0) && y_.isDualEnabled() ) )
              boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,0> > >(y_.variable(0)).impl().data) = boost::fusion::at_c<0>(x.data);
            Copy<1,n>::template fromCoefficientVector<Description>(x,y_);
            return;
          }

          assert(false);
        }
      };


      template <int n>
      struct Copy<n,n>
      {
        template <class Description>
        static void apply(const ProductSpace::Vector&, ::Kaskade::VariableSet<Description>&)
        {}

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const ProductSpace::Vector&, CoeffVector&)
        {}

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector&, ProductSpace::Vector&)
        {}
      };
    }
    /// \endcond


    /**
     * @ingroup KaskadeGroup
     * @brief Extract boost::fusion::vector< const Space0*, const Space1*, ... > from spaces.
     */
    template <class Description>
    auto extractSpaces(const VectorSpace& spaces)
    {
      using Spaces = typename Description::Spaces;

      if( is< VectorCreator<Description> >(spaces.impl()) )
        return Detail::ExtractSingleSpace<Description,Description::noOfVariables!=1>::apply(spaces);

      if( is<ProductSpace::VectorCreator>(spaces.impl()) )
      {
        const auto& spaces_ = cast_ref<ProductSpace::VectorCreator>(spaces.impl());
        using seq = std::make_integer_sequence<unsigned,boost::fusion::result_of::size<Spaces>::value>;
        return Detail::extractSpaces<Description>(spaces_,seq{});
      }

      assert(false);
    }

    /**
     * @ingroup KaskadeGroup
     * @brief Copy from \ref VectorAnchor "::VSA::Vector" to %Kaskade::VariableSet<Description>.
     *
     * Takes into account product space structures.
     */
    template <class Description>
    void copy(const ::VSA::Vector& x, ::Kaskade::VariableSet<Description>& y)
    {
      Detail::Copy<0,Description::noOfVariables>::apply(x,y);
    }

    /**
     * @ingroup KaskadeGroup
     * @brief Copy from \ref VectorAnchor "::VSA::Vector" to coefficient vector of %Kaskade 7.
     *
     * Takes into account product space structures.
     */
    template <class Description>
    void copyToCoefficientVector(const ::VSA::Vector& x, typename Description::template CoefficientVectorRepresentation<>::type& y)
    {
      Detail::Copy<0,Description::noOfVariables>::template toCoefficientVector<Description>(x,y);
    }

    /**
     * @ingroup KaskadeGroup
     * @brief Copy coefficient vector of %Kaskade 7 to \ref VectorAnchor "::VSA::Vector".
     *
     * Takes into account product space structures.
     */
    template <class Description>
    void copyFromCoefficientVector(const typename Description::template CoefficientVectorRepresentation<>::type& x,
                                   ::VSA::Vector& y)
    {
      Detail::Copy<0,Description::noOfVariables>::template fromCoefficientVector<Description>(x,y);
    }
  }
}

#endif // ALGORITHM_KASKADE_UTIL_HH
