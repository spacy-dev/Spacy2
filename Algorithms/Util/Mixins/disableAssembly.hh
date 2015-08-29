#ifndef ALGORITHM_UTIL_MIXIN_DISABLE_ASSEMBLY_HH
#define ALGORITHM_UTIL_MIXIN_DISABLE_ASSEMBLY_HH

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for enabling/disabling assembly.
     */
    class DisableAssembly
    {
    public:
      /**
       * @brief Constructor.
       * @param disable true: disable assemble, false: enable assembly
       */
      DisableAssembly(bool disable = false) noexcept;

      /**
       * @brief Disable/enable assembly.
       * @param disable true: disable assemble, false: enable assembly
       */
      void disableAssembly(bool disable = true) noexcept;

      /**
       * @brief Check if assembly is disabled.
       * @return true if assembly is disabled, else false
       */
      bool assemblyIsDisabled() const noexcept;

    private:
      bool disableAssembly_;
    };
  }
}
#endif // ALGORITHM_UTIL_MIXIN_CONSTANT_HH
