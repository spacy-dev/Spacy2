#include "util.hh"

#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Exceptions/invalidArgumentException.hh"

#include "Spacy/vectorSpace.hh"
#include "Spacy/Spaces/PrimalDualProductSpace/vector.hh"
#include "vector.hh"
#include "vectorSpace.hh"

#include <dolfin.h>
#include <iostream>



namespace Spacy
{
//  namespace
//  {
//    void copyVectorIfConsistent(const Vector& x, dolfin::GenericVector& y)
//    {
//      if(is<FEniCS::Vector>(x))
//        y = cast_ref<FEniCS::Vector>(x).impl();
//    }
//  }

  namespace FEniCS
  {
    void copyCoefficients(const dolfin::Form& F, dolfin::Form& G)
    {
      for(std::size_t i=0; i<F.num_coefficients(); ++i) G.set_coefficient( i , F.coefficient(i) );
    }


    void copy(const ::Spacy::Vector& x, dolfin::GenericVector& y)
    {
//      copyVectorIfConsistent(x,y);
      if( is<Vector>(x) )
      {
        y = cast_ref<Vector>(x).impl();
        return;
      }

      if( is<PrimalDualProductSpace::Vector>(x) )
      {
        const auto& x_ = cast_ref<PrimalDualProductSpace::Vector>(x);

          for( auto i : x_.creator().primalSubSpaceIds() )
          {
            const auto& xv_ = cast_ref<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = cast_ref<VectorCreator>(xv_.space()->impl());
              y.setitem(space.inverseDofmap(j),xv_.impl().getitem(j));
            }
          }
          for( auto i : x_.creator().dualSubSpaceIds() )
          {
            const auto& xv_ = cast_ref<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = cast_ref<VectorCreator>(xv_.space()->impl());
              y.setitem(space.inverseDofmap(j),xv_.impl().getitem(j));
            }
          }

        y.apply("insert");
        return;
      }

      throw InvalidArgumentException("copy(const Spacy::Vector& x, dolfin::GenericVector& y)");
    }

    void copy(const ::Spacy::Vector& x, dolfin::Function& y)
    {
      copy(x,*y.vector());
    }


    void copy(const dolfin::GenericVector& y, ::Spacy::Vector& x)
    {
      if( is<Vector>(x) )
      {
        cast_ref<Vector>(x).impl() = y;
        return;
      }

      if( is<PrimalDualProductSpace::Vector>(x) )
      {

        auto& x_ = cast_ref<PrimalDualProductSpace::Vector>(x);

          for( auto i : x_.creator().primalSubSpaceIds() )
          {
            auto& xv_ = cast_ref<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = cast_ref<VectorCreator>(xv_.space()->impl());
              xv_.impl().setitem( j , y.getitem( space.inverseDofmap(j) ) );
            }
            cast_ref<Vector>( x_.variable(i) ).impl().apply("insert");
          }

          for( auto i : x_.creator().dualSubSpaceIds() )
          {
            auto& xv_ = cast_ref<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = cast_ref<VectorCreator>(xv_.space()->impl());
              xv_.impl().setitem( j , y.getitem( space.inverseDofmap(j) ) );
            }
            cast_ref<Vector>( x_.variable(i) ).impl().apply("insert");
          }

        return;
      }

      throw InvalidArgumentException("copy(const dolfin::GenericVector& y, ::Spacy::Vector& x)");
    }

    void copy(const dolfin::Function& y, ::Spacy::Vector& x)
    {
      copy(*y.vector(),x);
    }

  }
}
