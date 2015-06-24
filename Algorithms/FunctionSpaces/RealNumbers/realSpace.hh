#ifndef ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
#define ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH

#include <memory>

#include "Interface/abstractHilbertSpace.hh"

namespace Algorithm
{
  class AbstractNorm;
  class AbstractScalarProduct;
  class AbstractFunctionSpaceElement;

  /// Space of real numbers.
  class RealSpace : public AbstractHilbertSpace
  {
  public:
    RealSpace();

  private:
    std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const override;
  };
}

#endif // ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
