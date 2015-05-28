#include "functionSpace.hh"

#include "../Interface/abstractHilbertSpace.hh"

#include <utility>

namespace Algorithm
{
  FunctionSpace::FunctionSpace(std::unique_ptr<AbstractHilbertSpace>&& implementation)
    : impl_(std::move(implementation)), norm_(impl_->getNorm()), sp_(impl_->getScalarProduct())
  {}

//  FunctionSpace::FunctionSpace(const AbstractHilbertSpace& implementation)
//    : FunctionSpace(implementation.clone())
//  {}

  void FunctionSpace::setScalarProduct(const ScalarProduct& sp)
  {
    sp_ = sp;
  }

  const ScalarProduct& FunctionSpace::getScalarProduct() const
  {
    return sp_;
  }

  const Norm& FunctionSpace::getNorm() const
  {
    return norm_;
  }

  FunctionSpaceElement FunctionSpace::element() const
  {
    return FunctionSpaceElement(impl_->element());
  }

  unsigned FunctionSpace::index() const
  {
    return impl_->index();
  }

  AbstractHilbertSpace const& FunctionSpace::impl() const
  {
    return *impl_;
  }
}
