#include "primalDualProductSpaceElement.hh"

#include "primalDualProductSpace.hh"

#include "Util/invalidargumentexception.hh"

#include <numeric>
#include <stdexcept>
#include <type_traits>

namespace Algorithm
{
  PrimalDualProductSpaceElement::PrimalDualProductSpaceElement(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& primalVariables, const AbstractBanachSpace& space)
    : AbstractFunctionSpaceElement(space),
      primalProductSpaceElement_(primalVariables,dynamic_cast<const PrimalDualProductSpace&>(space).getPrimalProductSpace()),
      dualProductSpaceElement_({},dynamic_cast<const PrimalDualProductSpace&>(space).getDualProductSpace())
  {}


  PrimalDualProductSpaceElement::PrimalDualProductSpaceElement(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > &primalVariables,
                                           const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > &dualVariables, const AbstractBanachSpace& space)
    : AbstractFunctionSpaceElement(space),
      primalProductSpaceElement_(primalVariables,dynamic_cast<const PrimalDualProductSpace&>(space).getPrimalProductSpace()),
      dualProductSpaceElement_(dualVariables,dynamic_cast<const PrimalDualProductSpace&>(space).getDualProductSpace())
  {}

  PrimalDualProductSpaceElement::PrimalDualProductSpaceElement(const PrimalDualProductSpace &space)
    : AbstractFunctionSpaceElement(space), primalProductSpaceElement_(space.getPrimalProductSpace()), dualProductSpaceElement_(space.getDualProductSpace())
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> PrimalDualProductSpaceElement::clone() const
  {
    auto primalVars = cloneVariables(primalProductSpaceElement_.variables());
    if(disablePrimal_) for(auto& var : primalVars) *var *= 0;
    auto dualVars = cloneVariables(dualProductSpaceElement_.variables());
    if(disableDual_) for(auto& var : dualVars) *var *= 0;
    reset();
    return std::make_unique<PrimalDualProductSpaceElement>(primalVars,dualVars,this->getSpace());
  }

  void PrimalDualProductSpaceElement::copyTo(AbstractFunctionSpaceElement& y) const
  {
    if( !isPrimalDualProductSpaceElement(y) ) throw InvalidArgumentException("PrimalDualProductSpaceElement::copyTo");

    PrimalDualProductSpaceElement& y_ = dynamic_cast<PrimalDualProductSpaceElement&>(y);

    if( !disablePrimal_ && !y_.disablePrimal_ )
      y_.primalProductSpaceElement_.variables() = cloneVariables(primalProductSpaceElement_.variables());

    if( !disableDual_ && !y_.disableDual_ )
      y_.dualProductSpaceElement_.variables() = cloneVariables(dualProductSpaceElement_.variables());

    reset();
    y_.reset();
  }

  PrimalDualProductSpaceElement& PrimalDualProductSpaceElement::operator+=(const AbstractFunctionSpaceElement& y)
  {
    if( !isPrimalDualProductSpaceElement(y) ) throw InvalidArgumentException("PrimalDualProductSpaceElement::operator+=");

    const PrimalDualProductSpaceElement& y_ = dynamic_cast<const PrimalDualProductSpaceElement&>(y);

    if(!disablePrimal_ && !y_.disablePrimal_)
      for(auto i = 0u; i < primalProductSpaceElement_.variables().size(); ++i)
        *primalProductSpaceElement_.variables()[i] += *y_.primalProductSpaceElement_.variables()[i];

    if(!disableDual_ && !y_.disableDual_)
      for(auto i = 0u; i < dualProductSpaceElement_.variables().size(); ++i)
        *dualProductSpaceElement_.variables()[i] += *y_.dualProductSpaceElement_.variables()[i];

    reset();
    y_.reset();
    return *this;
  }

  PrimalDualProductSpaceElement& PrimalDualProductSpaceElement::operator-=(const AbstractFunctionSpaceElement& y)
  {
    if( !isPrimalDualProductSpaceElement(y) ) throw InvalidArgumentException("PrimalDualProductSpaceElement::operator-=");

    const PrimalDualProductSpaceElement& y_ = dynamic_cast<const PrimalDualProductSpaceElement&>(y);

    if(!disablePrimal_ && !y_.disablePrimal_)
      for(auto i = 0u; i < primalProductSpaceElement_.variables().size(); ++i)
        (*primalProductSpaceElement_.variables()[i]) -= *y_.primalProductSpaceElement_.variables()[i];

    if(!disableDual_ && !y_.disableDual_)
      for(auto i = 0u; i < dualProductSpaceElement_.variables().size(); ++i)
        (*dualProductSpaceElement_.variables()[i]) -= *y_.dualProductSpaceElement_.variables()[i];

    reset();
    y_.reset();
    return *this;
  }

