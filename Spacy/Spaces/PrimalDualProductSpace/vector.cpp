#include "vector.hh"

#include "Spacy/Util/Exceptions/invalidArgumentException.hh"
#include "Spacy/Util/cast.hh"

#include "Spacy/Spaces/ProductSpace/vector.hh"
#include "vectorSpace.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vectorSpace.hh"

#include <algorithm>
#include <cassert>

namespace Spacy
{
  namespace PrimalDualProductSpace
  {
    Vector::Vector(const VectorSpace& space)
      : VectorBase(space)
    {
      variables_.push_back( cast_ref<VectorCreator>(space.impl()).primalSubSpace().vector() );
      variables_.push_back( cast_ref<VectorCreator>(space.impl()).dualSubSpace().vector() );
    }

    Vector& Vector::operator+=(const Vector& y)
    {
      checkSpaceCompatibility(this->space(),y.space());

      primalComponent() += y.primalComponent();
      dualComponent() += y.dualComponent();

      return *this;
    }

  //  AbstractVector& ProductSpaceElement::axpy(double a, const AbstractVector& y)
  //  {
  //    const auto& y_ = castTo<ProductSpaceElement>(y);

  //    if( !isPrimalDual() )
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


    Vector& Vector::operator-=(const Vector& y)
    {
      checkSpaceCompatibility(this->space(),y.space());

      primalComponent() -= y.primalComponent();
      dualComponent() -= y.dualComponent();

      return *this;
    }

    Vector& Vector::operator*=(double a)
    {
      primalComponent() *= a;
      dualComponent() *= a;

      return *this;
    }

    Vector Vector::operator- () const
    {
      Vector result = *this;
      return result *= -1;
    }

    bool Vector::operator==(const Vector& y) const
    {
      checkSpaceCompatibility(this->space(),y.space());
      for( auto i : creator().primalSubSpaceIds())
        if( !(variable(i) == y.variable(i)) )
          return false;
      for( auto i : creator().dualSubSpaceIds())
        if( !(variable(i) == y.variable(i)) )
          return false;

      return true;
    }

    ::Spacy::Vector& Vector::variable(unsigned k)
    {
      if( creator().isPrimalSubSpaceId(k) )
        return cast_ref<ProductSpace::Vector>(primalComponent()).variable(creator().primalIdMap(k));

      if( creator().isDualSubSpaceId(k) )
        return cast_ref<ProductSpace::Vector>(dualComponent()).variable(creator().dualIdMap(k));
      assert(false);
    }

    const ::Spacy::Vector& Vector::variable(unsigned k) const
    {
      if( creator().isPrimalSubSpaceId(k) )
        return cast_ref<ProductSpace::Vector>(primalComponent()).variable(creator().primalIdMap(k));

      if( creator().isDualSubSpaceId(k) )
        return cast_ref<ProductSpace::Vector>(dualComponent()).variable(creator().dualIdMap(k));
      assert(false);
    }

    const ::Spacy::Vector& Vector::primalComponent() const
    {
      return variables_[0];
    }

    ::Spacy::Vector& Vector::primalComponent()
    {
      return variables_[0];
    }

    void Vector::setPrimalComponent(::Spacy::Vector y)
    {
      variables_[0] = std::move(y);
    }

    const ::Spacy::Vector& Vector::dualComponent() const
    {
      return variables_[1];
    }

    ::Spacy::Vector& Vector::dualComponent()
    {
      return variables_[1];
    }

    void Vector::setDualComponent(::Spacy::Vector y)
    {
      variables_[1] = std::move(y);
    }

    const VectorCreator& Vector::creator() const
    {
      return cast_ref<PrimalDualProductSpace::VectorCreator>(space()->impl());
    }


    Real Vector::operator()(const Vector& y) const
    {
      checkDualPairing(*this,y);
      assert( variables_.size() == y.variables_.size() );

      auto result = Real{0.};
      result += primalComponent()( y.primalComponent() );
      result += dualComponent()( y.dualComponent() );

      return result;
    }
  }
}
