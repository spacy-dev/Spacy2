#ifndef ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
#define ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH

#include <memory>

#include "../../Interface/abstractHilbertSpace.hh"

namespace Algorithm
{
  class AbstractNorm;
  class AbstractScalarProduct;
  class AbstractFunctionSpaceElement;

  class RealSpace : public AbstractHilbertSpace
  {
  public:
    RealSpace();

  private:
    void setNormImpl(std::shared_ptr<AbstractNorm>) override;

    void setScalarProductImpl(std::shared_ptr<AbstractScalarProduct>) override;

    std::shared_ptr<AbstractScalarProduct> getScalarProductImpl() const override;

    std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const override;

    std::shared_ptr<AbstractScalarProduct> sp_;
  };
}

#endif // ALGORITHM_FUNCTIONSPACES_REALNUMBERS_REALSPACE_HH
