#include "vector.hh"

#include "vectorSpace.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

#include "Util/castTo.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

#include <utility>
#include <iostream>

#include <boost/type_erasure/any_cast.hpp>

namespace Algorithm
{
  using Interface::AbstractVector;

  namespace Fenics
  {
    Vector::Vector(const ::Algorithm::VectorSpace& space)
      : VectorBase<Vector>(space),
        Impl<dolfin::Function>( dolfin::Function( boost::type_erasure::any_cast<const VectorSpace&>(space.impl()).impl() ) )
    {}

    Vector::Vector(const dolfin::Function& f, const ::Algorithm::VectorSpace& space)
      : VectorBase<Vector>(space), Impl<dolfin::Function>( f )
    {}

//    void Vector::copyTo(AbstractVector& y) const
//    {
//      castTo<Vector>(y).impl() = impl();
//    }

//    void Vector::print(std::ostream& os) const
//    {
//      //      os << impl(); // todo generalize output
//    }

    Vector& Vector::operator=(const dolfin::Function& v)
    {
      impl() = v;
      return *this;
    }

    Vector& Vector::operator=(const Vector& y)
    {
      *impl().vector() = *y.impl().vector();
      return *this;
    }

    Vector& Vector::operator+=(const Vector& y)
    {
      *impl().vector() += *y.impl().vector();
      return *this;
    }

//    Vector& Vector::axpy(double a, const Vector& y)
//    {
//      impl().vector()->axpy(a,*castTo<Vector>(y).impl().vector());
//      return *this;
//    }

    Vector& Vector::operator-=(const Vector& y)
    {
      *impl().vector() -= *y.impl().vector();
      return *this;
    }

    Vector& Vector::operator*=(double a)
    {
      *impl().vector() *= a;
      return *this;
    }

    Vector Vector::operator- () const
    {
      auto v = *this;
      v *= -1;
      return v;
    }

    bool Vector::operator==(const Vector& y) const
    {
      auto dx = impl().vector()->copy();
     *dx -= *y.impl().vector();
      return dx->inner( *dx ) < eps();
    }

    double& Vector::coefficient(unsigned i)
    {
      throw CallOfUndefinedFunctionException("Fenics::Vector::coefficient");
//      return (*impl().vector())[i];
    }

    const double& Vector::coefficient(unsigned i) const
    {
      throw CallOfUndefinedFunctionException("Fenics::Vector::coefficient");
//      return (*impl().vector())[i];
    }

    unsigned Vector::size() const
    {
      return impl().vector()->size(); // todo generalize
    }

    dolfin::Function& Vector::operator[](unsigned i)
    {
      return impl()[i];
    }

    const dolfin::Function& Vector::operator[](unsigned i) const
    {
      return impl()[i];
    }

    double Vector::operator()(const Vector& y) const
    {
      return impl().vector()->inner( *y.impl().vector() );
    }

//    Vector* Vector::cloneImpl() const
//    {
//      return new Vector(*this);
//    }


    void copy(const ::Algorithm::Vector& x, dolfin::GenericVector& y, bool verbose)
    {
      if( verbose ) std::cout << "copy rhs" << std::endl;
      if( isAny<Vector>(x) )
      {
        y = *castAny<Vector>(x).impl().vector();
        return;
      }

      if( isAny<ProductSpaceElement>(x) )
      {
        const auto& x_ = castAny<ProductSpaceElement>(x);

//        if( verbose ) std::cout << "variables: " << x_.variables().size() << std::endl;
//        for( auto i=0u; i<x_.variables().size(); ++i )
          for( auto i : x_.productSpace().primalSubSpaceIds() )
          {
            const auto& xv_ = castAny<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = castAny<Fenics::VectorSpace>( xv_.space().impl() );

              if( verbose) std::cout << "primal variable: " << x_.isPrimalEnabled() << ": " << j << " -> " << space.inverseDofmap(j) << ": " << castAny<Vector>( x_.variable(i) ).impl().vector()->getitem(j) << std::endl;
              if(x_.isPrimalEnabled())
                y.setitem(space.inverseDofmap(j),xv_.impl().vector()->getitem(j));
              else y.setitem(space.inverseDofmap(j),0.);
            }
          }
          for( auto i : x_.productSpace().dualSubSpaceIds() )
          {
            const auto& xv_ = castAny<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = castAny<Fenics::VectorSpace>( xv_.space().impl() );

              if( verbose) std::cout << "dual variable: " << x_.isDualEnabled() << ": " << j << " -> " << space.inverseDofmap(j) << ": " << castAny<Vector>( x_.variable(i) ).impl().vector()->getitem(j) << std::endl;
              if(x_.isDualEnabled())
                y.setitem(space.inverseDofmap(j),xv_.impl().vector()->getitem(j));
              else y.setitem(space.inverseDofmap(j),0.);
            }
          }

        y.apply("insert");
        return;
      }

      throw InvalidArgumentException("copy(const Interface::AbstractVector& x, dolfin::GenericVector& y)");
    }

    void copy(const ::Algorithm::Vector& x, dolfin::Function& y, bool verbose)
    {
      copy(x,*y.vector(),verbose);
    }


    void copy(const dolfin::GenericVector& y, ::Algorithm::Vector& x, bool verbose)
    {
      if(verbose) std::cout << "from vector" << std::endl;
      if( isAny<Vector>(x) )
      {
        *castAny<Vector>(x).impl().vector() = y;
        return;
      }

      if( isAny<ProductSpaceElement>(x) )
      {

        auto& x_ = castAny<ProductSpaceElement>(x);

          for( auto i : x_.productSpace().primalSubSpaceIds() )
          {
            auto& xv_ = castAny<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = castAny<Fenics::VectorSpace>( xv_.space().impl() );
//              if( verbose) std::cout << space.inverseDofmap(j) << " -> " << j << ": " << y.getitem(i + j*x_.variables().size()) << std::endl;
              if( x_.isPrimalEnabled())
                xv_.impl().vector()->setitem( j , y.getitem( space.inverseDofmap(j)  /*i + j*x_.variables().size()*/ ) );
              else xv_.impl().vector()->setitem( j , 0. );
            }
            castAny<Vector>( x_.variable(i) ).impl().vector()->apply("insert");
          }

          for( auto i : x_.productSpace().dualSubSpaceIds() )
          {
            auto& xv_ = castAny<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = castAny<Fenics::VectorSpace>( xv_.space().impl() );
//              if( verbose) std::cout << space.inverseDofmap(j) << " -> " << j << ": " << y.getitem(i + j*x_.variables().size()) << std::endl;
              if( x_.isDualEnabled())
                xv_.impl().vector()->setitem( j , y.getitem( space.inverseDofmap(j)  /*i + j*x_.variables().size()*/ ) );
              else
                xv_.impl().vector()->setitem( j , 0. );
            }
            castAny<Vector>( x_.variable(i) ).impl().vector()->apply("insert");
          }

        return;
      }

      throw InvalidArgumentException("copy(const dolfin::GenericVector& y, Interface::AbstractVector& x)");
    }
  }
}
