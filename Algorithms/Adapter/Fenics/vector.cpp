#include "vector.hh"

#include "hilbertSpace.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

#include <utility>
#include <iostream>

namespace Algorithm
{
  using Interface::AbstractBanachSpace;
  using Interface::AbstractFunctionSpaceElement;

  namespace Fenics
  {
    Vector::Vector(const AbstractBanachSpace& space)
      : AbstractFunctionSpaceElement(space), Impl<dolfin::Function>( dolfin::Function( dynamic_cast<const HilbertSpace&>(space).impl() ) )
    {}

    Vector::Vector(const dolfin::Function& f, const AbstractBanachSpace& space)
      : AbstractFunctionSpaceElement(space), Impl<dolfin::Function>( f )
    {}

    void Vector::copyTo(AbstractFunctionSpaceElement& y) const
    {
      toVector(y).impl() = impl();
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

    Vector& Vector::operator=(const AbstractFunctionSpaceElement& y)
    {
      *impl().vector() = *toVector(y).impl().vector();
      return *this;
    }

    Vector& Vector::operator+=(const AbstractFunctionSpaceElement& y)
    {
//      impl() = impl() + toVector(y).impl();
      *impl().vector() += *toVector(y).impl().vector();
      return *this;
    }

    Vector& Vector::axpy(double a, const AbstractFunctionSpaceElement& y)
    {
      impl().vector()->axpy(a,*toVector(y).impl().vector());
      return *this;
    }

    Vector& Vector::operator-=(const AbstractFunctionSpaceElement& y)
    {
//      impl() = impl() - toVector(y).impl();
      *impl().vector() -= *toVector(y).impl().vector();
      return *this;
    }

    Vector& Vector::operator*=(double a)
    {
      *impl().vector() *= a;
      return *this;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> Vector::operator- () const
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

    double Vector::applyAsDualTo(const AbstractFunctionSpaceElement& y) const
    {
      auto const& y_ = toVector(y);
      return impl().vector()->inner( *y_.impl().vector() );
    }

    Vector* Vector::cloneImpl() const
    {
      return new Vector(*this);
//      return new Vector(impl(),space());
    }


    bool isVector(const AbstractFunctionSpaceElement& y)
    {
      return dynamic_cast< const Vector* >(&y) != nullptr;
    }

    Vector& toVector(AbstractFunctionSpaceElement& x)
    {
      if( !isVector(x) ) throw InvalidArgumentException("toVector(AbstractFunctionSpaceElement&)");
      return dynamic_cast<Vector&>(x);
    }

    const Vector& toVector(const AbstractFunctionSpaceElement& x)
    {
      if( !isVector(x) ) throw InvalidArgumentException("toVector(const AbstractFunctionSpaceElement&)");
      return dynamic_cast<const Vector&>(x);
    }


    void copy(const Interface::AbstractFunctionSpaceElement& x, dolfin::GenericVector& y, bool verbose)
    {
      if( verbose ) std::cout << "copy rhs" << std::endl;
      if( isVector(x) )
      {
        y = *toVector(x).impl().vector();
        return;
      }

      if( isProductSpaceElement(x) )
      {
        const auto& x_ = toProductSpaceElement(x);

        if( verbose ) std::cout << "variables: " << x_.variables().size() << std::endl;
//        for( auto i=0u; i<x_.variables().size(); ++i )
          for( auto i : x_.space().primalSubSpaceIds() )
          {
            const auto& xv_ = toVector( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = toHilbertSpace( xv_.space() );

              if( verbose) std::cout << "primal variable: " << x_.isPrimalEnabled() << ": " << j << " -> " << space.inverseDofmap(j) << ": " << toVector( x_.variable(i) ).impl().vector()->getitem(j) << std::endl;
              if(x_.isPrimalEnabled())
                y.setitem(space.inverseDofmap(j),xv_.impl().vector()->getitem(j));
              else y.setitem(space.inverseDofmap(j),0.);
            }
          }
          for( auto i : x_.space().dualSubSpaceIds() )
          {
            const auto& xv_ = toVector( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = toHilbertSpace( xv_.space() );

              if( verbose) std::cout << "dual variable: " << x_.isDualEnabled() << ": " << j << " -> " << space.inverseDofmap(j) << ": " << toVector( x_.variable(i) ).impl().vector()->getitem(j) << std::endl;
              if(x_.isDualEnabled())
                y.setitem(space.inverseDofmap(j),xv_.impl().vector()->getitem(j));
              else y.setitem(space.inverseDofmap(j),0.);
            }
          }

        y.apply("insert");
        //x_.reset();

        return;
      }

      throw InvalidArgumentException("copy(const Interface::AbstractFunctionSpaceElement& x, dolfin::GenericVector& y)");
    }


    void copy(const dolfin::GenericVector& y, Interface::AbstractFunctionSpaceElement& x, bool verbose)
    {
      if(verbose) std::cout << "from vector" << std::endl;
      if( isVector(x) )
      {
        *toVector(x).impl().vector() = y;
        return;
      }

      if( isProductSpaceElement(x) )
      {

        auto& x_ = toProductSpaceElement(x);

        //for( auto i=0u; i<x_.variables().size(); ++i )
          for( auto i : x_.space().primalSubSpaceIds() )
          {
            auto& xv_ = toVector( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = toHilbertSpace( xv_.space() );
              if( verbose) std::cout << space.inverseDofmap(j) << " -> " << j << ": " << y.getitem(i + j*x_.variables().size()) << std::endl;
              if( x_.isPrimalEnabled())
                xv_.impl().vector()->setitem( j , y.getitem( space.inverseDofmap(j)  /*i + j*x_.variables().size()*/ ) );
              else xv_.impl().vector()->setitem( j , 0. );
            }
            toVector( x_.variable(i) ).impl().vector()->apply("insert");
          }

          for( auto i : x_.space().dualSubSpaceIds() )
          {
            auto& xv_ = toVector( x_.variable(i) );
            for(auto j=0u; j<xv_.size(); ++j)
            {
              const auto& space = toHilbertSpace( xv_.space() );
              if( verbose) std::cout << space.inverseDofmap(j) << " -> " << j << ": " << y.getitem(i + j*x_.variables().size()) << std::endl;
              if( x_.isDualEnabled())
                xv_.impl().vector()->setitem( j , y.getitem( space.inverseDofmap(j)  /*i + j*x_.variables().size()*/ ) );
              else
                xv_.impl().vector()->setitem( j , 0. );
            }
            toVector( x_.variable(i) ).impl().vector()->apply("insert");
          }


        //x_.reset();
        return;
      }

      throw InvalidArgumentException("copy(const dolfin::GenericVector& y, Interface::AbstractFunctionSpaceElement& x)");
    }

  }
}
