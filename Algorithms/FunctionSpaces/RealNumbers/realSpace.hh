#ifndef ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
#define ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH

#include <memory>

#include "Interface/abstractVectorSpace.hh"

namespace Algorithm
{
  namespace Interface { class AbstractVector; }

  /// Space of real numbers.
  class RealSpace : public Interface::AbstractVectorSpace
  {
  public:
    RealSpace();

  private:
    std::unique_ptr<Interface::AbstractVector> elementImpl() const override;
  };
}

#endif // ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
