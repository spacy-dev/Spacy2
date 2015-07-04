#ifndef ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
#define ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH

#include <memory>

#include "Interface/abstractHilbertSpace.hh"

namespace Algorithm
{
  namespace Interface { class AbstractFunctionSpaceElement; }

  /// Space of real numbers.
  class RealSpace : public Interface::AbstractHilbertSpace
  {
  public:
    RealSpace();

  private:
    std::unique_ptr<Interface::AbstractFunctionSpaceElement> elementImpl() const override;
  };
}

#endif // ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
