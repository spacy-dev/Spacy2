#ifndef ALGORITHM_1D_MODEL_CUBIC_FUNCTION_HH
#define ALGORITHM_1D_MODEL_CUBIC_FUNCTION_HH

namespace Algorithm
{
  namespace Functions_1D
  {
    /**
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
      Cubic(double a, double b, double c, double d) noexcept;

      /// Compute \f$q(t) = a + bt + ct^2 + dt^3 \f$.
      double operator()(double t) const noexcept;

    private:
      double a_, b_, c_, d_;
    };
  }
}

#endif // ALGORITHM_1D_MODEL_CUBIC_FUNCTION_HH
