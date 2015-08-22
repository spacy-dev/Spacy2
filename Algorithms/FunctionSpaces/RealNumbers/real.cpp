#include "real.hh"

#include "vectorSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/castTo.hh"

#include <iostream>
#include <stdexcept>
#include <cmath>

namespace Algorithm
{
  Real::Real(double x, const VectorSpace &space)
    : VectorBase<Real>(space), x_(x)
  {}

  Real::Real(const VectorSpace& space)
    : Real(0.,space)
  {}

//  void Real::copyTo(AbstractVector& y) const
//  {
//    if( !is<Real>(y) ) throw InvalidArgumentException("Real::copyTo");

//    castTo<Real>(y).x_ = x_;
//  }

//  Real& Real::operator=(const Real& y)
//  {
////    if( !isAny<Real>(y) ) throw InvalidArgumentException("Real::operator=");
//    x_ = y.x_;
//    return *this;
//  }

//  Real::Real(const Real &y)
//  {
//    *this = y;
//  }

  Real& Real::operator+=(const Real& y)
  {
//    if( !isAny<Real>(y) ) throw InvalidArgumentException("Real::operator+=");
    x_ += y.x_;
    return *this;
  }

  Real& Real::operator-=(const Real& y)
  {
//    if( !isAny<Real>(y) ) throw InvalidArgumentException("Real::operator-=");
    x_ -= y.x_;
    return *this;
  }

  Real& Real::operator*=(double a)
  {
    x_ *= a;
    return *this;
  }

  Real Real::operator- () const
  {
    return Real(-x_, this->space() );
  }

  double Real::operator()(const Real& y) const
  {
    return x_ * y.x_;
  }


  unsigned Real::size() const
  {
    return 1;
  }


  double& Real::coefficient(unsigned i)
  {
    if(i > 0) throw std::out_of_range("In Real::coefficient(" + std::to_string(i) + ").");
    return x_;
  }

  const double& Real::coefficient(unsigned i) const
  {
    if(i > 0) throw std::out_of_range("In Real::coefficient(" + std::to_string(i) + ").");
    return x_;
  }

  bool Real::operator==(const Real& y) const
  {
    return std::fabs(x_-y.x_) < eps();
  }

//  void Real::print(std::ostream& os) const
//  {
//    os << "Space index: " << space().index() << "\n";
//    os << x_ << std::endl;
//  }

//  Real* Real::cloneImpl() const
//  {
//    return new Real(*this);
//  }
}
