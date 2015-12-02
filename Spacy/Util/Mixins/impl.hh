// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACYS_UTIL_Impl_HH
#define SPACYS_UTIL_Impl_HH

#include <memory>
#include <utility>
#include <stdexcept>
#include <type_traits>

#include "Spacy/Util/voider.hh"

namespace Spacy
{
  /** @addtogroup MixinGroup
   * @{
   */
  namespace Mixin
  {
    /// Stores an object of type Type and provides access via member function impl().
    template <class Type>
    class Impl
    {
    public:
      Impl()
        : impl_{}
      {}

      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      template <class... Args,
                class = std::enable_if_t<std::is_constructible<Type,Args...>::value> >
      explicit Impl(Args&&... args)
        : impl_(std::forward<Args>(args)...)
      {}

      /// Access implementation.
      Type& impl()
      {
        return impl_;
      }

      /// Access implementation.
      const Type& impl() const
      {
        return impl_;
      }

    private:
      Type impl_;
    };
  }
  /** @} */
}

#endif // SPACYS_UTIL_Impl_HH
