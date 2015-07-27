#ifndef ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH


namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for index of control variable.
     */
    class ControlIndex
    {
    public:
      /**
       * @brief Constructor. Sets index of control variable.
       */
      explicit ControlIndex(unsigned index = 1) noexcept;

      /**
       * @brief Set index of control variable.
       */
      void setControlIndex(unsigned index) noexcept;

      /**
       * @brief Get index of control variable.
       */
      double controlIndex() const noexcept;

    private:
      unsigned index_ = 1;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH
