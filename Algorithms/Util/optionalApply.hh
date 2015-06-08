#ifndef ALGORITHM_UTIL_OPTIONAL_APPLY_HH
#define ALGORITHM_UTIL_OPTIONAL_APPLY_HH

#include "static_checks.hh"
#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  namespace Flexible
  {
    template <class Vector, bool>
    struct ScalarProduct
    {
      static auto apply(const Vector& v, const Vector& w) { return v*w; }
    };

    template <class Vector>
    struct ScalarProduct<Vector,true>
    {
      static auto apply(const Vector& v, const Vector& w) { return dot(v,w); }
    };

    template <class Vector>
    auto scalarProduct(const Vector& v, const Vector& w)
    {
      return ScalarProduct<Vector,Has_FreeFunction_Dot<Vector>::value>(v,w);
    }
  }
}

#endif // ALGORITHM_UTIL_OPTIONAL_APPLY_HH
