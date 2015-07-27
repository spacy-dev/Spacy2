#ifndef ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH


namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for index of adjoint variable.
     */
    class AdjointIndex
    {
    public:
      /**
       * @brief Constructor. Sets index of adjoint variable.
       */
      explicit AdjointIndex(unsigned index = 2) noexcept;

      /**
       * @brief Set index of adjoint variable.
       */
      void setAdjointIndex(unsigned index) noexcept;

      /**
       * @brief Get index of adjoint variable.
       */
      double adjointIndex() const noexcept;

    private:
      unsigned index_ = 2;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH
