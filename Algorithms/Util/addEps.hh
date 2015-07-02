#ifndef ALGORITHM_UTIL_ADD_EPS_HH
#define ALGORITHM_UTIL_ADD_EPS_HH


namespace Algorithm
{
  /**
 * @brief Parameter class for maximal attainable accuracy.
 */
  class AddEps
  {
  public:
    /**
   * @brief Set maximal attainable accuracy.
   */
    void setEps(double) noexcept;

    /**
   * @brief Get maximal attainable accuracy.
   */
    double eps() const noexcept;

  private:
    double eps_ = 1e-15;
  };
}

#endif // ALGORITHM_UTIL_ADD_EPS_HH
