// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_FENICS_TO_VTK_HH
#define SPACY_ADAPTER_FENICS_TO_VTK_HH

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

#endif // SPACY_ADAPTER_FENICS_TO_VTK_HH
