#include "real.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/castTo.hh"

#include <iostream>
#include <stdexcept>

namespace Algorithm
{
  using Interface::AbstractBanachSpace;
  using Interface::AbstractFunctionSpaceElement;

  Real::Real(double x, const AbstractBanachSpace &space)
    : AbstractFunctionSpaceElement(space), x_(x)
  {}

  Real::Real(const AbstractBanachSpace &space)
    : Real(0.,space)
  {}

  void Real::copyTo(AbstractFunctionSpaceElement& y) const
  {
    if( !is<Real>(y) ) throw InvalidArgumentException("Real::copyTo");

    castTo<Real>(y).x_ = x_;
  }

  Real& Real::operator=(const AbstractFunctionSpaceElement& y)
  {
    if( !is<Real>(y) ) throw InvalidArgumentException("Real::operator=");
    x_ = castTo<Real>(y).x_;
    return *this;
  }

  Real& Real::operator+=(const AbstractFunctionSpaceElement& y)
  {
    if( !is<Real>(y) ) throw InvalidArgumentException("Real::operator+=");
    x_ += castTo<Real>(y).x_;
    return *this;
  }

  Real& Real::operator-=(const AbstractFunctionSpaceElement& y)
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

  std::unique_ptr<AbstractFunctionSpaceElement> Real::operator- () const
  {
    return std::make_unique<Real>(-x_, this->space() );
  }

  double Real::applyAsDualTo(const AbstractFunctionSpaceElement& y) const
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
