#include "productSpaceElement.hh"

#include "productSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/cast.hh"

#include <algorithm>
#include <cassert>

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
      variables_.push_back( cast_ref<ProductSpace>(space.impl()).primalSubSpace().element() );
      variables_.push_back( cast_ref<ProductSpace>(space.impl()).dualSubSpace().element() );
    }
    else
    {
      const auto& spaces = cast_ref<ProductSpace>(space.impl()).subSpaces();
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

  ProductSpaceElement& ProductSpaceElement::operator=(const ProductSpaceElement& y_)
  {
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
    return cast_ref<ProductSpaceElement>(variables_[0]);
  }

  ProductSpaceElement& ProductSpaceElement::primalComponent()
  {
    if( !isPrimalDualProductSpaceElement() ) throw std::runtime_error("ProductSpaceElement::primal can only be used with primal-dual product spaces.");
    return cast_ref<ProductSpaceElement>(variables_[0]);
  }

  void ProductSpaceElement::setPrimalComponent(const Vector& y)
  {
    variables_[0] = y;
  }

  const ProductSpaceElement& ProductSpaceElement::dualComponent() const
  {
    if( !isPrimalDualProductSpaceElement() ) throw std::runtime_error("ProductSpaceElement::dual can only be used with primal-dual product spaces.");
    return cast_ref<ProductSpaceElement>(variables_[1]);
  }

  ProductSpaceElement& ProductSpaceElement::dualComponent()
  {
    if( !isPrimalDualProductSpaceElement() ) throw std::runtime_error("ProductSpaceElement::dual can only be used with primal-dual product spaces.");
    return cast_ref<ProductSpaceElement>(variables_[1]);
  }

  void ProductSpaceElement::setDualComponent(const Vector& y)
  {
    variables_[1] = y;
  }

  const ProductSpace& ProductSpaceElement::productSpace() const
  {
    return cast_ref<ProductSpace>(space().impl());
  }

  bool ProductSpaceElement::isPrimalDualProductSpaceElement() const
  {
    return productSpace().isPrimalDualProductSpace();
  }
}
