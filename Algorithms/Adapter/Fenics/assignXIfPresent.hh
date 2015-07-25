#ifndef ALGORITHM_ADAPTER_FENICS_HASX_HH
#define ALGORITHM_ADAPTER_FENICS_HASX_HH

#include "Util/voider.hh"

namespace Algorithm
{
  template <class T>
  using Has_Member_X = decltype(std::declval<T>().x);

  template <class T>
  using Has_Member_X0 = decltype(std::declval<T>().x0);


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

  template < class T , class = void >
  struct Assign_X0_If_Present
  {
    template <class Arg>
    static void apply(T&, const Arg&)
    {}
  };

  template < class T >
  struct Assign_X0_If_Present< T , void_t< Has_Member_X0<T> > >
  {
    template <class Arg>
    static void apply(T& t, const Arg& x)
    {
      t.x0 = x;
    }
  };

  template < class T , class Arg >
  void assign_x0_if_present(T& t, const Arg& arg)
  {
    Assign_X0_If_Present<T>::apply(t,arg);
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_HASX_HH

