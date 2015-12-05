#ifndef SPACY_MIXINS_EQUALITY_POINTER_COMPARISON_HH
#define SPACY_MIXINS_EQUALITY_POINTER_COMPARISON_HH

#include <vector>

namespace Spacy
{
  namespace Mixin
  {
    /// Connect objects of type Impl that provides a suitable method void update(Impl*) to another object of same type.
    template <class Impl>
    class MixinConnection
    {
    public:
      /// Attach observer.
      void attach(Impl& observer)
      {
        observers_.push_back(&observer);
      }

      /// Detach observer.
      void detach(Impl& observer)
      {
        auto iend = end(observers_);
        for(auto iter = begin(observers_); iter!= iend; ++iter)
          if( *iter == &observer )
          {
            observers_.erase(iter);
            return;
          }
      }

      /// Notify observers about changes.
      void notify()
      {
        for( Impl* observer : observers_ )
          observer->update(static_cast<Impl*>(this));
      }

    private:
      std::vector<Impl*> observers_ = {};
    };
  }

  template <class Base, class Source, class ToAttach>
  void castAndAttach(Source& source, ToAttach& toAttach)
  {
    static_cast<Base&>(source).attach(static_cast<Base&>(toAttach));
  }

  template <class Base, class Source, class ToAttach>
  void castAndDetach(Source& source, ToAttach& toAttach)
  {
    static_cast<Base&>(source).detach(static_cast<Base&>(toAttach));
  }

  namespace Optional
  {
    namespace Mixin
    {
      template <class Callee, class ToAttach, class Mixin,
                bool valid = std::is_base_of<Mixin,ToAttach>::value && std::is_base_of<Mixin,Callee>::value>
      struct SingleAttach
      {
        static void apply(Callee& callee, ToAttach& toAttach)
        {
          static_cast<Mixin&>(callee).attach( static_cast<Mixin&>(toAttach));
        }
      };

      template <class Callee,class ToAttach, class Mixin>
      struct SingleAttach<Callee,ToAttach,Mixin,false>
      {
        static void apply(Callee&, ToAttach&)
        {}
      };


      template <class Callee, class ToDetach, class Mixin,
                bool valid = std::is_base_of<Mixin,ToDetach>::value && std::is_base_of<Mixin,Callee>::value>
      struct SingleDetach
      {
        static void apply(Callee& callee, ToDetach& toDetach)
        {
          static_cast<Mixin&>(callee).detach( static_cast<Mixin&>(toDetach));
        }
      };

      template <class Callee,class ToDetach, class Mixin>
      struct SingleDetach<Callee,ToDetach,Mixin,false>
      {
        static void apply(Callee&, ToDetach&)
        {}
      };

      template <class... Mixins>
      struct Attach;

      template <class Mixin, class... Mixins>
      struct Attach<Mixin,Mixins...>
      {
        template <class Callee, class ToAttach, class... Other>
        static void apply(Callee& callee, ToAttach& toAttach, Other&... other)
        {
          apply(callee,toAttach);
          apply(callee,other...);
        }

        template <class Callee>
        static void apply(Callee&)
        {}

        template <class Callee, class ToAttach>
        static void apply(Callee& callee, ToAttach& toAttach)
        {
          SingleAttach<Callee,ToAttach,Mixin>::apply(callee,toAttach);
          Attach<Mixins...>::apply(callee,toAttach);
        }
      };

      template <>
      struct Attach<>
      {
        template <class Callee, class ToAttach>
        static void apply(Callee&, ToAttach&)
        {}
      };


      template <class... Mixins>
      struct Detach;

      template <class Mixin, class... Mixins>
      struct Detach<Mixin,Mixins...>
      {
        template <class Callee, class ToDetach>
        static void apply(Callee& callee, ToDetach& toDetach)
        {
          SingleDetach<Callee,ToDetach,Mixin>::apply(callee,toDetach);
          Detach<Mixins...>::apply(callee,toDetach);
        }
      };

      template <>
      struct Detach<>
      {
        template <class Callee, class ToDetach>
        static void apply(Callee&, ToDetach&)
        {}
      };

      template <class Mixin, class Callee, class ToAttach>
      void attach(Callee& callee, ToAttach& toAttach)
      {
        SingleAttach<Callee,ToAttach,Mixin>::apply(callee,toAttach);
      }


      template <class Mixin, class Callee, class ToDetach>
      void detach(Callee& callee, ToDetach& toDetach)
      {
        SingleDetach<Callee,ToDetach,Mixin>::apply(callee,toDetach);
      }
    }
  }
}

#endif // SPACY_MIXINS_EQUALITY_POINTER_COMPARISON_HH
