#include "productSpaceElement.hh"

#include "primalProductSpaceElement.hh"
#include "dualProductSpaceElement.hh"

#include "productSpace.hh"

#include "../../Interface/abstractFunctionSpaceElement.hh"
#include "../../Util/invalidargumentexception.hh"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace Algorithm
{
    std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > cloneVariables(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& variables)
    {
      std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > clonedVariables;

      for(const auto& var : variables) clonedVariables.emplace_back<std::unique_ptr<AbstractFunctionSpaceElement> >(var->clone());

      return clonedVariables;
    }

  ProductSpaceElement::ProductSpaceElement(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > &primalVariables,
                                           const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > &dualVariables, const AbstractBanachSpace& space)
    : AbstractFunctionSpaceElement(space),
      primalVariables_(cloneVariables(primalVariables)), dualVariables_(cloneVariables(dualVariables))
  {}

  ProductSpaceElement::ProductSpaceElement(const AbstractBanachSpace& space)
    : AbstractFunctionSpaceElement(space)
  {
    const auto& primalSpaces = dynamic_cast<const ProductSpace&>(space).getPrimalSpaces();
    for (auto i=0u; i<primalSpaces.size(); ++i) primalVariables_.push_back(primalSpaces[i]->element());

    const auto& dualSpaces = dynamic_cast<const ProductSpace&>(space).getDualSpaces();
    for (auto i=0u; i<dualSpaces.size(); ++i) dualVariables_.push_back(dualSpaces[i]->element());
  }

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement::clone() const
  {
    return std::make_unique<ProductSpaceElement>(primalVariables_,dualVariables_,this->space_);
  }

  ProductSpaceElement& ProductSpaceElement::operator+=(const AbstractFunctionSpaceElement& y)
  {
    if( !isAnyProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::operator+=");

    if(!isDualProductSpaceElement(y))
      for(auto i = 0u; i < primalVariables_.size(); ++i)
        (*primalVariables_[i]) += primalVariable(y,i);

    if(!isPrimalProductSpaceElement(y))
      for(auto i = 0u; i < dualVariables_.size(); ++i)
        (*dualVariables_[i]) += dualVariable(y,i);

    return *this;
  }

  ProductSpaceElement& ProductSpaceElement::operator-=(const AbstractFunctionSpaceElement& y)
  {
    if( !isAnyProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::operator-=");

    if(!isDualProductSpaceElement(y))
      for(auto i = 0u; i < primalVariables_.size(); ++i)
        (*primalVariables_[i]) -= primalVariable(y,i);

    if(!isPrimalProductSpaceElement(y))
      for(auto i = 0u; i < dualVariables_.size(); ++i)
        (*dualVariables_[i]) -= dualVariable(y,i);

    return *this;
  }

  ProductSpaceElement& ProductSpaceElement::operator*=(double a)
  {
    for(auto i = 0u; i < primalVariables_.size(); ++i) (*primalVariables_[i]) *= a;

    for(auto i = 0u; i < dualVariables_.size(); ++i) (*dualVariables_[i]) *= a;

    return *this;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement::operator- () const
  {
    decltype(primalVariables_) primal;
    for(auto& var : primalVariables_) primal.push_back( -*var );
    decltype(dualVariables_) dual;
    for(auto& var : dualVariables_) dual.push_back( -*var );
    return std::make_unique<ProductSpaceElement>(primal,dual,this->space_);
  }

  unsigned ProductSpaceElement::size() const
  {
    return primalSize() + dualSize();
  }

  unsigned ProductSpaceElement::primalSize() const
  {
    return std::accumulate( begin(primalVariables_) , end(primalVariables_) , 0.0,
                            [](auto init, const auto& variable) { return init += variable->size(); });
  }

  unsigned ProductSpaceElement::dualSize() const
  {
    return std::accumulate( begin(dualVariables_) , end(dualVariables_) , 0.0,
                            [](auto init, const auto& variable) { return init += variable->size(); });
  }

  double& ProductSpaceElement::coefficient(unsigned i)
  {
    if( i > size() ) throw std::out_of_range("In ProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    if( i < primalSize() )
    {
      unsigned j = i;
      unsigned elementId = 0;
      while ( j >= primalVariables_[elementId]->size() )
      {
        j -= primalVariables_[elementId]->size();
        ++elementId;
      }
      return primalVariables_[elementId]->coefficient(j);
    }

    unsigned j = i-primalSize();
    unsigned elementId = 0;
    while ( j >= dualVariables_[elementId]->size() )
    {
      j -= dualVariables_[elementId]->size();
      ++elementId;
    }
    return dualVariables_[elementId]->coefficient(j);
    // return x_[i];
  }

  const double& ProductSpaceElement::coefficient(unsigned i) const
  {
    if( i > size() ) throw std::out_of_range("In ProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    if( i < primalSize() )
    {
      unsigned j = i;
      unsigned elementId = 0;
      while ( j > primalVariables_[elementId]->size() )
      {
        j -= primalVariables_[elementId]->size();
        ++elementId;
      }
      return primalVariables_[elementId]->coefficient(j);
    }

    unsigned j = i-primalSize();
    unsigned elementId = 0;
    while ( j > dualVariables_[elementId]->size() )
    {
      j -= dualVariables_[elementId]->size();
      ++elementId;
    }
    return dualVariables_[elementId]->coefficient(j);
  }

  void ProductSpaceElement::print(std::ostream& os) const
  {
    os << "Space index: " << this->space_.index() << "\n";
    os << "Primal variables:\n";
    for( auto& v : primalVariables_ ) v->print(os);
    os << "Dual variables:\n";
    for( auto& v : dualVariables_ ) v->print(os);
  }

  bool isAnyProductSpaceElement(const AbstractFunctionSpaceElement &x)
  {
    return isProductSpaceElement(x) || isPrimalProductSpaceElement(x) || isDualProductSpaceElement(x);
  }

  bool isProductSpaceElement(const AbstractFunctionSpaceElement& x)
  {
    return dynamic_cast<const ProductSpaceElement*>(&x) != nullptr;
  }

  const AbstractFunctionSpaceElement& primalVariable(const AbstractFunctionSpaceElement& x, unsigned i)
  {
    if( isProductSpaceElement_PrimalReference(x) )  return *dynamic_cast<const ProductSpaceElement&>( dynamic_cast<const ProductSpaceElement_PrimalReference&>(x).productSpaceElement_  ).primalVariables_[i];
    if( isProductSpaceElement_PrimalConstReference(x) ) return *dynamic_cast<const ProductSpaceElement&>( dynamic_cast<const ProductSpaceElement_PrimalConstReference&>(x).productSpaceElement_ ).primalVariables_[i];
    return *dynamic_cast<const ProductSpaceElement&>(x).primalVariables_[i];
  }

  const AbstractFunctionSpaceElement& dualVariable(const AbstractFunctionSpaceElement& x, unsigned i)
  {
    if( isProductSpaceElement_DualReference(x) )  return *dynamic_cast<const ProductSpaceElement&>( dynamic_cast<const ProductSpaceElement_DualReference&>(x).productSpaceElement_  ).dualVariables_[i];
    if( isProductSpaceElement_DualConstReference(x) ) return *dynamic_cast<const ProductSpaceElement&>( dynamic_cast<const ProductSpaceElement_DualConstReference&>(x).productSpaceElement_ ).dualVariables_[i];
    return *dynamic_cast<const ProductSpaceElement&>(x).dualVariables_[i];
  }


  unsigned primalVariableSize(const AbstractFunctionSpaceElement& x)
  {
    if( isProductSpaceElement_PrimalReference(x) )  return dynamic_cast<const ProductSpaceElement&>( dynamic_cast<const ProductSpaceElement_PrimalReference&>(x).productSpaceElement_  ).primalVariables_.size();
    if( isProductSpaceElement_PrimalConstReference(x) ) return dynamic_cast<const ProductSpaceElement&>( dynamic_cast<const ProductSpaceElement_PrimalConstReference&>(x).productSpaceElement_ ).primalVariables_.size();
    return dynamic_cast<const ProductSpaceElement&>(x).primalVariables_.size();
  }

  unsigned dualVariableSize(const AbstractFunctionSpaceElement& x)
  {
    if( isProductSpaceElement_DualReference(x) )  return dynamic_cast<const ProductSpaceElement&>( dynamic_cast<const ProductSpaceElement_DualReference&>(x).productSpaceElement_  ).dualVariables_.size();
    if( isProductSpaceElement_DualConstReference(x) ) return dynamic_cast<const ProductSpaceElement&>( dynamic_cast<const ProductSpaceElement_DualConstReference&>(x).productSpaceElement_ ).dualVariables_.size();
    return dynamic_cast<const ProductSpaceElement&>(x).dualVariables_.size();
  }

  FunctionSpaceElement primal(FunctionSpaceElement& x)
  {
    return FunctionSpaceElement( std::make_unique< ProductSpaceElement_PrimalReference >(dynamic_cast<ProductSpaceElement&>(x.impl())) );
  }

  FunctionSpaceElement primal(const FunctionSpaceElement& x)
  {
    return FunctionSpaceElement( std::make_unique< ProductSpaceElement_PrimalConstReference >(dynamic_cast<const ProductSpaceElement&>(x.impl())) );
  }

  FunctionSpaceElement dual(FunctionSpaceElement& x)
  {
    return FunctionSpaceElement( std::make_unique< ProductSpaceElement_DualReference >(dynamic_cast<ProductSpaceElement&>(x.impl())) );
  }

  FunctionSpaceElement dual(const FunctionSpaceElement& x)
  {
    return FunctionSpaceElement( std::make_unique< ProductSpaceElement_DualConstReference >(dynamic_cast<const ProductSpaceElement&>(x.impl())) );
  }
}