  PrimalDualProductSpaceElement& PrimalDualProductSpaceElement::operator*=(double a)
  {
    if(!disablePrimal_)
      for(auto i = 0u; i < primalProductSpaceElement_.variables().size(); ++i)
        *primalProductSpaceElement_.variables()[i] *= a;

    if(!disableDual_)
      for(auto i = 0u; i < dualProductSpaceElement_.variables().size(); ++i)
        *dualProductSpaceElement_.variables()[i] *= a;

    reset();
    return *this;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> PrimalDualProductSpaceElement::operator- () const
  {
    std::decay_t<decltype(primalProductSpaceElement_.variables())> primal;
    for(auto& var : primalProductSpaceElement_.variables()) primal.push_back( -*var );
    std::decay_t<decltype(dualProductSpaceElement_.variables())> dual;
    for(auto& var : dualProductSpaceElement_.variables()) dual.push_back( -*var );
    return std::make_unique<PrimalDualProductSpaceElement>(primal,dual,this->space_);
  }

  unsigned PrimalDualProductSpaceElement::size() const
  {
    return primalVariableSize() + dualVariableSize();
  }

  unsigned PrimalDualProductSpaceElement::primalVariableSize() const
  {
    return primalProductSpaceElement_.size();
  }

  unsigned PrimalDualProductSpaceElement::dualVariableSize() const
  {
    return dualProductSpaceElement_.size();
  }

  double& PrimalDualProductSpaceElement::coefficient(unsigned i)
  {
    if( i > size() ) throw std::out_of_range("In PrimalDualProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    if( !disablePrimal_ && i < primalVariableSize() )
    {
      unsigned j = i;
      unsigned elementId = 0;
      while ( j >= primalProductSpaceElement_.variables()[elementId]->size() )
      {
        j -= primalProductSpaceElement_.variables()[elementId]->size();
        ++elementId;
      }
      return primalProductSpaceElement_.variables()[elementId]->coefficient(j);
    }

    unsigned j =  disablePrimal_ ? i : i-primalVariableSize();
    unsigned elementId = 0;
    while ( j >= dualProductSpaceElement_.variables()[elementId]->size() )
    {
      j -= dualProductSpaceElement_.variables()[elementId]->size();
      ++elementId;
    }
    return dualProductSpaceElement_.variables()[elementId]->coefficient(j);
  }

  const double& PrimalDualProductSpaceElement::coefficient(unsigned i) const
  {
    if( i > size() ) throw std::out_of_range("In PrimalDualProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    if( i < primalVariableSize() )
    {
      unsigned j = i;
      unsigned elementId = 0;
      while ( j > primalProductSpaceElement_.variables()[elementId]->size() )
      {
        j -= primalProductSpaceElement_.variables()[elementId]->size();
        ++elementId;
      }
      return primalProductSpaceElement_.variables()[elementId]->coefficient(j);
    }

    unsigned j = i-primalVariableSize();
    unsigned elementId = 0;
    while ( j > dualProductSpaceElement_.variables()[elementId]->size() )
    {
      j -= dualProductSpaceElement_.variables()[elementId]->size();
      ++elementId;
    }
    return dualProductSpaceElement_.variables()[elementId]->coefficient(j);
  }

  void PrimalDualProductSpaceElement::print(std::ostream& os) const
  {
    os << "Space index: " << this->space_.index() << "\n";
    os << "Primal variables:\n";
    for( auto& v : primalProductSpaceElement_.variables() ) v->print(os);
    os << "Dual variables:\n";
    for( auto& v : dualProductSpaceElement_.variables() ) v->print(os);
  }

  void PrimalDualProductSpaceElement::reset() const
  {
    disablePrimal_ = disableDual_ = false;
  }

  void PrimalDualProductSpaceElement::disablePrimal() const
  {
    disablePrimal_ = true;
  }

  void PrimalDualProductSpaceElement::disableDual() const
  {
    disableDual_ = true;
  }

  bool PrimalDualProductSpaceElement::isPrimalEnabled() const
  {
    return !disablePrimal_;
  }

  bool PrimalDualProductSpaceElement::isDualEnabled() const
  {
    return !disableDual_;
  }

  bool isPrimalDualProductSpaceElement(const AbstractFunctionSpaceElement& x)
  {
    return dynamic_cast<const PrimalDualProductSpaceElement*>(&x) != nullptr;
  }


  FunctionSpaceElement& primal(FunctionSpaceElement &x)
  {
    dynamic_cast<PrimalDualProductSpaceElement&>(x.impl()).disableDual();
    return x;
  }

  const FunctionSpaceElement& primal(const FunctionSpaceElement &x)
  {
    dynamic_cast<const PrimalDualProductSpaceElement&>(x.impl()).disableDual();
    return x;
  }

  FunctionSpaceElement& dual(FunctionSpaceElement &x)
  {
    dynamic_cast<PrimalDualProductSpaceElement&>(x.impl()).disablePrimal();
    return x;
  }

  const FunctionSpaceElement& dual(const FunctionSpaceElement &x)
  {
    dynamic_cast<const PrimalDualProductSpaceElement&>(x.impl()).disablePrimal();
    return x;
  }
}
