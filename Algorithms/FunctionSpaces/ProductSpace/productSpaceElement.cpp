#include "productSpaceElement.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/castTo.hh"

#include <cassert>
#include <numeric>
#include <stdexcept>

#include <iostream>

namespace Algorithm
{
  using Interface::AbstractFunctionSpace;
  using Interface::AbstractFunctionSpaceElement;

  std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > cloneVariables(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& variables)
  {
    std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > clonedVariables;

    for(const auto& var : variables) clonedVariables.emplace_back<std::unique_ptr<AbstractFunctionSpaceElement> >( clone(*var) );

    return clonedVariables;
  }

  namespace
  {
    std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > cloneVariables(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> >& variables,
                                                                               const std::vector<unsigned>& ids)
    {
      std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > clonedVariables;

      for( unsigned i : ids ) clonedVariables.emplace_back< std::unique_ptr<AbstractFunctionSpaceElement> >( clone(variables[i]) );

      return clonedVariables;
    }
  }

  ProductSpaceElement::ProductSpaceElement(const std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > &variables, const ProductSpace& space)
    : AbstractFunctionSpaceElement(space), variables_(cloneVariables(variables))
  {}

  ProductSpaceElement::ProductSpaceElement(const ProductSpace& space)
    : AbstractFunctionSpaceElement(space)
  {
    const auto& spaces = castTo<ProductSpace>(space).subSpaces();
    for (auto i=0u; i<spaces.size(); ++i)
      variables_.push_back(spaces[i]->element());
  }

  ProductSpaceElement::ProductSpaceElement(const ProductSpaceElement& other)
    : AbstractFunctionSpaceElement(other), variables_(cloneVariables(other.variables()))
  {
    if( !other.isPrimalEnabled() )
      for( unsigned i : space().primalSubSpaceIds() )
        variable(i) *= 0;
    if( !other.isDualEnabled() )
      for( unsigned i : space().dualSubSpaceIds() )
        variable(i) *= 0;

    reset(other);
  }

  void ProductSpaceElement::copyTo(AbstractFunctionSpaceElement& y) const
  {
    auto& y_ = castTo<ProductSpaceElement>(y);

    if( ( !isPrimalEnabled() && !y_.isDualEnabled() ) ||
        ( !isDualEnabled() && !y_.isPrimalEnabled() ) )
    {
      reset(y_);
      return;
    }

    if( !isPrimalEnabled() || !y_.isPrimalEnabled() )
    {
      for( unsigned i : space().dualSubSpaceIds() ) y_.variables()[i] = clone(variable(i));
      reset(y_);
      return;
    }

    if( !isDualEnabled() || !y_.isDualEnabled() )
    {
      for( unsigned i : space().primalSubSpaceIds() ) y_.variables()[i] = clone(variable(i));
      reset(y_);
      return;
    }

    y_.variables_ = cloneVariables(variables_);

  }

  ProductSpaceElement& ProductSpaceElement::operator=(const AbstractFunctionSpaceElement& y)
  {
    const auto& y_ = castTo<ProductSpaceElement>(y);

    if( isPrimalEnabled() && y_.isPrimalEnabled() )
      for( unsigned i : space().primalSubSpaceIds() )
        variables()[i] = clone( y_.variable(i) );

    if( isDualEnabled() && y_.isDualEnabled() )
      for( unsigned i : space().dualSubSpaceIds() )
        variables()[i] = clone( y_.variable(i) );

    reset(y_);
    return *this;
  }

  ProductSpaceElement& ProductSpaceElement::operator+=(const AbstractFunctionSpaceElement& y)
  {
    const auto& y_ = castTo<ProductSpaceElement>(y);

    if( isPrimalEnabled() && y_.isPrimalEnabled() )
      for( unsigned i : space().primalSubSpaceIds() )
        variable(i) += y_.variable(i);

    if( isDualEnabled() && y_.isDualEnabled() )
      for( unsigned i : space().dualSubSpaceIds() )
        variable(i) += y_.variable(i);

    reset(y_);
    return *this;
  }

