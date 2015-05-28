#include "primalProductSpaceElement.hh"

#include "productSpace.hh"

#include "productSpaceElement.hh"
#include "dualProductSpaceElement.hh"

#include "../../Interface/abstractFunctionSpaceElement.hh"
#include "../../Util/invalidargumentexception.hh"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace Algorithm
{
  ProductSpaceElement_PrimalReference::ProductSpaceElement_PrimalReference(ProductSpaceElement& element)
    : AbstractFunctionSpaceElement(element.getSpace()),
      productSpaceElement_(element)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement_PrimalReference::clone() const
  {
    auto dualVariables = cloneVariables(productSpaceElement_.dualVariables_);
    for(auto& var : dualVariables) *var *= 0.;
    return std::make_unique< ProductSpaceElement >( productSpaceElement_.primalVariables_, dualVariables , productSpaceElement_.getSpace() );
  }

 ProductSpaceElement& ProductSpaceElement_PrimalReference::operator+=(const AbstractFunctionSpaceElement& y)
  {
    if( !isAnyProductSpaceElement(y) ) throw InvalidArgumentException("PrimalProductSpaceElement::operator+=");

    if( !isDualProductSpaceElement(y) )
      for(auto i = 0u; i < productSpaceElement_.primalVariables_.size(); ++i)
        *productSpaceElement_.primalVariables_[i] += primalVariable(y,i);

    return productSpaceElement_;
  }

  ProductSpaceElement& ProductSpaceElement_PrimalReference::operator-=(const AbstractFunctionSpaceElement& y)
  {
    if( !isAnyProductSpaceElement(y) ) throw InvalidArgumentException("PrimalProductSpaceElement::operator+=");

    if( !isDualProductSpaceElement(y) )
      for(auto i = 0u; i < productSpaceElement_.primalVariables_.size(); ++i)
        *productSpaceElement_.primalVariables_[i] -= primalVariable(y,i);
    return productSpaceElement_;
  }

  ProductSpaceElement& ProductSpaceElement_PrimalReference::operator*=(double a)
  {
    for(auto i = 0u; i < productSpaceElement_.primalVariables_.size(); ++i) *productSpaceElement_.primalVariables_[i] *= a;

    return productSpaceElement_;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement_PrimalReference::operator- () const
  {
    std::decay_t<decltype(productSpaceElement_.primalVariables_)> primal;
    for(auto& var : productSpaceElement_.primalVariables_) primal.push_back( -*var );
    return std::make_unique<ProductSpaceElement>(primal,primal,this->space_);
  }

  unsigned ProductSpaceElement_PrimalReference::size() const
  {
    return primalSize();
  }

  unsigned ProductSpaceElement_PrimalReference::primalSize() const
  {
    return std::accumulate( begin(productSpaceElement_.primalVariables_) , end(productSpaceElement_.primalVariables_) , 0.0,
                            [](auto init, const auto& variable) { return init += variable->size(); });
  }

  double& ProductSpaceElement_PrimalReference::coefficient(unsigned i)
  {
    if( i > size() ) throw std::out_of_range("In PrimalProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    unsigned j = i;
    unsigned elementId = 0;
    while ( j >= productSpaceElement_.primalVariables_[elementId]->size() )
    {
      j -= productSpaceElement_.primalVariables_[elementId]->size();
      ++elementId;
    }

    return productSpaceElement_.primalVariables_[elementId]->coefficient(j);
  }

  const double& ProductSpaceElement_PrimalReference::coefficient(unsigned i) const
  {
    if( i > size() ) throw std::out_of_range("In PrimalProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    unsigned j = i;
    unsigned elementId = 0;
    while ( j >= productSpaceElement_.primalVariables_[elementId]->size() )
    {
      j -= productSpaceElement_.primalVariables_[elementId]->size();
      ++elementId;
    }

    return productSpaceElement_.primalVariables_[elementId]->coefficient(j);
  }

  void ProductSpaceElement_PrimalReference::print(std::ostream& os) const
  {
    os << "Space index: " << this->space_.index() << "\n";
    os << "Primal variables:\n";
    for( auto& v : productSpaceElement_.primalVariables_ ) v->print(os);
  }


  ProductSpaceElement_PrimalConstReference::ProductSpaceElement_PrimalConstReference(const ProductSpaceElement& element)
    : AbstractFunctionSpaceElement(element.getSpace()),
      productSpaceElement_(element)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement_PrimalConstReference::clone() const
  {
    return std::make_unique< ProductSpaceElement_PrimalConstReference >( productSpaceElement_ );
  }

 ProductSpaceElement_PrimalConstReference& ProductSpaceElement_PrimalConstReference::operator+=(const AbstractFunctionSpaceElement& y)
  {
    if( !isAnyProductSpaceElement(y) ) throw InvalidArgumentException("PrimalProductSpaceElement::operator+=");

    for(auto i = 0u; i < productSpaceElement_.primalVariables_.size(); ++i)
      *productSpaceElement_.primalVariables_[i] += primalVariable(y,i);

    return *this;
  }

  ProductSpaceElement_PrimalConstReference& ProductSpaceElement_PrimalConstReference::operator-=(const AbstractFunctionSpaceElement& y)
  {
    if( !isAnyProductSpaceElement(y) ) throw InvalidArgumentException("PrimalProductSpaceElement::operator+=");

    for(auto i = 0u; i < productSpaceElement_.primalVariables_.size(); ++i) *productSpaceElement_.primalVariables_[i] -= primalVariable(y,i);
    return *this;
  }

  ProductSpaceElement_PrimalConstReference& ProductSpaceElement_PrimalConstReference::operator*=(double a)
  {
    for(auto i = 0u; i < productSpaceElement_.primalVariables_.size(); ++i) *productSpaceElement_.primalVariables_[i] *= a;

    return *this;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement_PrimalConstReference::operator- () const
  {
    std::decay_t<decltype(productSpaceElement_.primalVariables_)> primal;
    for(auto& var : productSpaceElement_.primalVariables_) primal.push_back( -*var );
    return std::make_unique<ProductSpaceElement>(primal,primal,this->space_);
  }

  unsigned ProductSpaceElement_PrimalConstReference::size() const
  {
    return primalSize();
  }

  unsigned ProductSpaceElement_PrimalConstReference::primalSize() const
  {
    return std::accumulate( begin(productSpaceElement_.primalVariables_) , end(productSpaceElement_.primalVariables_) , 0.0,
                            [](auto init, const auto& variable) { return init += variable->size(); });
  }

  double& ProductSpaceElement_PrimalConstReference::coefficient(unsigned i)
  {
    if( i > size() ) throw std::out_of_range("In PrimalProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    unsigned j = i;
    unsigned elementId = 0;
    while ( j >= productSpaceElement_.primalVariables_[elementId]->size() )
    {
      j -= productSpaceElement_.primalVariables_[elementId]->size();
      ++elementId;
    }

    return productSpaceElement_.primalVariables_[elementId]->coefficient(j);
  }

  const double& ProductSpaceElement_PrimalConstReference::coefficient(unsigned i) const
  {
    if( i > size() ) throw std::out_of_range("In PrimalProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    unsigned j = i;
    unsigned elementId = 0;
    while ( j >= productSpaceElement_.primalVariables_[elementId]->size() )
    {
      j -= productSpaceElement_.primalVariables_[elementId]->size();
      ++elementId;
    }

    return productSpaceElement_.primalVariables_[elementId]->coefficient(j);
  }

  void ProductSpaceElement_PrimalConstReference::print(std::ostream& os) const
  {
    os << "Space index: " << this->space_.index() << "\n";
    os << "Primal variables:\n";
    for( auto& v : productSpaceElement_.primalVariables_ ) v->print(os);
  }

  bool isPrimalProductSpaceElement(const AbstractFunctionSpaceElement &x)
  {
    return isProductSpaceElement_PrimalConstReference(x) || isProductSpaceElement_PrimalReference(x);
  }

  bool isProductSpaceElement_PrimalReference(const AbstractFunctionSpaceElement& x)
  {
    return dynamic_cast<const ProductSpaceElement_PrimalReference*>(&x) != nullptr;
  }

  bool isProductSpaceElement_PrimalConstReference(const AbstractFunctionSpaceElement& x)
  {
    return dynamic_cast<const ProductSpaceElement_PrimalConstReference*>(&x) != nullptr;
  }
}
