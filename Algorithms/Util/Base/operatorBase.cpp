#include "operatorBase.hh"

#include "vectorSpace.hh"

namespace Algorithm
{
  OperatorBase::OperatorBase(VectorSpace* domain, VectorSpace* range)
    : domain_(domain), range_(range)
  {}

  OperatorBase::OperatorBase(VectorSpace& domain, VectorSpace& range)
    : OperatorBase(&domain,&range)
  {}

  VectorSpace& OperatorBase::domain()
  {
    return *domain_;
  }

  const VectorSpace& OperatorBase::domain() const
  {
    return *domain_;
  }

  VectorSpace& OperatorBase::range()
  {
    return *range_;
  }

  const VectorSpace& OperatorBase::range() const
  {
    return *range_;
  }

  VectorSpace* OperatorBase::domain_ptr() const
  {
    return domain_;
  }

  VectorSpace*  OperatorBase::range_ptr() const
  {
    return range_;
  }
}
