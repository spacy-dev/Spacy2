#ifndef ALGORITHM_UTIL_MIXIN_MAXSTEPS_HH
#define ALGORITHM_UTIL_MIXIN_MAXSTEPS_HH

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for maximal number of steps/iterations.
     */
    class MaxSteps
    {
    public:
      /**
       * @brief Constructor. Set maximal number of steps/iterations.
       */
      explicit MaxSteps(unsigned maxSteps = 100) noexcept;

      /**
       * @brief Set maximal number of steps/iterations for iterative solvers.
       */
      void setMaxSteps(unsigned) noexcept;

      /**
       * @brief Get maximal number of steps/iterations for iterative solvers.
       */
      unsigned maxSteps() const noexcept;

    private:
      unsigned maxSteps_ = 100;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_MAXSTEPS_HH
