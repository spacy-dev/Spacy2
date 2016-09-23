#include "real.hh"

#include <Spacy/Util/cast.hh>
#include <Spacy/Util/Exceptions/invalidArgumentException.hh>

#include <Spacy/vector.hh>
#include "vectorSpace.hh"

#include <cmath>

namespace
{
  double computePow(double x, double y)
  {
    return pow(x,y);
  }

  double computeSqrt(double x)
  {
    return sqrt(x);
  }

  double computeCbrt(double x)
  {
    return cbrt(x);
  }
}

namespace Spacy
{
  Real::Real(double x, const VectorSpace& space)
    : VectorBase(space),
      Mixin::Get<double>(x)
  {}

  Real::Real(const VectorSpace& space)
    : Real(0.,space)
  {}

  Real::Real(double x)
    : Real(x,Space::R)
  {}

  Real& Real::operator=(double y)
  {
    get() = y;
    return *this;
  }

  Real Real::operator()(const Real& y) const
  {
    checkDualPairing(*this,y);
    return *this * y;
  }

  Real& Real::operator+=(const Real& y)
  {
    checkSpaceCompatibility(this->space(),y.space());
    get() += y.get();
    return *this;
  }

  Real& Real::operator-=(const Real& y)
  {
    checkSpaceCompatibility(this->space(),y.space());
    get() -= y.get();
    return *this;
  }

  Real& Real::operator*=(const Real& y)
  {
    get() *= y.get();
    return *this;
  }

  Real& Real::operator*=(double a)
  {
    get() *= a;
    return *this;
  }

  Real& Real::operator/=(const Real& y)
  {
      get() *= y.get();
      return *this;
  }

  Real Real::operator-() const
  {
    auto result = *this;
    result.get() *= -1;
    return result;
  }

  bool Real::operator==(const Real& y) const
  {
    checkSpaceCompatibility(this->space(),y.space());
    return get() == y.get();
  }


  double toDouble(const Vector& x)
  {
    if( !is<Real>(x) ) throw Exception::InvalidArgument("Spacy::toDouble(const Vector& v)");
    return get(cast_ref<Real>(x));
  }

  double& toDouble(Vector& x)
  {
    if( !is<Real>(x) ) throw Exception::InvalidArgument("Spacy::toDouble(Vector& v)");
    return get(cast_ref<Real>(x));
  }

  Real abs(Real x)
  {
    return ( x > 0 ) ? x : -x;
  }


  Real max(const Real& x, const Real& y)
  {
    return ( x.get() > y.get() ) ? x : y;
  }

  Real min(const Real& x, const Real& y)
  {
    return ( x.get() < y.get() ) ? x : y;
  }

  Real pow(Real x, double y)
  {
    return Real(computePow(x.get(),y),x.space());
  }

  Real sqrt(Real x)
  {
    return Real(computeSqrt(x.get()),x.space());
  }

  Real cbrt(Real x)
  {
    return Real(computeCbrt(x.get()),x.space());
  }
}
