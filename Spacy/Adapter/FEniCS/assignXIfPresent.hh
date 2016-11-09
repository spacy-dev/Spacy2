#pragma once

#include <Spacy/Util/voider.hh>

namespace Spacy
{
  namespace FEniCS
  {
    /// @cond
    template <class T>
    using Has_Member_X = decltype(std::declval<T>().x);

    template < class T , class = void >
    struct Assign_X_If_Present
    {
      template <class Arg>
      static void apply(T&, const Arg&)
      {}
    };

    template < class T >
    struct Assign_X_If_Present< T , void_t< Has_Member_X<T> > >
    {
      template <class Arg>
      static void apply(T& t, const Arg& x)
      {
        t.x = x;
      }
    };
    /// @endcond

    /**
     * @brief Assign T.x if t has a public member with name x. Else does nothing.
     */
    template < class T , class Arg >
    void assign_x_if_present(T& t, const Arg& x)
    {
      Assign_X_If_Present<T>::apply(t,x);
    }
  }
}
