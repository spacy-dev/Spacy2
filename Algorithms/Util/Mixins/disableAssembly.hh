#ifndef ALGORITHM_UTIL_MIXIN_DISABLE_ASSEMBLY_HH
#define ALGORITHM_UTIL_MIXIN_DISABLE_ASSEMBLY_HH

namespace Algorithm
{
  namespace Mixin
  {
    class DisableAssembly
    {
    public:
      DisableAssembly(bool disable = false) noexcept;

      void disableAssembly(bool disable = true) noexcept;

      bool assemblyIsDisabled() const noexcept;

    private:
      bool disableAssembly_ = false;
    };
  }
}
#endif // ALGORITHM_UTIL_MIXIN_CONSTANT_HH
