#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <memory>

#include "vectorSpace.hh"

#include "Interface/Operator/abstractOperator.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class Vector;
  class LinearOperator;

  class Operator : public Mixin::UniqueImpl<Interface::AbstractOperator>
  {
  public:
    Operator(std::unique_ptr<Interface::AbstractOperator>&& implementation);

    Vector operator()(const Vector& x) const;

    /**
     * @brief Compute first directional derivative \f$A(x)'dx\f$ at \f$x\f$ in direction \f$dx\f$.
     */
    Vector d1(const Vector& x, const Vector& dx) const;

    /**
     * @brief Compute second directional derivative \f$A(x)'(dx,dy)\f$ at \f$x\f$ in direction \f$dx\f$ and \f$dy\f$.
     */
    Vector d2(const Vector& x, const Vector& dx, const Vector& dy) const;

    /**
     * @brief Get linearization \f$A(x)'\f$ at \f$x\f$.
     */
    LinearOperator linearization(const Vector& x) const;

    /**
     * @brief Access underlying domain.
     */
    VectorSpace* domain_ptr() const;

    /**
     * @brief Access range space.
     */
    VectorSpace* range_ptr() const;

    /**
     * @brief Access underlying domain.
     */
    const VectorSpace& domain() const;

    /**
     * @brief Access range space
     */
    const VectorSpace& range() const;
  };
}
#endif // ALGORITHM_OPERATOR_HH
