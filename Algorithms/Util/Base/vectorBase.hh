#ifndef ALGORITHM_VECTOR_BASE_HH
#define ALGORITHM_VECTOR_BASE_HH

#include "vectorSpace.hh"

namespace Algorithm
{
  template <class VType, class Value = void>
  class VectorBase :
      public VectorBase<VType> , public Mixin::Impl<Value>
  {
  public:
    VectorBase( const VectorSpace& space , const Value& value )
      : VectorBase<VType>(space) , Mixin::Impl<Value>(value)
    {}
  };


  template <class VType>
  class VectorBase<VType,void>
  {
  public:
    VectorBase( const VectorSpace& space )
      : space_(space)
    {}

    VectorBase(const VectorBase &other)
      : space_(other.space_)
    {}

    VectorBase& operator=(const VectorBase& other)
    {
      assert(&space_ == &other.space());
      return *this;
    }

    const VectorSpace& space() const
    {
      return space_;
    }

    unsigned spaceIndex() const
    {
      return space().index();
    }

    bool isAdmissible() const
    {
      return true;
    }

    double norm() const
    {
      return space().norm()(static_cast<const VType&>(*this));
    }

    double scalarProduct(const VType& y) const
    {
      return space().scalarProduct()( static_cast<const VType&>(*this) , y );
    }

  private:
    const VectorSpace& space_;
  };

  template <class VType>
  class SupportedOperatorBase
  {
  public:
    VType& operator+=(const VType& y)
    {
      static_cast<VType*>(this)->impl() += y.impl();
      return static_cast<VType&>(*this);
    }

    VType& operator-=(const VType& y)
    {
      static_cast<VType*>(this)->impl() -= y.impl();
      return static_cast<VType&>(*this);
    }

    VType& operator*=(double a)
    {
      static_cast<VType*>(this)->impl() *= a;
      return static_cast<VType&>(*this);
    }

    VType operator-() const
    {
      VType y = static_cast<const VType&>(*this);
      y.impl() *= -1;
      return y;
    }
  };
}

#endif // ALGORITHM_VECTOR_BASE_HH
