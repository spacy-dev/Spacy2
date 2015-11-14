#include "vector.hh"

#include "Spacy/Util/Exceptions/invalidArgumentException.hh"
#include "Spacy/Util/cast.hh"

#include "vectorSpace.hh"
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
    const auto& spaces = cast_ref<VectorCreator>(space.impl()).subSpaces();
    for (auto i=0u; i<spaces.size(); ++i)
      variables_.push_back( spaces[i]->vector() );
    }

    Vector& Vector::operator+=(const Vector& y)
    {
      checkSpaceCompatibility(this->space(),y.space());

      for(auto i=0u; i<variables_.size(); ++i)
        variable(i) += y.variable(i);
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

      for(auto i=0u; i<variables_.size(); ++i)
        variable(i) -= y.variable(i);
      return *this;
    }

    Vector& Vector::operator*=(double a)
    {
      for(auto i=0u; i<variables_.size(); ++i)
        variable(i) *= a;
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

      for(auto i=0u; i<variables_.size(); ++i)
        if( !(variable(i) == y.variable(i)) )
          return false;

      return true;
    }

    unsigned Vector::numberOfVariables() const
    {
        return variables_.size();
    }

    ::Spacy::Vector& Vector::variable(unsigned k)
    {
      return variables_[k];
    }

    const ::Spacy::Vector& Vector::variable(unsigned k) const
    {
      return variables_[k];
    }

    const VectorCreator& Vector::creator() const
    {
      return cast_ref<VectorCreator>(space()->impl());
    }


    Real Vector::operator()(const Vector& y) const
    {
      checkDualPairing(*this,y);
      assert( variables_.size() == y.variables_.size() );

    auto result = Real{0.};
    for(auto i=0u; i<variables_.size(); ++i)
      result += variable(i)( y.variable(i) );
    return result;
    }
  }
}
