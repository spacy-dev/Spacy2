#ifndef ALGORITHM_ALGORITHM_PARAMETERS_HH
#define ALGORITHM_ALGORITHM_PARAMETERS_HH

#include "Util/epsParameter.hh"

namespace Algorithm
{
  /**
   * @brief Basic parameters for simple algorithms.
   */
  class Parameter : public EpsParameter
  {
  public:
    /**
     * @brief Set absolute accuracy.
     */
    void setAbsoluteAccuracy(double) noexcept;

    /**
     * @brief Set relative accuracy.
     */
    void setRelativeAccuracy(double) noexcept;

    /**
     * @brief Set maximal number of steps/iterations for iterative solvers.
     */
    void setMaxSteps(unsigned) noexcept;

    /**
     * @brief Enable/disable verbosity.
     */
    void setVerbosity(bool) noexcept;

    /**
     * @brief Get absolute accuracy.
     */
    double absoluteAccuracy() const noexcept;

    /**
     * @brief Get relative accuracy.
     */
    double relativeAccuracy() const noexcept;

    /**
     * @brief Get maximal number of steps/iterations for iterative solvers.
     */
    unsigned maxSteps() const noexcept;

    /**
     * @brief Check if verbosity is turned on.
     */
    bool isVerbose() const noexcept;

  private:
    double absoluteAccuracy_ = 1e-12;
    double relativeAccuracy_ = 1e-6;
    unsigned maxSteps_ = 100;
    bool verbose_ = false;
  };
}

#endif // ALGORITHM_ALGORITHM_PARAMETERS_HH
