#ifndef ALGORITHM_ADAPTER_FENICS_HASX_HH
#define ALGORITHM_ADAPTER_FENICS_HASX_HH

#include "Util/voider.hh"

namespace Algorithm
{
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
}

#endif // ALGORITHM_ADAPTER_FENICS_HASX_HH

