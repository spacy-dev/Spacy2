#include "vector.hh"

#include "vectorSpace.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

#include "Util/castTo.hh"

#include <utility>
#include <iostream>

namespace Algorithm
{
  using Interface::AbstractVector;

  namespace Fenics
  {
    Vector::Vector(const Interface::AbstractVectorSpace& space)
      : AbstractVector(space),
        Impl<dolfin::Function>( dolfin::Function( castTo<Fenics::VectorSpace>(space).impl() ) )
    {}

    Vector::Vector(const dolfin::Function& f, const Interface::AbstractVectorSpace& space)
      : AbstractVector(space), Impl<dolfin::Function>( f )
    {}

    void Vector::copyTo(AbstractVector& y) const
    {
      castTo<Vector>(y).impl() = impl();
    }

    void Vector::print(std::ostream& os) const
    {
      //      os << impl(); // todo generalize output
    }

    Vector& Vector::operator=(const dolfin::Function& v)
    {
      impl() = v;
      return *this;
    }

    Vector& Vector::operator=(const AbstractVector& y)
    {
      *impl().vector() = *castTo<Vector>(y).impl().vector();
      return *this;
    }

    Vector& Vector::operator+=(const AbstractVector& y)
    {
      *impl().vector() += *castTo<Vector>(y).impl().vector();
      return *this;
    }

    Vector& Vector::axpy(double a, const AbstractVector& y)
    {
      impl().vector()->axpy(a,*castTo<Vector>(y).impl().vector());
      return *this;
    }

    Vector& Vector::operator-=(const AbstractVector& y)
    {
      *impl().vector() -= *castTo<Vector>(y).impl().vector();
      return *this;
    }

    Vector& Vector::operator*=(double a)
    {
      *impl().vector() *= a;
      return *this;
    }

    std::unique_ptr<AbstractVector> Vector::operator- () const
    {
      auto v = clone( *this );
      *v *= -1;
      return std::move(v);
    }

    double& Vector::coefficient(unsigned i)
    {
      return *(impl().vector()->data() + i);
    }

    const double& Vector::coefficient(unsigned i) const
    {
      return *(impl().vector()->data() + i);
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

    double Vector::applyAsDualTo(const AbstractVector& y) const
    {
      auto const& y_ = castTo<Vector>(y);
      return impl().vector()->inner( *y_.impl().vector() );
    }

    Vector* Vector::cloneImpl() const
    {
      return new Vector(*this);
    }


    void copy(const Interface::AbstractVector& x, dolfin::GenericVector& y, bool verbose)
    {
      if( verbose ) std::cout << "copy rhs" << std::endl;
      if( is<Vector>(x) )
      {
        y = *castTo<Vector>(x).impl().vector();
        return;
      }

      if( is<ProductSpaceElement>(x) )
      {
        const auto& x_ = castTo<ProductSpaceElement>(x);

//        if( verbose ) std::cout << "variables: " << x_.variables().size() << std::endl;
//        for( auto i=0u; i<x_.variables().size(); ++i )
          for( auto i : x_.space().primalSubSpaceIds() )
          {
            const auto& xv_ = castTo<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = castTo<Fenics::VectorSpace>( xv_.space() );

              if( verbose) std::cout << "primal variable: " << x_.isPrimalEnabled() << ": " << j << " -> " << space.inverseDofmap(j) << ": " << castTo<Vector>( x_.variable(i) ).impl().vector()->getitem(j) << std::endl;
              if(x_.isPrimalEnabled())
                y.setitem(space.inverseDofmap(j),xv_.impl().vector()->getitem(j));
              else y.setitem(space.inverseDofmap(j),0.);
            }
          }
          for( auto i : x_.space().dualSubSpaceIds() )
          {
            const auto& xv_ = castTo<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = castTo<Fenics::VectorSpace>( xv_.space() );

              if( verbose) std::cout << "dual variable: " << x_.isDualEnabled() << ": " << j << " -> " << space.inverseDofmap(j) << ": " << castTo<Vector>( x_.variable(i) ).impl().vector()->getitem(j) << std::endl;
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
      copy(x.impl(),*y.vector(),verbose);
    }


    void copy(const dolfin::GenericVector& y, Interface::AbstractVector& x, bool verbose)
    {
      if(verbose) std::cout << "from vector" << std::endl;
      if( is<Vector>(x) )
      {
        *castTo<Vector>(x).impl().vector() = y;
        return;
      }

      if( is<ProductSpaceElement>(x) )
      {

        auto& x_ = castTo<ProductSpaceElement>(x);

          for( auto i : x_.space().primalSubSpaceIds() )
          {
            auto& xv_ = castTo<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = castTo<Fenics::VectorSpace>( xv_.space() );
//              if( verbose) std::cout << space.inverseDofmap(j) << " -> " << j << ": " << y.getitem(i + j*x_.variables().size()) << std::endl;
              if( x_.isPrimalEnabled())
                xv_.impl().vector()->setitem( j , y.getitem( space.inverseDofmap(j)  /*i + j*x_.variables().size()*/ ) );
              else xv_.impl().vector()->setitem( j , 0. );
            }
            castTo<Vector>( x_.variable(i) ).impl().vector()->apply("insert");
          }

          for( auto i : x_.space().dualSubSpaceIds() )
          {
            auto& xv_ = castTo<Vector>( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = castTo<Fenics::VectorSpace>( xv_.space() );
//              if( verbose) std::cout << space.inverseDofmap(j) << " -> " << j << ": " << y.getitem(i + j*x_.variables().size()) << std::endl;
              if( x_.isDualEnabled())
                xv_.impl().vector()->setitem( j , y.getitem( space.inverseDofmap(j)  /*i + j*x_.variables().size()*/ ) );
              else
                xv_.impl().vector()->setitem( j , 0. );
            }
            castTo<Vector>( x_.variable(i) ).impl().vector()->apply("insert");
          }

        return;
      }

      throw InvalidArgumentException("copy(const dolfin::GenericVector& y, Interface::AbstractVector& x)");
    }

  }
}
