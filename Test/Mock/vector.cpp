// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"
#include "Spacy/Spaces/realSpace.hh"

#include "vector.hh"

namespace Mock
{
  Vector::Vector(const Spacy::VectorSpace& space)
    : space_(&space)
  {}


  Vector& Vector::operator+=(const Vector& y )
  {
    value_ += value(y);
    return *this;
  }

  Vector& Vector::operator-=(const Vector& y)
  {
    value_ -= value(y);
    return *this;
  }

  Vector& Vector::operator*=(double a)
  {
    value_ *= a;
    return *this;
  }

  Vector Vector::operator-() const
  {
    auto y = Vector(*this);
    return y *= -1;
  }

  bool Vector::operator==(const Vector y) const
  {
    return value(*this) == value(y);
  }

  Spacy::Real Vector::operator()(const Vector& y) const
  {
    auto z = y;
    return value( z *= value(*this) );
  }

  const Spacy::VectorSpace& Vector::space() const
  {
    assert( space_ != nullptr );
    return *space_;
  }

  void Vector::toFile(const std::string&) const
  {}
}
