#ifndef ALGORITHM_HILBERTSPACENORM_HH
#define ALGORITHM_HILBERTSPACENORM_HH

#include "Interface/hilbertSpaceNorm.hh"
#include "norm.hh"
#include "scalarProduct.hh"

namespace Algorithm
{
  class Vector;

  class HilbertSpaceNorm : public Norm
  {
  public:
    HilbertSpaceNorm(std::shared_ptr<Interface::HilbertSpaceNorm> implementation);

    HilbertSpaceNorm(const ScalarProduct& sp);

    double scalarProduct(const Vector& x, const Vector& y) const;

    double squared(const Vector& x) const;

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
