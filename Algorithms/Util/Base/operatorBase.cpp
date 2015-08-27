#include "operatorBase.hh"

#include "vectorSpace.hh"

namespace Algorithm
{
  OperatorBase::OperatorBase(const VectorSpace& domain, const VectorSpace& range)
    : domain_(domain), range_(range)
  {}

  const VectorSpace& OperatorBase::domain() const
  {
    return domain_;
  }

  const VectorSpace& OperatorBase::range() const
  {
    return range_;
  }
}
