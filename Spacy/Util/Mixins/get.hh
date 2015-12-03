// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACYS_UTIL_GET_HH
#define SPACYS_UTIL_GET_HH

#include <utility>
#include <type_traits>

namespace Spacy
{
  /** @addtogroup MixinGroup
   * @{
   */
  namespace Mixin
  {
    /// Stores an object of type Type and provides access via member function get().
    template <class Type>
    class Get
    {
    public:
      Get() : t_{}
      {}

      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      template <class... Args,
                class = std::enable_if_t<std::is_constructible<Type,Args...>::value> >
      explicit Get(Args&&... args)
        : t_(std::forward<Args>(args)...)
      {}

      /// Access implementation.
      Type& get()
      {
        return t_;
      }

      /// Access implementation.
      const Type& get() const
      {
        return t_;
      }

    private:
      Type t_;
    };
  }
  /** @} */
}

#endif // SPACYS_UTIL_GET_HH
