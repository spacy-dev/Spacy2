#include "productSpaceElement.hh"

#include "productSpace.hh"

#include "../../Util/invalidargumentexception.hh"

#include <numeric>
#include <stdexcept>

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
    auto primalVars = cloneVariables(primalVariables_);
    if(disablePrimal_) for(auto& var : primalVars) *var *= 0;
    auto dualVars = cloneVariables(dualVariables_);
    if(disableDual_) for(auto& var : dualVars) *var *= 0;
    reset();
    return std::make_unique<ProductSpaceElement>(primalVars,dualVars,this->getSpace());
  }

  void ProductSpaceElement::copyTo(AbstractFunctionSpaceElement& y) const
  {
    if( !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::copyTo");

    ProductSpaceElement& y_ = dynamic_cast<ProductSpaceElement&>(y);

    if( !disablePrimal_ && !y_.disablePrimal_ )
      y_.primalVariables_ = cloneVariables(primalVariables_);

    if( !disableDual_ && !y_.disableDual_ )
      y_.dualVariables_ = cloneVariables(dualVariables_);

    reset();
    y_.reset();
  }

  ProductSpaceElement& ProductSpaceElement::operator+=(const AbstractFunctionSpaceElement& y)
  {
    if( !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::operator+=");

    const ProductSpaceElement& y_ = dynamic_cast<const ProductSpaceElement&>(y);

    if(!disablePrimal_ && !y_.disablePrimal_)
      for(auto i = 0u; i < primalVariables_.size(); ++i)
        *primalVariables_[i] += *y_.primalVariables_[i];

    if(!disableDual_ && !y_.disableDual_)
      for(auto i = 0u; i < dualVariables_.size(); ++i)
        *dualVariables_[i] += *y_.dualVariables_[i];

    reset();
    y_.reset();
    return *this;
  }

  ProductSpaceElement& ProductSpaceElement::operator-=(const AbstractFunctionSpaceElement& y)
  {
    if( !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::operator-=");

    const ProductSpaceElement& y_ = dynamic_cast<const ProductSpaceElement&>(y);

    if(!disablePrimal_ && !y_.disablePrimal_)
      for(auto i = 0u; i < primalVariables_.size(); ++i)
        (*primalVariables_[i]) -= *y_.primalVariables_[i];

    if(!disableDual_ && !y_.disableDual_)
      for(auto i = 0u; i < dualVariables_.size(); ++i)
        (*dualVariables_[i]) -= *y_.dualVariables_[i];

    reset();
    y_.reset();
    return *this;
  }

  ProductSpaceElement& ProductSpaceElement::operator*=(double a)
  {
    if(!disablePrimal_)
      for(auto i = 0u; i < primalVariables_.size(); ++i)
        *primalVariables_[i] *= a;

    if(!disableDual_)
      for(auto i = 0u; i < dualVariables_.size(); ++i)
        *dualVariables_[i] *= a;

    reset();
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

  bool isProductSpaceElement(const AbstractFunctionSpaceElement& x)
  {
    return dynamic_cast<const ProductSpaceElement*>(&x) != nullptr;
  }


  FunctionSpaceElement& primal(FunctionSpaceElement &x)
  {
    dynamic_cast<ProductSpaceElement&>(x.impl()).disableDual();
    return x;
  }

  const FunctionSpaceElement& primal(const FunctionSpaceElement &x)
  {
    dynamic_cast<const ProductSpaceElement&>(x.impl()).disableDual();
    return x;
  }

  FunctionSpaceElement& dual(FunctionSpaceElement &x)
  {
    dynamic_cast<ProductSpaceElement&>(x.impl()).disablePrimal();
    return x;
  }

  const FunctionSpaceElement& dual(const FunctionSpaceElement &x)
  {
    dynamic_cast<const ProductSpaceElement&>(x.impl()).disablePrimal();
    return x;
  }
}
