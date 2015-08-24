#include "vector.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/cast.hh"

#include "vectorSpace.hh"

#include <algorithm>
#include <cassert>

namespace Algorithm
{
  namespace ProductSpace
  {
    namespace
    {
      std::vector< ::Algorithm::Vector > cloneVariables(const std::vector< ::Algorithm::Vector >& variables, const std::vector<unsigned>& ids)
      {
        std::vector< ::Algorithm::Vector > clonedVariables;

        for( unsigned i : ids ) clonedVariables.push_back( variables[i] );

        return clonedVariables;
      }
    }


    Vector::Vector(const VectorSpace& space)
      : VectorBase<Vector>(space)
    {
      if( isPrimalDual() )
      {
        variables_.push_back( cast_ref<VectorCreator>(space.impl()).primalSubSpace().element() );
        variables_.push_back( cast_ref<VectorCreator>(space.impl()).dualSubSpace().element() );
      }
      else
      {
        const auto& spaces = cast_ref<VectorCreator>(space.impl()).subSpaces();
        for (auto i=0u; i<spaces.size(); ++i)
          variables_.push_back( spaces[i]->element() );
      }
    }

    Vector::Vector(const Vector& other)
      : VectorBase<Vector>(other.space()), variables_(other.variables_)
    {
      if( isPrimalDual() )
      {
        if( !other.isPrimalEnabled() ) primalComponent() *= 0;
        if( !other.isDualEnabled() ) dualComponent() *= 0;
        reset(other);
      }
    }

    Vector& Vector::operator=(const Vector& y_)
    {
      if( !isPrimalDual() )
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

    Vector& Vector::operator+=(const Vector& y_)
    {
      if( !isPrimalDual() )
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


    Vector& Vector::operator-=(const Vector& y_)
    {
      if( !isPrimalDual() )
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
      for( auto i=0u; i<variables_.size(); ++i )
        if( !(variables_[i] == y.variables_[i]) ) return false;

      return true;
    }


    double Vector::operator()(const Vector& y_) const
    {
      assert( variables_.size() == y_.variables_.size() );

      if( !isPrimalDual() )
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

    ::Algorithm::Vector& Vector::variable(unsigned i)
    {
      if( isPrimalDual() )
      {
        if( creator().isPrimalSubSpaceId(i) )
          return primalComponent().variable(creator().primalIdMap(i));

        if( creator().isDualSubSpaceId(i) )
          return dualComponent().variable(creator().dualIdMap(i));
        assert(false);
      }

      return variables_[i];
    }

    const ::Algorithm::Vector& Vector::variable(unsigned i) const
    {
      if( isPrimalDual() )
      {
        if( creator().isPrimalSubSpaceId(i) )
          return primalComponent().variable(creator().primalIdMap(i));

        if( creator().isDualSubSpaceId(i) )
          return dualComponent().variable(creator().dualIdMap(i));
        assert(false);
      }

      return variables_[i];
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

    void Vector::setPrimalComponent(const ::Algorithm::Vector& y)
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

    void Vector::setDualComponent(const ::Algorithm::Vector& y)
    {
      variables_[1] = y;
    }

    const VectorCreator& Vector::creator() const
    {
      return cast_ref<ProductSpace::VectorCreator>(space().impl());
    }

    bool Vector::isPrimalDual() const
    {
      return creator().isPrimalDual();
    }
  }
}
