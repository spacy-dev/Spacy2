#include "abstractVector.hh"

#include "abstractScalarProduct.hh"
#include "abstractVectorSpace.hh"
#include "abstractNorm.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractVector::AbstractVector(const AbstractVectorSpace& space)
      : space_(space)
    {}

    unsigned AbstractVector::spaceIndex() const
    {
      return space_.index();
    }

    const AbstractVectorSpace& AbstractVector::space() const
    {
      return space_;
    }

    bool AbstractVector::equals(const AbstractVector& y) const
    {
      auto y_ = clone(y);
      *y_ -= *this;
      const auto& norm_ = space().norm();
      return norm_(*y_) < eps();
    }

    bool AbstractVector::isAdmissible() const
    {
      return true;
    }


    AbstractVector& AbstractVector::axpy(double a, const AbstractVector& y)
    {
      auto z = clone(y);
      *z *= a;
      return *this +=*z;
    }

    double AbstractVector::operator()(const AbstractVector& y) const
    {
      if( !space().isDualWRT( y.space() ) ) throw InvalidArgumentException("AbstractVector::operator()");
      return applyAsDualTo(y);
    }

    std::ostream& operator<<(std::ostream& os, const AbstractVector& element)
    {
      element.print(os);
      return os;
    }

    double operator* (const AbstractVector& x, const AbstractVector& y)
    {
      if( x.space().isHilbertSpace() && x.spaceIndex() == y.spaceIndex() )
      {
        auto sp = x.space().scalarProduct();
        return sp->operator ()(x,y);
      }

      throw InvalidArgumentException("operator*(const AbstractVector&,const AbstractVector&)");
      return 0.;
    }
  }
}
