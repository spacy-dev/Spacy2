#ifndef SPACY_KASKADE_UTIL_HH
#define SPACY_KASKADE_UTIL_HH

#include <memory>
#include <type_traits>
#include <vector>

#include "fem/variables.hh"

#include "Spacy/vectorSpace.hh"
#include "Spacy/Spaces/productSpace.hh"
#include "l2Product.hh"
#include "vector.hh"

namespace Spacy
{
  namespace Kaskade
  {
//    namespace
//    {
//      void copyToDolfinVectorIfConsistent(const Vector& x, dolfin::GenericVector& y)
//      {
//        if( !is<FEniCS::Vector>(x) ) return;

//        const auto& x_ = cast_ref<FEniCS::Vector>(x);
//        for(auto j=0u; j<cast_ref<FEniCS::VectorCreator>(x_.space()->creator()).size(); ++j)
//        {
//          const auto& creator = Spacy::creator<FEniCS::VectorCreator>(*x_.space());
//          y.setitem(creator.inverseDofmap(j),x_.impl().getitem(j));
//        }
//        y.apply("insert");
//      }

//      void copyFromDolfinVectorIfConsistent(const dolfin::GenericVector& y, Vector& x)
//      {
//        if( !is<FEniCS::Vector>(x) ) return;

//        auto& x_ = cast_ref<FEniCS::Vector>(x);
//        for(auto j=0u; j<cast_ref<FEniCS::VectorCreator>(x_.space()->creator()).size(); ++j)
//        {
//          const auto& creator = Spacy::creator<FEniCS::VectorCreator>(*x_.space());
//          x_.impl().setitem( j , y.getitem( creator.inverseDofmap(j) ) );
//        }
//        x_.impl().apply("insert");
//      }
//    }

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
          newSpaces[i] = std::make_shared< VectorSpace>( ::Spacy::makeHilbertSpace( VectorCreator< ExtractDescription_t<Description,i> >{ *boost::fusion::at_c<Variable::spaceIndex>(spaces) } , l2Product< ExtractDescription_t<Description,i> >{} ) );
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
          return &cast_ref< VectorCreator< ExtractDescription_t<Description,j> > >( spaces.subSpace(j).creator()).impl();
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
          return Spaces{ &cast_ref< VectorCreator<Description> >(spaces.creator()).impl() };
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
          boost::fusion::at_c<i>(y.data).coefficients() = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(x.component(i)).impl().data);
          Copy<i+1,n>::apply(x,y);
        }

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const ProductSpace::Vector& x, CoeffVector& y)
        {
          boost::fusion::at_c<i>(y.data) = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(x.component(i)).impl().data);
          Copy<i+1,n>::template toCoefficientVector<Description>(x,y);
        }

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector& x, ProductSpace::Vector& y)
        {
          boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(y.component(i)).impl().data) = boost::fusion::at_c<i>(x.data);
          Copy<i+1,n>::template fromCoefficientVector<Description>(x,y);
        }
      };

      template <int n>
      struct Copy<0,n>
      {
        template <class Description>
        static void apply(const ::Spacy::Vector& x, ::Kaskade::VariableSet<Description>& y)
        {
          if( is< Vector< Description > >(x) )
          {
            boost::fusion::at_c<0>(y.data).coefficients() = boost::fusion::at_c<0>(cast_ref< Vector< Description > >(x).impl().data);
            return;
          }

          if( is<ProductSpace::Vector>(x))
          {
            const auto& x_ = cast_ref<ProductSpace::Vector>(x);
            boost::fusion::at_c<0>(y.data).coefficients() = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,0> > >(x_.component(0)).impl().data);
            Copy<1,n>::apply(x_,y);
            return;
          }

          assert(false);
        }

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const ::Spacy::Vector& x, CoeffVector& y)
        {
          if( is< Vector< Description > >(x) )
          {
            boost::fusion::at_c<0>(y.data) = boost::fusion::at_c<0>(cast_ref< Vector< Description > >(x).impl().data);
            return;
          }

          if( is<ProductSpace::Vector>(x))
          {
            const auto& x_ = cast_ref<ProductSpace::Vector>(x);
            boost::fusion::at_c<0>(y.data) = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,0> > >(x_.component(0)).impl().data);
            Copy<1,n>::template toCoefficientVector<Description>(x_,y);
            return;
          }

          assert(false);
        }

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector& x, ::Spacy::Vector& y)
        {
          if( is< Vector< Description > >(y) )
          {
            boost::fusion::at_c<0>(cast_ref< Vector< Description > >(y).impl().data) = boost::fusion::at_c<0>(x.data);
            return;
          }

          if( is<ProductSpace::Vector>(y))
          {
            auto& y_ = cast_ref<ProductSpace::Vector>(y);
            boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,0> > >(y_.component(0)).impl().data) = boost::fusion::at_c<0>(x.data);
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

      if( is< VectorCreator<Description> >(spaces.creator()) )
      {
        return Detail::ExtractSingleSpace<Description,Description::noOfVariables!=1>::apply(spaces);
      }
      if( is<ProductSpace::VectorCreator>(spaces.creator()) )
      {
        const auto& spaces_ = cast_ref<ProductSpace::VectorCreator>(spaces.creator());
        using seq = std::make_integer_sequence<unsigned,boost::fusion::result_of::size<Spaces>::value>;
        return Detail::extractSpaces<Description>(spaces_,seq{});
      }

      assert(false);
    }

    /**
     * @ingroup KaskadeGroup
     * @brief Copy from \ref VectorAnchor "::Spacy::Vector" to %Kaskade::VariableSet<Description>.
     *
     * Takes into account product space structures.
     */
    template <class Description>
    void copy(const ::Spacy::Vector& x, ::Kaskade::VariableSet<Description>& y)
    {
      Detail::Copy<0,Description::noOfVariables>::apply(x,y);
    }

    /**
     * @ingroup KaskadeGroup
     * @brief Copy from \ref VectorAnchor "::Spacy::Vector" to coefficient vector of %Kaskade 7.
     *
     * Takes into account product space structures.
     */
    template <class Description>
    void copyToCoefficientVector(const ::Spacy::Vector& x, typename Description::template CoefficientVectorRepresentation<>::type& y)
    {
      Detail::Copy<0,Description::noOfVariables>::template toCoefficientVector<Description>(x,y);
    }

    /**
     * @ingroup KaskadeGroup
     * @brief Copy coefficient vector of %Kaskade 7 to \ref VectorAnchor "::Spacy::Vector".
     *
     * Takes into account product space structures.
     */
    template <class Description>
    void copyFromCoefficientVector(const typename Description::template CoefficientVectorRepresentation<>::type& x,
                                   ::Spacy::Vector& y)
    {
      Detail::Copy<0,Description::noOfVariables>::template fromCoefficientVector<Description>(x,y);
    }
  }
}

#endif // SPACY_KASKADE_UTIL_HH
