#include "vector.hh"

#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Exceptions/incompatibleSpaceException.hh"

#include "Spacy/vectorSpace.hh"


namespace Spacy
{
  // free functions
  Vector operator+(Vector x, const Vector& y)
  {
    return x += y;
  }

  Vector operator-(Vector x, const Vector& y)
  {
    return x -= y;
  }

  Real operator*(const Vector& x, const Vector& y)
  {
    return x.space()->scalarProduct()(x,y);
  }

  Real norm(const Vector& x)
  {
    return x.space()->norm()(x);
  }

  void checkDualPairing(const Vector& x, const Vector& y)
  {
    if( !y.space()->isPrimalWRT(*x.space()) )
      throw IncompatibleSpaceException(x.space()->index(),y.space()->index());
  }
}
