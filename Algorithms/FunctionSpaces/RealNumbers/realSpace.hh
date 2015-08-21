#ifndef ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
#define ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH

#include <memory>

namespace Algorithm
{
  /// \cond
  class Vector;
  class VectorSpace;
  namespace Interface { class AbstractVector; }
  /// \endcond

  /// Space of real numbers.
  class RealSpace
  {
  public:
    Vector element(const VectorSpace* space) const;
  };

  VectorSpace makeRealSpace();
}

#endif // ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
