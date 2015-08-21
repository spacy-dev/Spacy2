#include "real.hh"

#include "vectorSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/castTo.hh"

#include <iostream>
#include <stdexcept>

namespace Algorithm
{
  using Interface::AbstractVector;

  Real::Real(double x, const VectorSpace &space)
    : AbstractVector(space), x_(x)
  {}

  Real::Real(const VectorSpace& space)
    : Real(0.,space)
  {}

  void Real::copyTo(AbstractVector& y) const
  {
    if( !is<Real>(y) ) throw InvalidArgumentException("Real::copyTo");

    castTo<Real>(y).x_ = x_;
  }

  Real& Real::operator=(const AbstractVector& y)
  {
    if( !is<Real>(y) ) throw InvalidArgumentException("Real::operator=");
    x_ = castTo<Real>(y).x_;
    return *this;
  }

  Real& Real::operator+=(const AbstractVector& y)
  {
    if( !is<Real>(y) ) throw InvalidArgumentException("Real::operator+=");
    x_ += castTo<Real>(y).x_;
    return *this;
  }

  Real& Real::operator-=(const AbstractVector& y)
  {
    if( !is<Real>(y) ) throw InvalidArgumentException("Real::operator-=");
    x_ -= castTo<Real>(y).x_;
    return *this;
  }

  Real& Real::operator*=(double a)
  {
    x_ *= a;
    return *this;
  }

  std::unique_ptr<AbstractVector> Real::operator- () const
  {
    return std::make_unique<Real>(-x_, this->space() );
  }

  double Real::applyAsDualTo(const AbstractVector& y) const
  {
    return x_ * castTo<Real>(y).x_;
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

  void Real::print(std::ostream& os) const
  {
    os << "Space index: " << space().index() << "\n";
    os << x_ << std::endl;
  }

  Real* Real::cloneImpl() const
  {
    return new Real(*this);
  }
}
