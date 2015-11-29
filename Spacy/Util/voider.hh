// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_VOIDER_HH
#define SPACY_UTIL_VOIDER_HH

namespace Spacy
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace Detail
  {
    /// Helper, required in order to make void_t work with gcc.
    template <class...> struct voider { using type = void; };
  }
  /**
   * \endcond
   */

  /// Most fascinating type ever. Is always void.
  template <class... Types>
  using void_t = typename Detail::voider<Types...>::type;
}

#endif // SPACY_UTIL_VOIDER_HH
