#ifndef ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH


namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Parameter class for index of state variable.
     */
    class StateIndex
    {
    public:
      /**
       * @brief Constructor. Sets index of state variable.
       */
      explicit StateIndex(unsigned index = 0) noexcept;

      /**
       * @brief Set index of state variable.
       */
      void setStateIndex(unsigned index) noexcept;

      /**
       * @brief Get index of state variable.
       */
      double stateIndex() const noexcept;

    private:
      unsigned index_ = 0;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH
