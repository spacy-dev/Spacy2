#include "functionSpace.hh"

#include "Interface/abstractFunctionSpace.hh"
#include "functionSpaceElement.hh"

#include <utility>

namespace Algorithm
{
  FunctionSpace::FunctionSpace(std::shared_ptr<Interface::AbstractFunctionSpace> implementation)
    : Mixin::SharedImpl<Interface::AbstractFunctionSpace>(implementation)
  {
    if( impl().isHilbertSpace() )
      impl().setDualSpace(sharedImpl());
  }

  void FunctionSpace::setNorm(const Norm& norm)
  {
    impl().setNorm(norm.sharedImpl());
  }

  Norm FunctionSpace::norm() const
  {
    return Norm( impl().sharedNorm() );
  }

  FunctionSpaceElement FunctionSpace::element() const
  {
    return FunctionSpaceElement(impl().element());
  }

  unsigned FunctionSpace::index() const
  {
    return impl().index();
  }

  void FunctionSpace::setScalarProduct(const ScalarProduct& sp)
  {
    impl().setScalarProduct( sp.sharedImpl() );
  }

  void FunctionSpace::setScalarProduct(ScalarProduct&& sp)
  {
    impl().setScalarProduct( std::move( sp.sharedImpl() ) );
  }

  ScalarProduct FunctionSpace::scalarProduct() const
  {
    return ScalarProduct( impl().scalarProduct() );
  }


  void connectPrimalDual(FunctionSpace& primalSpace, FunctionSpace& dualSpace)
  {
    Interface::makePrimalDual( primalSpace.impl() , dualSpace.impl() );
  }
}
