#pragma once

#include <string>

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  namespace FEniCS
  {
    /// @cond
    class Vector;
    /// @endcond

    void writeVTK(const Vector& x, const std::string& fileName);

    void writeVTK(const ::Spacy::Vector& x, const std::string& fileName);
  }
}
