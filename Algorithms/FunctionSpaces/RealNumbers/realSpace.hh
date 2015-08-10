#ifndef ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
#define ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH

#include <memory>

#include "Interface/abstractFunctionSpace.hh"

namespace Algorithm
{
  namespace Interface { class AbstractFunctionSpaceElement; }

  /// Space of real numbers.
  class RealSpace : public Interface::AbstractFunctionSpace
  {
  public:
    RealSpace();

  private:
    std::unique_ptr<Interface::AbstractFunctionSpaceElement> elementImpl() const override;
  };
}

#endif // ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
