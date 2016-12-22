#pragma once

#include <Spacy/vectorSpace.hh>

namespace Spacy
{
  /**
   * @brief Construct space of real numbers.
   */
  VectorSpace make_real_space(bool defaultIndex = false);

  namespace Space
  {
    /**
     * @brief Global space of real numbers with space index 0.
     *
     * This space is used by class Real if the underlying space is left unspecified.
     */
    static VectorSpace R = make_real_space(true);
  }
}