  AbstractFunctionSpaceElement& ProductSpaceElement::axpy(double a, const AbstractFunctionSpaceElement& y)
  {
    const auto& y_ = castTo<ProductSpaceElement>(y);

    if( isPrimalEnabled() && y_.isPrimalEnabled() )
      for( unsigned i : space().primalSubSpaceIds() )
        variable(i).axpy( a , y_.variable(i) );

    if( isDualEnabled() && y_.isDualEnabled() )
      for( unsigned i : space().dualSubSpaceIds() )
        variable(i).axpy( a , y_.variable(i) );

    reset(y_);
    return *this;
  }


  ProductSpaceElement& ProductSpaceElement::operator-=(const AbstractFunctionSpaceElement& y)
  {
    const auto& y_ = castTo<ProductSpaceElement>(y);

    if( isPrimalEnabled() && y_.isPrimalEnabled() )
      for( unsigned i : space().primalSubSpaceIds() )
        variable(i) -= y_.variable(i);

    if( isDualEnabled() && y_.isDualEnabled() )
      for( unsigned i : space().dualSubSpaceIds() )
        variable(i) -= y_.variable(i);

    reset(y_);
    return *this;
  }

  ProductSpaceElement& ProductSpaceElement::operator*=(double a)
  {
    if( isPrimalEnabled() )
      for( unsigned i : space().primalSubSpaceIds() )
        variable(i) *= a;

    if( isDualEnabled() )
      for( unsigned i : space().dualSubSpaceIds() )
        variable(i) *= a;

    reset();
    return *this;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpaceElement::operator- () const
  {
    std::vector<std::unique_ptr<AbstractFunctionSpaceElement> > vars = cloneVariables(variables());
    if( isPrimalEnabled() )
      for( unsigned i : space().primalSubSpaceIds() )
        vars[i] = -variable(i);
    else
      for( unsigned i : space().primalSubSpaceIds() )
        vars[i] = clone(variable(i));

    if( isDualEnabled() )
      for( unsigned i : space().dualSubSpaceIds() )
        vars[i] = -variable(i);
    else
      for( unsigned i : space().dualSubSpaceIds() )
        vars[i] = clone(variable(i));

    reset();
    return std::make_unique<ProductSpaceElement>(vars,space());
  }

  double ProductSpaceElement::applyAsDualTo(const AbstractFunctionSpaceElement& y) const
  {
    auto result = 0.;
    const auto& y_  = castTo<ProductSpaceElement>(y);
    assert( variables().size() == y_.variables().size() );

    if( isPrimalEnabled() && y_.isPrimalEnabled() )
      for( auto i : space().primalSubSpaceIds() )
        result += variable(i)( y_.variable(i) );
    if( isDualEnabled() && y_.isDualEnabled() )
      for( auto i : space().dualSubSpaceIds() )
        result += variable(i)( y_.variable(i) );

    reset(y_);
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
    os << "Space index: " << space().index() << "\n";
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
    auto tmp = new ProductSpaceElement(*this);
    return tmp;
  }


  ProductSpaceElement ProductSpaceElement::primalElement() const
  {
    return ProductSpaceElement( cloneVariables(variables(),space().primalSubSpaceIds()) , space().primalSubSpace() );
  }

  ProductSpaceElement ProductSpaceElement::dualElement() const
  {
    return ProductSpaceElement( cloneVariables(variables(),space().dualSubSpaceIds()) , space().dualSubSpace() );
  }

  const ProductSpace& ProductSpaceElement::space() const
  {
    return castTo<ProductSpace>(AbstractFunctionSpaceElement::space());
  }


  FunctionSpaceElement primalElement(const FunctionSpaceElement& x)
  {
    return FunctionSpaceElement( castTo<ProductSpaceElement>(x.impl()).primalElement() );
  }

}
