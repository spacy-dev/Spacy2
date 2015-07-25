#include "disableAssembly.hh"

namespace Algorithm
{
  namespace Mixin
  {
    DisableAssembly::DisableAssembly(bool disable) noexcept
      : disableAssembly_(disable)
    {}

    void DisableAssembly::disableAssembly(bool disable) noexcept
    {
      disableAssembly_ = disable;
    }

    bool DisableAssembly::assemblyIsDisabled() const noexcept
    {
      return disableAssembly_;
    }
  }
}
