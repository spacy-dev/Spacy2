#include "dualProductSpaceElement.hh"

#include "productSpace.hh"

#include "productSpaceElement.hh"
#include "primalProductSpaceElement.hh"

#include "../../Interface/abstractFunctionSpaceElement.hh"
#include "../../Util/invalidargumentexception.hh"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace Algorithm
{
  ProductSpaceElement_DualReference::ProductSpaceElement_DualReference(ProductSpaceElement& element)
    : AbstractFunctionSpaceElement(element.getSpace()),
      productSpaceElement_(element)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement_DualReference::clone() const
  {
    auto primalVariables = cloneVariables(productSpaceElement_.primalVariables_);
    for(auto& var : primalVariables) *var *= 0.;
    return std::make_unique< ProductSpaceElement >( primalVariables , productSpaceElement_.dualVariables_, productSpaceElement_.getSpace() );
  }

 ProductSpaceElement& ProductSpaceElement_DualReference::operator+=(const AbstractFunctionSpaceElement& y)
  {
    if( !isAnyProductSpaceElement(y) ) throw InvalidArgumentException("DualProductSpaceElement::operator+=");

    if( !isPrimalProductSpaceElement(y) )
      for(auto i = 0u; i < productSpaceElement_.dualVariables_.size(); ++i)
        *productSpaceElement_.dualVariables_[i] += dualVariable(y,i);

    return productSpaceElement_;
  }

  ProductSpaceElement& ProductSpaceElement_DualReference::operator-=(const AbstractFunctionSpaceElement& y)
  {
    if( !isAnyProductSpaceElement(y) ) throw InvalidArgumentException("DualProductSpaceElement::operator+=");

    if( !isPrimalProductSpaceElement(y) )
      for(auto i = 0u; i < productSpaceElement_.dualVariables_.size(); ++i)
        *productSpaceElement_.dualVariables_[i] -= dualVariable(y,i);
    return productSpaceElement_;
  }

  ProductSpaceElement& ProductSpaceElement_DualReference::operator*=(double a)
  {
    for(auto i = 0u; i < productSpaceElement_.dualVariables_.size(); ++i) *productSpaceElement_.dualVariables_[i] *= a;

    return productSpaceElement_;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement_DualReference::operator- () const
  {
    std::decay_t<decltype(productSpaceElement_.dualVariables_)> dual;
    for(auto& var : productSpaceElement_.dualVariables_) dual.push_back( -*var );
    return std::make_unique<ProductSpaceElement>(productSpaceElement_.dualVariables_,dual,this->space_);
  }

  unsigned ProductSpaceElement_DualReference::size() const
  {
    return dualSize();
  }

  unsigned ProductSpaceElement_DualReference::dualSize() const
  {
    return std::accumulate( begin(productSpaceElement_.dualVariables_) , end(productSpaceElement_.dualVariables_) , 0.0,
                            [](auto init, const auto& variable) { return init += variable->size(); });
  }

  double& ProductSpaceElement_DualReference::coefficient(unsigned i)
  {
    if( i > size() ) throw std::out_of_range("In DualProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    unsigned j = i;
    unsigned elementId = 0;
    while ( j >= productSpaceElement_.dualVariables_[elementId]->size() )
    {
      j -= productSpaceElement_.dualVariables_[elementId]->size();
      ++elementId;
    }

    return productSpaceElement_.dualVariables_[elementId]->coefficient(j);
  }

  const double& ProductSpaceElement_DualReference::coefficient(unsigned i) const
  {
    if( i > size() ) throw std::out_of_range("In DualProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    unsigned j = i;
    unsigned elementId = 0;
    while ( j >= productSpaceElement_.dualVariables_[elementId]->size() )
    {
      j -= productSpaceElement_.dualVariables_[elementId]->size();
      ++elementId;
    }

    return productSpaceElement_.dualVariables_[elementId]->coefficient(j);
  }

  void ProductSpaceElement_DualReference::print(std::ostream& os) const
  {
    os << "Space index: " << this->space_.index() << "\n";
    os << "Dual variables:\n";
    for( auto& v : productSpaceElement_.dualVariables_ ) v->print(os);
  }


  ProductSpaceElement_DualConstReference::ProductSpaceElement_DualConstReference(const ProductSpaceElement& element)
    : AbstractFunctionSpaceElement(element.getSpace()),
      productSpaceElement_(element)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement_DualConstReference::clone() const
  {
    return std::make_unique< ProductSpaceElement_DualConstReference >( productSpaceElement_ );
  }

 ProductSpaceElement_DualConstReference& ProductSpaceElement_DualConstReference::operator+=(const AbstractFunctionSpaceElement& y)
  {
    if( !isAnyProductSpaceElement(y) ) throw InvalidArgumentException("DualProductSpaceElement::operator+=");

    for(auto i = 0u; i < productSpaceElement_.dualVariables_.size(); ++i)
      *productSpaceElement_.dualVariables_[i] += dualVariable(y,i);

    return *this;
  }

  ProductSpaceElement_DualConstReference& ProductSpaceElement_DualConstReference::operator-=(const AbstractFunctionSpaceElement& y)
  {
    if( !isAnyProductSpaceElement(y) ) throw InvalidArgumentException("DualProductSpaceElement::operator+=");

    for(auto i = 0u; i < productSpaceElement_.dualVariables_.size(); ++i) *productSpaceElement_.dualVariables_[i] -= dualVariable(y,i);
    return *this;
  }

  ProductSpaceElement_DualConstReference& ProductSpaceElement_DualConstReference::operator*=(double a)
  {
    for(auto i = 0u; i < productSpaceElement_.dualVariables_.size(); ++i) *productSpaceElement_.dualVariables_[i] *= a;

    return *this;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement_DualConstReference::operator- () const
  {
    std::decay_t<decltype(productSpaceElement_.dualVariables_)> dual;
    for(auto& var : productSpaceElement_.dualVariables_) dual.push_back( -*var );
    return std::make_unique<ProductSpaceElement>(productSpaceElement_.primalVariables_,dual,this->space_);
  }

  unsigned ProductSpaceElement_DualConstReference::size() const
  {
    return dualSize();
  }

  unsigned ProductSpaceElement_DualConstReference::dualSize() const
  {
    return std::accumulate( begin(productSpaceElement_.dualVariables_) , end(productSpaceElement_.dualVariables_) , 0.0,
                            [](auto init, const auto& variable) { return init += variable->size(); });
  }

  double& ProductSpaceElement_DualConstReference::coefficient(unsigned i)
  {
    if( i > size() ) throw std::out_of_range("In DualProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    unsigned j = i;
    unsigned elementId = 0;
    while ( j >= productSpaceElement_.dualVariables_[elementId]->size() )
    {
      j -= productSpaceElement_.dualVariables_[elementId]->size();
      ++elementId;
    }

    return productSpaceElement_.dualVariables_[elementId]->coefficient(j);
  }

  const double& ProductSpaceElement_DualConstReference::coefficient(unsigned i) const
  {
    if( i > size() ) throw std::out_of_range("In DualProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    unsigned j = i;
    unsigned elementId = 0;
    while ( j >= productSpaceElement_.dualVariables_[elementId]->size() )
    {
      j -= productSpaceElement_.dualVariables_[elementId]->size();
      ++elementId;
    }

    return productSpaceElement_.dualVariables_[elementId]->coefficient(j);
  }

  void ProductSpaceElement_DualConstReference::print(std::ostream& os) const
  {
    os << "Space index: " << this->space_.index() << "\n";
    os << "Dual variables:\n";
    for( auto& v : productSpaceElement_.dualVariables_ ) v->print(os);
  }

  bool isDualProductSpaceElement(const AbstractFunctionSpaceElement &x)
  {
    return isProductSpaceElement_DualConstReference(x) || isProductSpaceElement_DualReference(x);
  }

  bool isProductSpaceElement_DualReference(const AbstractFunctionSpaceElement& x)
  {
    return dynamic_cast<const ProductSpaceElement_DualReference*>(&x) != nullptr;
  }

  bool isProductSpaceElement_DualConstReference(const AbstractFunctionSpaceElement& x)
  {
    return dynamic_cast<const ProductSpaceElement_DualConstReference*>(&x) != nullptr;
  }
}
