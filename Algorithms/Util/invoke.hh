#ifndef ALGORITHM_UTIL_INVOKE_HH
#define ALGORITHM_UTIL_INVOKE_HH

#include <tuple>
#include <utility>

namespace Algorithm
{
  template <class T, int index>
  struct IdArg : std::integral_constant<int,index>
  {
    IdArg(const T& t) : t_(t) {}
    operator T() const { return t_; }
    const T& t_;
  };

  template <int... indices>
  struct ToVariableTuple
  {
    template <class... Args>
    static auto apply(const Args&... args)
    {
      return std::make_tuple(IdArg<Args,indices>(args)...);
    }
  };



  template<typename Func, typename Tup, std::size_t... index>
  decltype(auto) invoke_helper(Func&& func, Tup&& tup, std::index_sequence<index...>)
  {
      return func(std::get<index>(std::forward<Tup>(tup))...);
  }

  template<typename Func, typename Tup>
  decltype(auto) invoke(Func&& func, Tup&& tup)
  {
      constexpr auto Size = std::tuple_size<typename std::decay<Tup>::type>::value;
      return invoke_helper(std::forward<Func>(func),
                           std::forward<Tup>(tup),
                           std::make_index_sequence<Size>{});
  }

  template<typename Type, typename Tup, std::size_t... index>
  decltype(auto) create_helper(Tup&& tup, std::index_sequence<index...>)
  {
      return Type(std::get<index>(std::forward<Tup>(tup))...);
  }

  template<typename Type, typename Tup>
  decltype(auto) create(Tup&& tup)
  {
      constexpr auto Size = std::tuple_size<typename std::decay<Tup>::type>::value;
      return create_helper<Type>(std::forward<Tup>(tup),
                           std::make_index_sequence<Size>{});
  }
}

#endif // ALGORITHM_UTIL_INVOKE_HH
