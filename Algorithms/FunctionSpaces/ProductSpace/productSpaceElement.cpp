#include "productSpaceElement.hh"

#include "productSpace.hh"

#include "Util/invalidargumentexception.hh"

#include <cassert>
#include <numeric>
#include <stdexcept>

namespace Algorithm
{
  std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > cloneVariables(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& variables)
  {
    std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > clonedVariables;

    for(const auto& var : variables) clonedVariables.emplace_back<std::unique_ptr<AbstractFunctionSpaceElement> >( clone(*var) );

    return clonedVariables;
  }

  ProductSpaceElement::ProductSpaceElement(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > &variables, const AbstractBanachSpace& space)
    : AbstractFunctionSpaceElement(space), variables_(cloneVariables(variables))
  {}

  ProductSpaceElement::ProductSpaceElement(const AbstractBanachSpace& space)
    : AbstractFunctionSpaceElement(space)
  {
    const auto& spaces = dynamic_cast<const ProductSpace&>(space).getSpaces();
    for (auto i=0u; i<spaces.size(); ++i) variables_.push_back(spaces[i]->element());
  }

  void ProductSpaceElement::copyTo(AbstractFunctionSpaceElement& y) const
  {
    if( !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::copyTo");

    dynamic_cast<ProductSpaceElement&>(y).variables_ = cloneVariables(variables_);

  }

  ProductSpaceElement& ProductSpaceElement::operator=(const AbstractFunctionSpaceElement& y)
  {
    if( !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::operator=");

    for(auto i = 0u; i < variables_.size(); ++i)
      *variables_[i] = *dynamic_cast<const ProductSpaceElement&>(y).variables_[i];

    return *this;
  }

  ProductSpaceElement& ProductSpaceElement::operator+=(const AbstractFunctionSpaceElement& y)
  {
    if( !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::operator+=");

    for(auto i = 0u; i < variables_.size(); ++i)
      *variables_[i] += *dynamic_cast<const ProductSpaceElement&>(y).variables_[i];

    return *this;
  }

  ProductSpaceElement& ProductSpaceElement::operator-=(const AbstractFunctionSpaceElement& y)
  {
    if( !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceElement::operator-=");

    for(auto i = 0u; i < variables_.size(); ++i)
      (*variables_[i]) -= *dynamic_cast<const ProductSpaceElement&>(y).variables_[i];

    return *this;
  }

  ProductSpaceElement& ProductSpaceElement::operator*=(double a)
  {
    for(auto i = 0u; i < variables_.size(); ++i)
      *variables_[i] *= a;

    return *this;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement::operator- () const
  {
    decltype(variables_) primal;
    for(auto& var : variables_) primal.push_back( -*var );
    return std::make_unique<ProductSpaceElement>(primal,this->getSpace());
  }

  double ProductSpaceElement::applyAsDualTo(const AbstractFunctionSpaceElement& y) const
  {
    auto result = 0.;
    const auto& y_  = dynamic_cast<const ProductSpaceElement&>(y);
    assert( variables().size() == y_.variables().size() );
    for(auto i=0u; i<variables().size(); ++i)
      result += variable(i)( y_.variable(i) );

    return result;
  }

  unsigned ProductSpaceElement::size() const
  {
    return std::accumulate( begin(variables_) , end(variables_), 0.0,
                            [](auto init, const auto& variable) { return init += variable->size(); } );
  }

  double& ProductSpaceElement::coefficient(unsigned i)
  {
    if( i > size() ) throw std::out_of_range("In ProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    unsigned elementId = 0;
    while ( i >= variables_[elementId]->size() )
    {
      i -= variables_[elementId]->size();
      ++elementId;
    }
    return variables_[elementId]->coefficient(i);
  }

  const double& ProductSpaceElement::coefficient(unsigned i) const
  {
    if( i > size() ) throw std::out_of_range("In ProductSpaceElement::coefficient(" + std::to_string(i) + ").");

    unsigned elementId = 0;
    while ( i >= variables_[elementId]->size() )
    {
      i -= variables_[elementId]->size();
      ++elementId;
    }
    return variables_[elementId]->coefficient(i);
  }

  void ProductSpaceElement::print(std::ostream& os) const
  {
    os << "Space index: " << getSpace().index() << "\n";
    os << "Variables:\n";
    for( auto& v : variables_ ) v->print(os);
  }

  AbstractFunctionSpaceElement& ProductSpaceElement::variable(unsigned i)
  {
    return *variables_[i];
  }

  const AbstractFunctionSpaceElement& ProductSpaceElement::variable(unsigned i) const
  {
    return *variables_[i];
  }

  std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& ProductSpaceElement::variables()
  {
    return variables_;
  }

  const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& ProductSpaceElement::variables() const
  {
    return variables_;
  }

  ProductSpaceElement* ProductSpaceElement::cloneImpl() const
  {
    return new ProductSpaceElement(cloneVariables(variables_),this->getSpace());
  }


  bool isProductSpaceElement(const AbstractFunctionSpaceElement& x)
  {
    return dynamic_cast<const ProductSpaceElement*>(&x) != nullptr;
  }
}
