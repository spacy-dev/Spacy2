#include "real.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Util/invalidargumentexception.hh"

#include <iostream>
#include <stdexcept>

namespace Algorithm
{
  Real::Real(double x, const AbstractBanachSpace &space)
    : AbstractFunctionSpaceElement(space), x_(x)
  {}

  Real::Real(const AbstractBanachSpace &space)
    : Real(0.,space)
  {}

  void Real::copyTo(AbstractFunctionSpaceElement& y) const
  {
    if( !isRealElement(y) ) throw InvalidArgumentException("Real::copyTo");

    dynamic_cast<Real&>(y).x_ = x_;
  }

  Real& Real::operator=(const AbstractFunctionSpaceElement& y)
  {
    if( !isRealElement(y) ) throw InvalidArgumentException("Real::operator=");
    x_ = dynamic_cast<const Real&>(y).x_;
    return *this;
  }

  Real& Real::operator+=(const AbstractFunctionSpaceElement& y)
  {
    if( !isRealElement(y) ) throw InvalidArgumentException("Real::operator+=");
    x_ += dynamic_cast<const Real&>(y).x_;
    return *this;
  }

  Real& Real::operator-=(const AbstractFunctionSpaceElement& y)
  {
    if( !isRealElement(y) ) throw InvalidArgumentException("Real::operator-=");
    x_ -= dynamic_cast<const Real&>(y).x_;
    return *this;
  }

  Real& Real::operator*=(double a)
  {
    x_ *= a;
    return *this;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> Real::operator- () const
  {
    return std::make_unique<Real>(-x_, this->getSpace() );
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
    os << "Space index: " << this->space_.index() << "\n";
    os << x_ << std::endl;
  }

  Real* Real::cloneImpl() const
  {
    return new Real(*this);
  }


  bool isRealElement(const AbstractFunctionSpaceElement& x)
  {
    return dynamic_cast<const Real*>(&x) != nullptr;
  }
}
