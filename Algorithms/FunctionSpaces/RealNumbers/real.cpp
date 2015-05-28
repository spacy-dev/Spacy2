#include "real.hh"

#include "../../Util/invalidargumentexception.hh"

#include <iostream>
#include <stdexcept>

namespace Algorithm
{
  Real::Real(double x, const AbstractHilbertSpace &space)
    : AbstractFunctionSpaceElement(space), x_(1,x)
  {}

  Real::Real(const AbstractHilbertSpace &space)
    : Real(0.,space)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> Real::clone() const
  {
    return std::make_unique<Real>(x_[0],dynamic_cast<const AbstractHilbertSpace&>(this->space_) );
  }

  Real& Real::operator+=(const AbstractFunctionSpaceElement& y)
  {
    if( dynamic_cast<const Real*>(&y) == nullptr ) throw InvalidArgumentException("Real::operator+=");
    x_[0] += dynamic_cast<const Real&>(y).x_[0];
    return *this;
  }

  Real& Real::operator-=(const AbstractFunctionSpaceElement& y)
  {
    if( dynamic_cast<const Real*>(&y) == nullptr ) throw InvalidArgumentException("Real::operator-=");
    x_[0] -= dynamic_cast<const Real&>(y).x_[0];
    return *this;
  }

  Real& Real::operator*=(double a)
  {
    x_[0] *= a;
    return *this;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> Real::operator- () const
  {
    return std::make_unique<Real>(-x_[0],dynamic_cast<const AbstractHilbertSpace&>(this->space_) );
  }

  unsigned Real::size() const
  {
    return 1;
  }


  double& Real::coefficient(unsigned i)
  {
    if(i > 0) throw std::out_of_range("In Real::coefficient(" + std::to_string(i) + ").");
    return x_[i];
  }

  const double& Real::coefficient(unsigned i) const
  {
    if(i > 0) throw std::out_of_range("In Real::coefficient(" + std::to_string(i) + ").");
    return x_[i];
  }

  void Real::print(std::ostream& os) const
  {
    os << "Space index: " << this->space_.index() << "\n";
    os << x_[0] << std::endl;
  }
}
