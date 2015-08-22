#ifndef ALGORITHM_INTERFACE_ABSTRACT_VECTOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_VECTOR_HH

#include <iostream>
#include <memory>
#include <vector>

#include "vector.hh"
#include "vectorSpace.hh"
#include "Util/Mixins/cloneable.hh"
#include "Util/Mixins/eps.hh"

namespace Algorithm
{
  /// \cond
//  class VectorSpace;
  /// \endcond

  template <class VType>
  class VectorBase
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

    double norm(const VType& x) const
    {
      return space().norm()(x);
    }

    double scalarProduct(const VType& x, const VType& y) const
    {
      return space().scalarProduct()(x,y);
    }

  private:
    const VectorSpace& space_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_VECTOR_HH
