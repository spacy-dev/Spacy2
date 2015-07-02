#ifndef ALGORITHM_ALGORITHM_PARAMETERS_HH
#define ALGORITHM_ALGORITHM_PARAMETERS_HH

namespace Algorithm
{
  /**
   * @brief Basic parameters for simple algorithms.
   */
  class Parameter
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
     * @brief Set maximal attainable accuracy.
     */
    void setEps(double) noexcept;

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
     * @brief Get maximal attainable accuracy.
     */
    double eps() const noexcept;

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
    double eps_ = 1e-15;
    unsigned maxSteps_ = 100;
    bool verbose_ = false;
  };
}

#endif // ALGORITHM_ALGORITHM_PARAMETERS_HH
