// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_KASKADE_UTIL_HH
#define SPACY_ADAPTER_KASKADE_UTIL_HH

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
  /** @addtogroup KaskadeGroup
   * @{
   */

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
        using type = ::Kaskade::VariableSetDescription< Spaces , boost::fusion::vector< Variable > >;
      };

      template <class Description, int i>
      using ExtractDescription_t = typename ExtractDescription<Description,i>::type;

      template <class Description, int i, int n>
      struct MakeSpaces
      {
        using Variable = std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Variables,i>::type>;

        static void apply(const Description& descriptions, std::vector<std::shared_ptr< VectorSpace> >& newSpaces)
        {
          newSpaces[i] = std::make_shared< VectorSpace>(
                           ::Spacy::makeHilbertSpace(
                             VectorCreator< ExtractDescription_t<Description,i> >( descriptions.spaces ,
                                                                                   std::vector<std::string>(1,descriptions.names[i]) ),
                                                                                   l2Product< ExtractDescription_t<Description,i> >() ) );
          MakeSpaces<Description,i+1,n>::apply(descriptions,newSpaces);
        }
      };

      template <class Description, int n>
      struct MakeSpaces<Description,n,n>
      {
        static void apply(const Description&, std::vector<std::shared_ptr< VectorSpace> >&)
        {}
      };

      template <class Description, unsigned i, unsigned j, unsigned n,
                bool doapply = ( i == std::decay_t<typename boost::fusion::result_of::at_c<typename Description::Variables,j>::type>::spaceIndex ) >
      struct ExtractSpace
      {
        using Variables = typename Description::Variables;
        static auto apply(const ProductSpace::VectorCreator& spaces)
        {
          return creator< VectorCreator< ExtractDescription_t<Description,j> > >( spaces.subSpace(j) ).impl().spaces;
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
          return creator< VectorCreator<Description> >(spaces).impl().spaces;
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
        return extractSpace<Description,0>(spaces);
//        return typename Description::Spaces( extractSpace<Description,is>(spaces)... );
      }

      template <class Description, int i>
      void copyToVariableSetDescription(const ProductSpace::Vector& x, ::Kaskade::VariableSet<Description>& y, int j=0)
      {
        if( is< Vector< ExtractDescription_t<Description,i> > >(x.component(j)) )
        {
          boost::fusion::at_c<i>(y.data).coefficients() = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(x.component(j)).impl().data);
          return;
        }

        if( is<ProductSpace::Vector>(x.component(j)))
        {
          const auto& x_ = cast_ref<ProductSpace::Vector>(x.component(j));
          for(auto k=0u; k<x_.numberOfVariables(); ++k)
            copyToVariableSetDescription<Description,i>( x_ , y , k );
          return;
        }
      }

      template <class Description, int i, class CoeffVector>
      void copyToCoefficientVector(const ProductSpace::Vector& x, CoeffVector& y, int j=0)
      {
        if( is< Vector< ExtractDescription_t<Description,i> > >(x.component(j)) )
        {
          boost::fusion::at_c<i>(y.data) = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(x.component(j)).impl().data);
          return;
        }

        if( is<ProductSpace::Vector>(x.component(j)))
        {
          const auto& x_ = cast_ref<ProductSpace::Vector>(x.component(j));
          for(auto k=0u; k<x_.numberOfVariables(); ++k)
            copyToCoefficientVector<Description,i>( x_ , y , k );
          return;
        }
      }


      template <class Description, int i, class CoeffVector>
      void copyFromCoefficientVector(const CoeffVector& x, ProductSpace::Vector& y, int j=0)
      {
        if( is< Vector< ExtractDescription_t<Description,i> > >(y.component(j)) )
        {
          boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(y.component(j)).impl().data) = boost::fusion::at_c<i>(x.data);
          return;
        }

        if( is<ProductSpace::Vector>(y.component(j)))
        {
          auto& y_ = cast_ref<ProductSpace::Vector>(y.component(j));
          for(auto k=0u; k<y_.numberOfVariables(); ++k)
            copyFromCoefficientVector<Description,i>( x , y_ , k );
          return;
        }
      }


      template <int i, int n>
      struct Copy
      {
        template <class Description>
        static void apply(const ProductSpace::Vector& x, ::Kaskade::VariableSet<Description>& y)
        {
          for(auto j=0u; j<x.numberOfVariables(); ++j)
            copyToVariableSetDescription<Description,i>(x,y,j);
          Copy<i+1,n>::apply(x,y);
        }

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const ProductSpace::Vector& x, CoeffVector& y)
        {
          for(auto j=0u; j<x.numberOfVariables(); ++j)
            copyToCoefficientVector<Description,i>(x,y,j);
          Copy<i+1,n>::template toCoefficientVector<Description>(x,y);
        }

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector& x, ProductSpace::Vector& y)
        {
          for(auto j=0u; j<y.numberOfVariables(); ++j)
            copyFromCoefficientVector<Description,i>(x,y,j);
          Copy<i+1,n>::template fromCoefficientVector<Description>(x,y);
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

    template <class Description>
    typename Description::Spaces extractSingleSpace(const VectorSpace& spaces)
    {
        return Detail::ExtractSingleSpace<Description,Description::noOfVariables!=1>::apply(spaces);
    }

    template <class Description>
    typename Description::Spaces extractProductSpace(const VectorSpace& spaces)
    {
      if( is<ProductSpace::VectorCreator>( cast_ref<ProductSpace::VectorCreator>(spaces.creator()).subSpace(0).creator() ) )
      {
        const auto& productSpace = cast_ref<ProductSpace::VectorCreator>(spaces.creator());
//        for(auto i = 0u; productSpace.subSpaces().size(); ++i )
        return extractProductSpace<Description>(productSpace.subSpace(0));
      }
      else
      {
        const auto& spaces_ = cast_ref<ProductSpace::VectorCreator>(spaces.creator());
        using seq = std::make_integer_sequence<unsigned,boost::fusion::result_of::size<typename Description::Spaces>::value>;
        return Detail::extractSpaces<Description>(spaces_,seq{});
      }
    }


    /// Extract boost::fusion::vector< const Space0*, const Space1*, ... > from spaces.
    template <class Description>
    typename Description::Spaces extractSpaces(const VectorSpace& spaces)
    {
      using Spaces = typename Description::Spaces;

      if( is< VectorCreator<Description> >(spaces.creator()) )
        return extractSingleSpace<Description>(spaces);

      if( is<ProductSpace::VectorCreator>(spaces.creator()) )
        return extractProductSpace<Description>(spaces);

      assert(false);
    }

    /// Copy from ::Spacy::Vector to %Kaskade::VariableSet<Description>.
    template <class Description>
    void copy(const ::Spacy::Vector& x, ::Kaskade::VariableSet<Description>& y)
    {
      if( is< Vector< Description > >(x) )
      {
        boost::fusion::at_c<0>(y.data).coefficients() = boost::fusion::at_c<0>(cast_ref< Vector< Description > >(x).impl().data);
        return;
      }

      if( is<ProductSpace::Vector>(x))
      {
        Detail::Copy<0,Description::noOfVariables>::apply(cast_ref<ProductSpace::Vector>(x),y);
        return;
      }
    }

    /// Copy from ::Spacy::Vector to coefficient vector of %Kaskade 7.
    template <class Description>
    void copyToCoefficientVector(const ::Spacy::Vector& x, typename Description::template CoefficientVectorRepresentation<>::type& y)
    {
      if( is< Vector< Description > >(x) )
      {
        boost::fusion::at_c<0>(y.data) = boost::fusion::at_c<0>(cast_ref< Vector< Description > >(x).impl().data);
        return;
      }

      if( is<ProductSpace::Vector>(x))
      {
        Detail::Copy<0,Description::noOfVariables>::template toCoefficientVector<Description>(cast_ref<ProductSpace::Vector>(x),y);
        return;
      }
    }

    ///  Copy coefficient vector of %Kaskade 7 to ::Spacy::Vector.
    template <class Description>
    void copyFromCoefficientVector(const typename Description::template CoefficientVectorRepresentation<>::type& x,
                                   ::Spacy::Vector& y)
    {
      if( is< Vector< Description > >(y) )
      {
        boost::fusion::at_c<0>(cast_ref< Vector< Description > >(y).impl().data) = boost::fusion::at_c<0>(x.data);
        return;
      }

      if( is<ProductSpace::Vector>(y))
      {
        Detail::Copy<0,Description::noOfVariables>::template fromCoefficientVector<Description>(x,cast_ref<ProductSpace::Vector>(y));
        return;
      }
    }
  }
  /** @} */
}

#endif // SPACY_ADAPTER_KASKADE_UTIL_HH
