#ifndef SPACY_1D_MODEL_CUBIC_FUNCTION_HH
#define SPACY_1D_MODEL_CUBIC_FUNCTION_HH

#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  namespace Functions_1D
  {
    /**
     * @ingroup SpacyGroup
     * @brief A one-dimensional cubic function \f$q(t) = a + bt + ct^2 + dt^3\f$.
     */
    class Cubic
    {
    public:
      /**
       * @brief Constructor.
       * @param a coefficient of constant term
       * @param b coefficient of linear term
       * @param c coefficient of quadratic term
       * @param d coefficient of cubic term
       */
      Cubic(Real a, Real b, Real c, Real d) noexcept;

      /**
       * @brief Compute \f$q(t) = a + bt + ct^2 + dt^3 \f$.
       * @param t argument
       * @return \f$q(t) = a + bt + ct^2 + dt^3 \f$
       */
      Real operator()(Real t) const noexcept;

    private:
      Real a_, b_, c_, d_;
    };
  }
}

#endif // SPACY_1D_MODEL_CUBIC_FUNCTION_HH