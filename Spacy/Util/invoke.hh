#pragma once

#include <tuple>
#include <utility>

namespace Spacy
{
  /// @cond
  namespace InvokeDetail
  {
    template<typename Func, typename Tup, std::size_t... index>
    decltype(auto) invoke_helper(Func&& func, Tup&& tup, std::index_sequence<index...>)
    {
        return func(std::get<index>(std::forward<Tup>(tup))...);
    }

    template<typename Type, typename Tup, std::size_t... index>
    decltype(auto) create_helper(Tup&& tup, std::index_sequence<index...>)
    {
        return Type(std::get<index>(std::forward<Tup>(tup))...);
    }
  }
  /// @endcond


  /**
   * @brief Invoke function with arguments contained in tup.
   * @param func function to be invoked
   * @param tup = (a0,a1,...) arguments, individual entries must be accessible with std::get.
   *
   * Candidates for Tup are std::tuple, std::array, std::pair.
   *
   * @return func(a0,a1,...)
   */
  template<typename Func, typename Tup>
  decltype(auto) invoke(Func&& func, Tup&& tup)
  {
      constexpr auto Size = std::tuple_size<typename std::decay<Tup>::type>::value;
      return InvokeDetail::invoke_helper(std::forward<Func>(func),
                                         std::forward<Tup>(tup),
                                         std::make_index_sequence<Size>{});
  }

  /**
   * @brief Create object of type Type from elements contained in tup.
   * @param tup = (a0,a1,...) constructor arguments, individual entries must be accessible with std::get.
   *
   * Candidates for Tup are std::tuple, std::array, std::pair.
   *
   * @return Type(a0,a1,...)
   */
  template<typename Type, typename Tup>
  decltype(auto) create(Tup&& tup)
  {
      constexpr auto Size = std::tuple_size<typename std::decay<Tup>::type>::value;
      return InvokeDetail::create_helper<Type>(std::forward<Tup>(tup),
                                               std::make_index_sequence<Size>{});
  }
}
