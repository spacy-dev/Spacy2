#ifndef ALGORITHM_HILBERTSPACENORM_HH
#define ALGORITHM_HILBERTSPACENORM_HH

#include "Interface/hilbertSpaceNorm.hh"
#include "norm.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  class HilbertSpaceNorm : public Norm
  {
  public:
    HilbertSpaceNorm(std::shared_ptr<Interface::HilbertSpaceNorm> implementation);

    double scalarProduct(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const;

    double squared(const FunctionSpaceElement& x) const;

    /**
     * \brief Access implementation.
     */
    Interface::HilbertSpaceNorm& impl();

    /**
     * \brief Access implementation.
     */
    const Interface::HilbertSpaceNorm& impl() const;

    /**
     * \brief Access implementation.
     */
    std::shared_ptr<Interface::HilbertSpaceNorm> sharedImpl() const;
  };
}

#endif // ALGORITHM_HILBERTSPACENORM_HH
