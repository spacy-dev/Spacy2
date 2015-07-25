#include "productSpaceElement.hh"

#include "productSpace.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

#include <cassert>
#include <numeric>
#include <stdexcept>
#include <type_traits>

#include <iostream>

namespace Algorithm
{
  using Interface::AbstractBanachSpace;
  using Interface::AbstractFunctionSpaceElement;

  namespace Fenics
  {
    ProductSpaceElement::ProductSpaceElement(const dolfin::Function& v, const Interface::AbstractBanachSpace& space)
      : AbstractFunctionSpaceElement(space),
        Mixin::Impl<dolfin::Function>(v)
    {}

    void ProductSpaceElement::copyTo(AbstractFunctionSpaceElement& y) const
    {
      if( !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::copyTo");

      ProductSpaceElement& y_ = dynamic_cast<ProductSpaceElement&>(y);

      if( !disablePrimal_ && !y_.disablePrimal_ )
        y_.primalProductSpaceElement_.variables() = cloneVariables(primalProductSpaceElement_.variables());

      if( !disableDual_ && !y_.disableDual_ )
        y_.dualProductSpaceElement_.variables() = cloneVariables(dualProductSpaceElement_.variables());

      reset();
      y_.reset();
    }

    ProductSpaceElement& ProductSpaceElement::operator=(const AbstractFunctionSpaceElement& y)
    {
      if( !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::operator+=");

      const ProductSpaceElement& y_ = dynamic_cast<const ProductSpaceElement&>(y);

      if(!disablePrimal_ && !y_.disablePrimal_)
        for(auto i = 0u; i < primalProductSpaceElement_.variables().size(); ++i)
          *primalProductSpaceElement_.variables()[i] = *y_.primalProductSpaceElement_.variables()[i];

      if(!disableDual_ && !y_.disableDual_)
        for(auto i = 0u; i < dualProductSpaceElement_.variables().size(); ++i)
          *dualProductSpaceElement_.variables()[i] = *y_.dualProductSpaceElement_.variables()[i];

      reset();
      y_.reset();
      return *this;
    }

    ProductSpaceElement& ProductSpaceElement::operator+=(const AbstractFunctionSpaceElement& y)
    {
      if( !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::operator+=");

      const ProductSpaceElement& y_ = dynamic_cast<const ProductSpaceElement&>(y);

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

    ProductSpaceElement& ProductSpaceElement::operator-=(const AbstractFunctionSpaceElement& y)
    {
      if( !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::operator-=");

      const ProductSpaceElement& y_ = dynamic_cast<const ProductSpaceElement&>(y);

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

    ProductSpaceElement& ProductSpaceElement::operator*=(double a)
    {
      if( isPrimalEnabled() )
        for(auto i = 0u; i < primalProductSpaceElement_.variables().size(); ++i)
          *primalProductSpaceElement_.variables()[i] *= a;

      if( isDualEnabled() )
        for(auto i = 0u; i < dualProductSpaceElement_.variables().size(); ++i)
          *dualProductSpaceElement_.variables()[i] *= a;

      reset();
      return *this;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement::operator- () const
    {
      std::decay_t<decltype(primalProductSpaceElement_.variables())> primal;
      if( isPrimalEnabled() )
        for(auto& var : primalProductSpaceElement_.variables())
          primal.push_back( -*var );
      std::decay_t<decltype(dualProductSpaceElement_.variables())> dual;
      if( isDualEnabled() )
        for(auto& var : dualProductSpaceElement_.variables())
          dual.push_back( -*var );
      return std::make_unique<ProductSpaceElement>(primal,dual,getSpace());
    }

    double ProductSpaceElement::applyAsDualTo(const AbstractFunctionSpaceElement& y) const
    {
      auto result = 0.;
      const auto& y_ = dynamic_cast<const ProductSpaceElement&>(y);
      assert( primalProductSpaceElement_.variables().size() == y_.primalProductSpaceElement_.variables().size() );
      assert( dualProductSpaceElement_.variables().size()   == y_.dualProductSpaceElement_.variables().size() );

      if( isPrimalEnabled() && y_.isPrimalEnabled() )
        for(auto i=0u; i<primalProductSpaceElement_.variables().size(); ++i)
        {
          std::cout << "Adding primal variable " << i << std::endl;
          result += primalProductSpaceElement_.variable(i)( y_.primalProductSpaceElement_.variable(i) );
        }

      if( isDualEnabled() && y_.isDualEnabled() )
        for(auto i=0u; i<dualProductSpaceElement_.variables().size(); ++i)
        {
          std::cout << "Adding dual variable " << i << std::endl;
          result += dualProductSpaceElement_.variable(i)( y_.dualProductSpaceElement_.variable(i) );
        }

      reset();
      y_.reset();
      return result;
    }

    unsigned ProductSpaceElement::size() const
    {
      return primalVariableSize() + dualVariableSize();
    }

    unsigned ProductSpaceElement::primalVariableSize() const
    {
      return primalProductSpaceElement_.size();
    }

    unsigned ProductSpaceElement::dualVariableSize() const
    {
      return dualProductSpaceElement_.size();
    }

    double& ProductSpaceElement::coefficient(unsigned i)
    {
      if( i > size() ) throw std::out_of_range("In ProductSpaceElement::coefficient(" + std::to_string(i) + ").");

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

    const double& ProductSpaceElement::coefficient(unsigned i) const
    {
      if( i > size() ) throw std::out_of_range("In ProductSpaceElement::coefficient(" + std::to_string(i) + ").");

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

    void ProductSpaceElement::print(std::ostream& os) const
    {
      os << "Space index: " << getSpace().index() << "\n";
      os << "Primal variables:\n";
      for( auto& v : primalProductSpaceElement_.variables() ) v->print(os);
      os << "Dual variables:\n";
      for( auto& v : dualProductSpaceElement_.variables() ) v->print(os);
    }


    bool isProductSpaceElement(const AbstractFunctionSpaceElement& x)
    {
      return dynamic_cast<const ::Algorithm::Fenics::ProductSpaceElement*>(&x) != nullptr;
    }

    ProductSpaceElement& toProductSpaceElement(AbstractFunctionSpaceElement& x)
    {
      if( !isProductSpaceElement(x) ) throw InvalidArgumentException("Fenics::toProductSpaceElement(AbstractFunctionSpaceElement&)");
      return dynamic_cast< ::Algorithm::Fenics::ProductSpaceElement& >(x);
    }

    const ProductSpaceElement& toProductSpaceElement(const AbstractFunctionSpaceElement& x)
    {
      if( !isProductSpaceElement(x) ) throw InvalidArgumentException("Fenics::toProductSpaceElement(const AbstractFunctionSpaceElement&)");
      return dynamic_cast<const ::Algorithm::Fenics::ProductSpaceElement&>(x);
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

    ProductSpaceElement* ProductSpaceElement::cloneImpl() const
    {
      auto primalVars = cloneVariables(primalProductSpaceElement_.variables());
      if(disablePrimal_) for(auto& var : primalVars) *var *= 0;
      auto dualVars = cloneVariables(dualProductSpaceElement_.variables());
      if(disableDual_) for(auto& var : dualVars) *var *= 0;
      reset();
      return new ProductSpaceElement(primalVars,dualVars,this->getSpace());
    }
  }
}
