#ifndef SPACY_UTIL_MIXINS_TARGET_HH
#define SPACY_UTIL_MIXINS_TARGET_HH

namespace Spacy
{
  namespace Mixin
  {
    /**
     * @brief Mixin class that provides a member function target() to access type-erased content (@see std::function).
     */
    template <class Derived>
    struct Target
    {
      template <class To>
      To* target()
      {
        if( dynamic_cast<typename Derived::template Base<To>*>( &static_cast<Derived*>(this)->base_.impl() ) != nullptr )
         return &dynamic_cast<typename Derived::template Base<To>*>( &static_cast<Derived*>(this)->base_.impl() )->impl();

        return nullptr;
      }

      template <class To>
      const To* target() const
      {
        if( dynamic_cast<const typename Derived::template Base<To>*>( &static_cast<const Derived*>(this)->base_.impl() ) != nullptr )
          return &dynamic_cast<const typename Derived::template Base<To>*>( &static_cast<const Derived*>(this)->base_.impl() )->impl();

        return nullptr;
      }
    };
  }
}

#endif // SPACY_UTIL_MIXINS_TARGET_HH
