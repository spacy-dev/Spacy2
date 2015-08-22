#include "productSpaceElement.hh"

#include "productSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/castTo.hh"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <stdexcept>

#include <iostream>

#include <boost/type_erasure/any_cast.hpp>

namespace Algorithm
{
  namespace
  {
    std::vector<Vector> cloneVariables(const std::vector<Vector>& variables, const std::vector<unsigned>& ids)
    {
      std::vector<Vector> clonedVariables;

      for( unsigned i : ids ) clonedVariables.push_back( variables[i] );

      return clonedVariables;
    }
  }


  ProductSpaceElement::ProductSpaceElement(const VectorSpace& space)
    : VectorBase<ProductSpaceElement>(space)
  {
    if( isPrimalDualProductSpaceElement() )
    {
      variables_.push_back( castAny<ProductSpace>(space.impl()).primalSubSpace().element() );
      variables_.push_back( castAny<ProductSpace>(space.impl()).dualSubSpace().element() );
    }
    else
    {
      const auto& spaces = castAny<ProductSpace>(space.impl()).subSpaces();
      for (auto i=0u; i<spaces.size(); ++i)
        variables_.push_back( spaces[i]->element() );
    }
  }

  ProductSpaceElement::ProductSpaceElement(const ProductSpaceElement& other)
    : VectorBase<ProductSpaceElement>(other.space()), variables_(other.variables_)
  {
    if( isPrimalDualProductSpaceElement() )
    {
      if( !other.isPrimalEnabled() ) primalComponent() *= 0;
      if( !other.isDualEnabled() ) dualComponent() *= 0;
      reset(other);
    }
  }

//  void ProductSpaceElement::copyTo(AbstractVector& y) const
//  {
//    auto& y_ = castTo<ProductSpaceElement>(y);

//    if( !isPrimalDualProductSpaceElement() )
//    {
//      y_.variables_ = cloneVariables(variables_);
//      return;
//    }

//    if( ( !isPrimalEnabled() && !y_.isDualEnabled() ) ||
//        ( !isDualEnabled() && !y_.isPrimalEnabled() ) )
//    {
//      reset(y_);
//      return;
//    }

//    if( isPrimalEnabled() && y_.isPrimalEnabled() )
//      y_.setPrimalComponent( clone(primalComponent()) );

//    if( isDualEnabled() && y_.isDualEnabled() )
//      y_.setDualComponent( clone(dualComponent()) );

//    reset(y_);
//    return;
//  }

  ProductSpaceElement& ProductSpaceElement::operator=(const ProductSpaceElement& y_)
  {
//    const auto& y_ = castAny<ProductSpaceElement>(y);

    if( !isPrimalDualProductSpaceElement() )
    {
      variables_ = y_.variables_;
      return *this;
    }

    if( isPrimalEnabled() && y_.isPrimalEnabled() )
      setPrimalComponent( y_.primalComponent() );

    if( isDualEnabled() && y_.isDualEnabled() )
      setDualComponent( y_.dualComponent() );

    reset(y_);
    return *this;
  }

  ProductSpaceElement& ProductSpaceElement::operator+=(const ProductSpaceElement& y_)
  {
//    const auto& y_ = castAny<ProductSpaceElement>(y);

    if( !isPrimalDualProductSpaceElement() )
    {
      for(auto i=0u; i<variables_.size(); ++i)
        variable(i) += y_.variable(i);
      return *this;
    }

    if( isPrimalEnabled() && y_.isPrimalEnabled() )
      primalComponent() += y_.primalComponent();

    if( isDualEnabled() && y_.isDualEnabled() )
      dualComponent() += y_.dualComponent();

    reset(y_);
    return *this;
  }

//  AbstractVector& ProductSpaceElement::axpy(double a, const AbstractVector& y)
//  {
//    const auto& y_ = castTo<ProductSpaceElement>(y);

//    if( !isPrimalDualProductSpaceElement() )
//    {
//      for(auto i=0u; i<variables_.size(); ++i)
//        variable(i).axpy(a,y_.variable(i));
//      return *this;
//    }

//    if( isPrimalEnabled() && y_.isPrimalEnabled() )
//      primalComponent().axpy(a,y_.primalComponent());

//    if( isDualEnabled() && y_.isDualEnabled() )
//      dualComponent().axpy(a,y_.dualComponent());

//    reset(y_);
//    return *this;
//  }


  ProductSpaceElement& ProductSpaceElement::operator-=(const ProductSpaceElement& y_)
  {
//    const auto& y_ = castAny<ProductSpaceElement>(y);

    if( !isPrimalDualProductSpaceElement() )
    {
      for(auto i=0u; i<variables_.size(); ++i)
        variable(i) -= y_.variable(i);
      return *this;
    }

    if( isPrimalEnabled() && y_.isPrimalEnabled() )
      primalComponent() -= y_.primalComponent();

    if( isDualEnabled() && y_.isDualEnabled() )
      dualComponent() -= y_.dualComponent();

    reset(y_);
    return *this;
  }

  ProductSpaceElement& ProductSpaceElement::operator*=(double a)
  {
    if( !isPrimalDualProductSpaceElement() )
    {
      for(auto i=0u; i<variables_.size(); ++i)
        variable(i) *= a;
      return *this;
    }

    if( isPrimalEnabled() )
      primalComponent() *= a;

    if( isDualEnabled() )
      dualComponent() *= a;

    reset();
    return *this;
  }

  ProductSpaceElement ProductSpaceElement::operator- () const
  {
    bool primalEnabled = isPrimalEnabled(), dualEnabled = isDualEnabled();
    ProductSpaceElement result = *this;
    if(primalEnabled) result.primalComponent() *= -1;
    if(dualEnabled) result.dualComponent() *= -1;
    return result;
  }

