#include "operatorBase.hh"

#include "Spacy/vectorSpace.hh"

namespace Spacy
{
  OperatorBase::OperatorBase(const VectorSpace& domain, const VectorSpace& range)
    : domain_(domain), range_(range)
  {}


  /// Move assignment.
  OperatorBase& OperatorBase::operator=(OperatorBase&& B)
  {
    checkSpaceCompatibility(range(),B.range());
    checkSpaceCompatibility(domain(),B.domain());
    return *this;
  }

  /// Move assignment.
  OperatorBase& OperatorBase::operator=(const OperatorBase& B)
  {
    checkSpaceCompatibility(range(),B.range());
    checkSpaceCompatibility(domain(),B.domain());
    return *this;
  }

  const VectorSpace& OperatorBase::domain() const
  {
    return domain_;
  }

  const VectorSpace& OperatorBase::range() const
  {
    return range_;
  }
}
