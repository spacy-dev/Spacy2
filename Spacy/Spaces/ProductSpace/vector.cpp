#include "vector.hh"

#include "Spacy/Util/Exceptions/invalidArgumentException.hh"
#include "Spacy/Util/cast.hh"

#include "Spacy/Spaces/ProductSpace/vectorSpace.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vectorSpace.hh"

#include <algorithm>
#include <cassert>

namespace Spacy
{
  namespace ProductSpace
  {
    Vector::Vector(const VectorSpace& space)
      : VectorBase(space)
    {
      if( isPrimalDual() )
      {
        variables_.push_back( cast_ref<VectorCreator>(space.impl()).primalSubSpace().vector() );
        variables_.push_back( cast_ref<VectorCreator>(space.impl()).dualSubSpace().vector() );
      }
      else
      {
        const auto& spaces = cast_ref<VectorCreator>(space.impl()).subSpaces();
        for (auto i=0u; i<spaces.size(); ++i)
          variables_.push_back( spaces[i]->vector() );
      }
    }

    Vector::Vector(const Vector& y)
      : VectorBase(*y.space()), variables_(y.variables_)
    {
      if( isPrimalDual() )
      {
        if( !y.isPrimalEnabled() ) primalComponent() *= 0;
        if( !y.isDualEnabled() ) dualComponent() *= 0;
        reset(y);
      }
    }

    Vector::Vector(Vector&& y)
      : VectorBase(*y.space()), variables_(std::move(y.variables_))
    {
      if( isPrimalDual() )
      {
        if( !y.isPrimalEnabled() ) primalComponent() *= 0;
        if( !y.isDualEnabled() ) dualComponent() *= 0;
        reset(y);
      }
    }

    Vector& Vector::operator=(const Vector& y)
    {
      checkSpaceCompatibility(this->space(),y.space());
      if( !isPrimalDual() )
      {
        variables_ = y.variables_;
        return *this;
      }

      if( isPrimalEnabled() && y.isPrimalEnabled() )
        setPrimalComponent( y.primalComponent() );

      if( isDualEnabled() && y.isDualEnabled() )
        setDualComponent( y.dualComponent() );

      reset(y);
      return *this;
    }

    Vector& Vector::operator=(Vector&& y)
    {
      checkSpaceCompatibility(this->space(),y.space());
      if( !isPrimalDual() )
      {
        variables_ = std::move(y.variables_);
        return *this;
      }

      if( isPrimalEnabled() && y.isPrimalEnabled() )
        setPrimalComponent( std::move(y.primalComponent()) );

      if( isDualEnabled() && y.isDualEnabled() )
        setDualComponent( std::move(y.dualComponent()) );

      reset(y);
      return *this;
    }

    Vector& Vector::operator+=(const Vector& y)
    {
      checkSpaceCompatibility(this->space(),y.space());
      if( !isPrimalDual() )
      {
        for(auto i=0u; i<variables_.size(); ++i)
          variable(i) += y.variable(i);
        return *this;
      }

      if( isPrimalEnabled() && y.isPrimalEnabled() )
        primalComponent() += y.primalComponent();

      if( isDualEnabled() && y.isDualEnabled() )
        dualComponent() += y.dualComponent();

      reset(y);
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
      if( !isPrimalDual() )
      {
        for(auto i=0u; i<variables_.size(); ++i)
          variable(i) -= y.variable(i);
        return *this;
      }

      if( isPrimalEnabled() && y.isPrimalEnabled() )
        primalComponent() -= y.primalComponent();

      if( isDualEnabled() && y.isDualEnabled() )
        dualComponent() -= y.dualComponent();

      reset(y);
      return *this;
    }

    Vector& Vector::operator*=(double a)
    {
      if( !isPrimalDual() )
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

    Vector Vector::operator- () const
    {
      bool primalEnabled = isPrimalEnabled(), dualEnabled = isDualEnabled();
      Vector result = *this;
      if(primalEnabled) result.primalComponent() *= -1;
      if(dualEnabled) result.dualComponent() *= -1;
      return result;
    }

    bool Vector::operator==(const Vector& y) const
    {
      checkSpaceCompatibility(this->space(),y.space());
      if(isPrimalEnabled())
        for( auto i : creator().primalSubSpaceIds())
          if( !(variable(i) == y.variable(i)) )
          {
            reset(y);
            return false;
          }
      if(isDualEnabled())
        for( auto i : creator().dualSubSpaceIds())
          if( !(variable(i) == y.variable(i)) )
          {
            reset(y);
            return false;
          }

      reset(y);
      return true;
    }

    ::Spacy::Vector& Vector::variable(unsigned k)
    {
      if( isPrimalDual() )
      {
        if( creator().isPrimalSubSpaceId(k) )
          return primalComponent().variable(creator().primalIdMap(k));

        if( creator().isDualSubSpaceId(k) )
          return dualComponent().variable(creator().dualIdMap(k));
        assert(false);
      }

      return variables_[k];
    }

    const ::Spacy::Vector& Vector::variable(unsigned k) const
    {
      if( isPrimalDual() )
      {
        if( creator().isPrimalSubSpaceId(k) )
          return primalComponent().variable(creator().primalIdMap(k));

        if( creator().isDualSubSpaceId(k) )
          return dualComponent().variable(creator().dualIdMap(k));
        assert(false);
      }

      return variables_[k];
    }

    const Vector& Vector::primalComponent() const
    {
      if( !isPrimalDual() ) throw std::runtime_error("ProductSpace::Vector::primalComponent() can only be used with primal-dual product spaces.");
      return cast_ref<Vector>(variables_[0]);
    }

    Vector& Vector::primalComponent()
    {
      if( !isPrimalDual() ) throw std::runtime_error("ProductSpace::Vector::primalComponent() can only be used with primal-dual product spaces.");
      return cast_ref<Vector>(variables_[0]);
    }

    void Vector::setPrimalComponent(const ::Spacy::Vector& y)
    {
      variables_[0] = y;
    }

    const Vector& Vector::dualComponent() const
    {
      if( !isPrimalDual() ) throw std::runtime_error("ProductSpace::Vector::dualComponent() can only be used with primal-dual product spaces.");
      return cast_ref<Vector>(variables_[1]);
    }

    Vector& Vector::dualComponent()
    {
      if( !isPrimalDual() ) throw std::runtime_error("ProductSpace::Vector::dualComponent() can only be used with primal-dual product spaces.");
      return cast_ref<Vector>(variables_[1]);
    }

    void Vector::setDualComponent(const ::Spacy::Vector& y)
    {
      variables_[1] = y;
    }

    const VectorCreator& Vector::creator() const
    {
      return cast_ref<ProductSpace::VectorCreator>(space()->impl());
    }

    bool Vector::isPrimalDual() const
    {
      return creator().isPrimalDual();
    }


    Real Vector::operator()(const Vector& y) const
    {
      checkDualPairing(*this,y);
      assert( variables_.size() == y.variables_.size() );

      if( !isPrimalDual() )
      {
        auto result = Real{0.};
        for(auto i=0u; i<variables_.size(); ++i)
          result += variable(i)( y.variable(i) );
        return result;
      }

      auto result = Real{0.};
      if( isPrimalEnabled() && y.isPrimalEnabled() )
        result += primalComponent()( y.primalComponent() );
      if( isDualEnabled() && y.isDualEnabled() )
        result += dualComponent()( y.dualComponent() );

      reset(y);
      return result;
    }
  }
}