  bool ProductSpaceElement::operator==(const ProductSpaceElement& y) const
  {
    for( auto i=0u; i<variables_.size(); ++i )
      if( !(variables_[i] == y.variables_[i]) ) return false;

    return true;
  }


  double ProductSpaceElement::operator()(const ProductSpaceElement& y_) const
  {
//    const auto& y_  = castAny<ProductSpaceElement>(y);
    assert( variables_.size() == y_.variables_.size() );

    if( !isPrimalDualProductSpaceElement() )
    {
      auto result = 0.;
      for(auto i=0u; i<variables_.size(); ++i)
        result += variable(i)( y_.variable(i) );
      return result;
    }

    auto result = 0.;
    if( isPrimalEnabled() && y_.isPrimalEnabled() )
      result += primalComponent()( y_.primalComponent() );
    if( isDualEnabled() && y_.isDualEnabled() )
      result += dualComponent()( y_.dualComponent() );

    reset(y_);
    return result;
  }

  unsigned ProductSpaceElement::size() const
  {
    return std::accumulate( begin(variables_) , end(variables_), 0.0,
                            [](auto init, const auto& variable) { return init += variable.size(); } );
  }

  double& ProductSpaceElement::coefficient(unsigned i)
  {
//    if( i > size() ) throw std::out_of_range("In ProductSpaceElement::coefficient(" + std::to_string(i) + ").");

//    unsigned elementId = 0;
//    while ( i >= variables_[elementId]->size() )
//    {
//      i -= variables_[elementId]->size();
//      ++elementId;
//    }
//    return variables_[elementId]->coefficient(i);
  }

  const double& ProductSpaceElement::coefficient(unsigned i) const
  {
//    if( i > size() ) throw std::out_of_range("In ProductSpaceElement::coefficient(" + std::to_string(i) + ").");

//    unsigned elementId = 0;
//    while ( i >= variables_[elementId]->size() )
//    {
//      i -= variables_[elementId]->size();
//      ++elementId;
//    }
//    return variables_[elementId]->coefficient(i);
  }

//  void ProductSpaceElement::print(std::ostream& os) const
//  {
//    os << "Space index: " << space().index() << "\n";
//    os << "Variables:\n";
//    for( auto& v : variables_ ) v->print(os);
//  }

  Vector& ProductSpaceElement::variable(unsigned i)
  {
    if( isPrimalDualProductSpaceElement() )
    {
      if( productSpace().isPrimalSubSpaceId(i) )
        return primalComponent().variable(productSpace().primalIdMap(i));

      if( productSpace().isDualSubSpaceId(i) )
        return dualComponent().variable(productSpace().dualIdMap(i));
      assert(false);
    }

    return variables_[i];
  }

  const Vector& ProductSpaceElement::variable(unsigned i) const
  {
    if( isPrimalDualProductSpaceElement() )
    {
      if( productSpace().isPrimalSubSpaceId(i) )
        return primalComponent().variable(productSpace().primalIdMap(i));

      if( productSpace().isDualSubSpaceId(i) )
        return dualComponent().variable(productSpace().dualIdMap(i));
      assert(false);
    }

    return variables_[i];
  }

//  ProductSpaceElement* ProductSpaceElement::cloneImpl() const
//  {
//    return new ProductSpaceElement(*this);
//  }

  const ProductSpaceElement& ProductSpaceElement::primalComponent() const
  {
    if( !isPrimalDualProductSpaceElement() ) throw std::runtime_error("ProductSpaceElement::primal can only be used with primal-dual product spaces.");
    return castAny<ProductSpaceElement>(variables_[0]);
  }

  ProductSpaceElement& ProductSpaceElement::primalComponent()
  {
    if( !isPrimalDualProductSpaceElement() ) throw std::runtime_error("ProductSpaceElement::primal can only be used with primal-dual product spaces.");
    return castAny<ProductSpaceElement>(variables_[0]);
  }

  void ProductSpaceElement::setPrimalComponent(const Vector& y)
  {
    variables_[0] = y;
  }

  const ProductSpaceElement& ProductSpaceElement::dualComponent() const
  {
    if( !isPrimalDualProductSpaceElement() ) throw std::runtime_error("ProductSpaceElement::dual can only be used with primal-dual product spaces.");
    return castAny<ProductSpaceElement>(variables_[1]);
  }

  ProductSpaceElement& ProductSpaceElement::dualComponent()
  {
    if( !isPrimalDualProductSpaceElement() ) throw std::runtime_error("ProductSpaceElement::dual can only be used with primal-dual product spaces.");
    return castAny<ProductSpaceElement>(variables_[1]);
  }

  void ProductSpaceElement::setDualComponent(const Vector& y)
  {
    variables_[1] = y;
  }

//  double ProductSpaceElement::operator()(const ProductSpaceElement& y) const
//  {
//    const auto& y_  = castAny<ProductSpaceElement>(y);
//    assert( variables_.size() == y_.variables_.size() );

//    if( !isPrimalDualProductSpaceElement() )
//    {
//      auto result = 0.;
//      for(auto i=0u; i<variables_.size(); ++i)
//        result += variable(i)( y_.variable(i) );
//      return result;
//    }

//    auto result = 0.;
//    if( isPrimalEnabled() && y_.isPrimalEnabled() )
//      result += primalComponent()( y_.primalComponent() );
//    if( isDualEnabled() && y_.isDualEnabled() )
//      result += dualComponent()( y_.dualComponent() );

//    reset(y_);
//    return result;
//  }
  const ProductSpace& ProductSpaceElement::productSpace() const
  {
    return castAny<ProductSpace>(space().impl());
  }

  bool ProductSpaceElement::isPrimalDualProductSpaceElement() const
  {
    return productSpace().isPrimalDualProductSpace();
  }
}
