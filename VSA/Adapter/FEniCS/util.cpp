#include "util.hh"

#include "VSA/Util/cast.hh"
#include "VSA/Util/Exceptions/invalidArgumentException.hh"

#include "VSA/vectorSpace.hh"
#include "VSA/Spaces/ProductSpace/vector.hh"
#include "vector.hh"
#include "vectorSpace.hh"

#include <dolfin.h>

namespace VSA
{
  namespace FEniCS
  {
    void copyCoefficients(const dolfin::Form& F, dolfin::Form& G)
    {
      for(std::size_t i=0; i<F.num_coefficients(); ++i) G.set_coefficient( i , F.coefficient(i) );
    }


    void copy(const ::VSA::Vector& x, dolfin::GenericVector& y)
    {
      if( is<Vector>(x) )
      {
        y = cast_ref<Vector>(x).impl();
        return;
      }

      if( is<ProductSpace::Vector>(x) )
      {
        const auto& x_ = cast_ref<ProductSpace::Vector>(x);

          for( auto i : x_.creator().primalSubSpaceIds() )
          {
            const auto& xv_ = cast_ref<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = cast_ref<VectorCreator>(xv_.space()->impl());

              if(x_.isPrimalEnabled())
                y.setitem(space.inverseDofmap(j),xv_.impl().getitem(j));
              else y.setitem(space.inverseDofmap(j),0.);
            }
          }
          for( auto i : x_.creator().dualSubSpaceIds() )
          {
            const auto& xv_ = cast_ref<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = cast_ref<VectorCreator>(xv_.space()->impl());

              if(x_.isDualEnabled())
                y.setitem(space.inverseDofmap(j),xv_.impl().getitem(j));
              else y.setitem(space.inverseDofmap(j),0.);
            }
          }

        y.apply("insert");
        return;
      }

      throw InvalidArgumentException("copy(const Interface::AbstractVector& x, dolfin::GenericVector& y)");
    }

    void copy(const ::VSA::Vector& x, dolfin::Function& y)
    {
      copy(x,*y.vector());
    }


    void copy(const dolfin::GenericVector& y, ::VSA::Vector& x)
    {
      if( is<Vector>(x) )
      {
        cast_ref<Vector>(x).impl() = y;
        return;
      }

      if( is<ProductSpace::Vector>(x) )
      {

        auto& x_ = cast_ref<ProductSpace::Vector>(x);

          for( auto i : x_.creator().primalSubSpaceIds() )
          {
            auto& xv_ = cast_ref<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = cast_ref<VectorCreator>(xv_.space()->impl());
              if( x_.isPrimalEnabled())
                xv_.impl().setitem( j , y.getitem( space.inverseDofmap(j) ) );
              else xv_.impl().setitem( j , 0. );
            }
            cast_ref<Vector>( x_.variable(i) ).impl().apply("insert");
          }

          for( auto i : x_.creator().dualSubSpaceIds() )
          {
            auto& xv_ = cast_ref<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = cast_ref<VectorCreator>(xv_.space()->impl());
              if( x_.isDualEnabled())
                xv_.impl().setitem( j , y.getitem( space.inverseDofmap(j) ) );
              else
                xv_.impl().setitem( j , 0. );
            }
            cast_ref<Vector>( x_.variable(i) ).impl().apply("insert");
          }

        return;
      }

      throw InvalidArgumentException("copy(const dolfin::GenericVector& y, ::VSA::Vector& x)");
    }

    void copy(const dolfin::Function& y, ::VSA::Vector& x)
    {
      copy(*y.vector(),x);
    }

  }
}
