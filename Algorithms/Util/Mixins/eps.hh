#ifndef ALGORITHM_UTIL_MIXIN_EPS_HH
#define ALGORITHM_UTIL_MIXIN_EPS_HH


namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Parameter class for maximal attainable accuracy.
     */
    class Eps
    {
    public:
      /**
       * @brief Constructor. Sets maximal attainable accuracy.
       */
      explicit Eps(double eps = 1e-15) noexcept;

      /**
       * @brief Set maximal attainable accuracy.
       */
      void setEps(double) noexcept;

      /**
       * @brief Get maximal attainable accuracy.
       */
      double eps() const noexcept;

      /**
       * @brief Get square root of maximal attainable accuracy.
       */
      double sqrtEps() const noexcept;

      /**
       * @brief Get third root of maximal attainable accuracy.
       */
      double cbrtEps() const noexcept;

    private:
      double eps_ = 1e-15;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_EPS_HH
