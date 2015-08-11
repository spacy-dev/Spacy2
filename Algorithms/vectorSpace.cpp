#include "vectorSpace.hh"

#include "Interface/abstractVectorSpace.hh"
#include "vector.hh"

#include <utility>

namespace Algorithm
{
  VectorSpace::VectorSpace(std::shared_ptr<Interface::AbstractVectorSpace> implementation)
    : Mixin::SharedImpl<Interface::AbstractVectorSpace>(implementation)
  {
    if( impl().isHilbertSpace() )
      impl().setDualSpace(sharedImpl());
  }

  void VectorSpace::setNorm(const Norm& norm)
  {
    impl().setNorm(norm.sharedImpl());
  }

  Norm VectorSpace::norm() const
  {
    return Norm( impl().sharedNorm() );
  }

  Vector VectorSpace::element() const
  {
    return Vector(impl().element());
  }

  unsigned VectorSpace::index() const
  {
    return impl().index();
  }

  void VectorSpace::setScalarProduct(const ScalarProduct& sp)
  {
    impl().setScalarProduct( sp.sharedImpl() );
  }

  void VectorSpace::setScalarProduct(ScalarProduct&& sp)
  {
    impl().setScalarProduct( std::move( sp.sharedImpl() ) );
  }

  ScalarProduct VectorSpace::scalarProduct() const
  {
    return ScalarProduct( impl().scalarProduct() );
  }


  void connectPrimalDual(VectorSpace& primalSpace, VectorSpace& dualSpace)
  {
    Interface::makePrimalDual( primalSpace.impl() , dualSpace.impl() );
  }
}
