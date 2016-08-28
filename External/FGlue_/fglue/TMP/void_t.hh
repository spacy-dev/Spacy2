// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef VOID_T_HH
#define VOID_T_HH

namespace FGlue
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace Detail
  {
    /// helper to make gcc happy
    template <class...> struct voider { using type = void; };
  }
  /**
   * \endcond
   */

  /// Most fascinating type ever. Is void for all input types.
  template <class... Types>
  using void_t = typename Detail::voider<Types...>::type;
}

#endif // VOID_T_HH
