#pragma once

#include <cmath>
#include <ostream>
#include <utility>
#include <type_traits>

#include <Spacy/Util/voider.hh>

namespace Spacy
{
    namespace Mixin
    {
        /**
         * @brief Stores an object of type Type and provides access via member function get().
         *
         * If Type is a builtin arithmetic type (i.e.@code std::arithmetic<Type>::value@endcode returns true),
         * then deriving from Get<Type> enable the comparison operators @code<,>,<=,>=,==@endcode.
         */
        template <class Type>
        class Get
        {
        public:
            Get() : t_{}
            {}

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

        template <class T>
        T& get(Get<T>& t)
        {
            return t.get();
        }

        template <class T>
        const T& get(const Get<T>& t)
        {
            return t.get();
        }

        template <class T, class S>
        struct are_arithmetic
        {
            static constexpr bool value = std::is_arithmetic<T>::value && std::is_arithmetic<S>::value;
        };

        template <class T>
        using TryMemFn_get = decltype( std::declval<T>().get());

        template <class T, class = void>
        struct HasMemFn_get : std::false_type
        {};

        template <class T>
        struct HasMemFn_get< T, voider< TryMemFn_get<T> > >
                : std::integral_constant<bool, std::is_arithmetic< std::decay_t<TryMemFn_get<T>> >::value ||
                                               HasMemFn_get< std::decay_t<TryMemFn_get<T>> >::value>
        {};

        template <class...> struct provide_operators;

        template <class T, typename... Ts>
        struct provide_operators<T,Ts...>
        {
            static constexpr bool value = provide_operators<T>::value && provide_operators<Ts...>::value;
        };

        template <class T>
        struct provide_operators<T>
        {
            static constexpr bool value = HasMemFn_get<T>::value;
        };

        template <class T, class S>
        struct are_admissible
        {
            static constexpr bool value = provide_operators<T>::value && std::is_arithmetic<S>::value;
        };

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator+(T x, S y)
        {
            get(x) += y;
            return x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator+(S x, const T& y)
        {
            return y + x;
        }

        template <class T,
                  typename std::enable_if<provide_operators<T>::value>::type* = nullptr>
        auto operator+(T x, const T& y)
        {
            get(x) += get(y);
            return x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator-(T x, S y)
        {
            get(x) -= y;
            return x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator-(S x, T y)
        {
            get(y) -= x;
            return -y;
        }

        template <class T,
                  typename std::enable_if<provide_operators<T>::value>::type* = nullptr>
        T operator-(T x, const T& y)
        {
            get(x) -= get(y);
            return x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator*(T x, S y)
        {
            get(x) *= y;
            return x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator*(S x, T y)
        {
            get(y) *= x;
            return y;
        }

        template <class T,
                  typename std::enable_if<provide_operators<T>::value>::type* = nullptr>
        auto operator*(T x, const T& y)
        {
            get(x) *= get(y);
            return x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator/(T x, S y)
        {
            get(x) /= y;
            return x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator/(S x, T y)
        {
            get(y) = 1/get(y);
            return y *= x;
        }

        template <class T,
                  typename std::enable_if<provide_operators<T>::value>::type* = nullptr>
        auto operator/(T x, const T& y)
        {
            get(x) /= get(y);
            return x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        bool operator<(const T& x, S y)
        {
            return get(x) < y;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        bool operator<(S x, const T& y)
        {
            return x < get(y);
        }

        template <class T, class S,
                  typename std::enable_if<provide_operators<T,S>::value>::type* = nullptr>
        bool operator<(const T& x, const S& y)
        {
            return get(x) < get(y);
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        bool operator>(const T& x, S y)
        {
            return y < x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        bool operator>(S x, const T& y)
        {
            return y < x;
        }

        template <class T, class S,
                  typename std::enable_if<provide_operators<T,S>::value>::type* = nullptr>
        bool operator>(const T& x, const S& y)
        {
            return get(y) < get(x);
        }

        template <class T, class S,
                  typename std::enable_if<provide_operators<T,S>::value>::type* = nullptr>
        bool operator<=(const T& x, const S& y)
        {
            return get(x) <= get(y);
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        bool operator<=(const T& x, S y)
        {
            return get(x) <= y;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        bool operator<=(S x, const T& y)
        {
            return x <= get(y);
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        bool operator>=(S x, const T& y)
        {
            return y <= x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        bool operator>=(const T& x, S y)
        {
            return y <= x;
        }

        template <class T, class S,
                  typename std::enable_if<provide_operators<T,S>::value>::type* = nullptr>
        bool operator>=(const T& x, const S& y)
        {
            return y <= x;
        }

        template <class T, class S,
                  typename std::enable_if<provide_operators<T,S>::value>::type* = nullptr>
        bool operator==(const T& x, const S& y)
        {
            return get(x) == get(y);
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        bool operator==(const T& x, S y)
        {
            return get(x) == y;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        bool operator==(S x, const T& y)
        {
            return y == x;
        }

        template <class T,
                  typename std::enable_if<provide_operators<T>::value>::type* = nullptr>
        std::ostream& operator<<(std::ostream& os, const T& x)
        {
            return os << get(x);
        }
    }

    /// Compute maximum.
    template <class T,
              std::enable_if_t<std::is_arithmetic<T>::value>* = nullptr>
    auto max(T x, T y)
    {
        return std::max(x,y);
    }

    /// Compute maximum.
    template <class T,
              std::enable_if_t< Mixin::HasMemFn_get<T>::value >* = nullptr>
    auto max(T x, const T& y)
    {
        get(x) = max(get(x),get(y));
        return x;
    }

    /// Compute maximum.
    template <class T, class S,
              std::enable_if_t<Mixin::HasMemFn_get<T>::value>* = nullptr,
              std::enable_if_t<std::is_arithmetic<S>::value>* = nullptr>
    auto max(T x, S y)
    {
        get(x) = max(get(x),y);
        return x;
    }

    /// Compute maximum.
    template <class T, class S,
              std::enable_if_t<Mixin::HasMemFn_get<T>::value>* = nullptr,
              std::enable_if_t<std::is_arithmetic<S>::value>* = nullptr>
    auto max(S x, T y)
    {
        get(y) = max(x,get(y));
        return y;
    }

    /// Compute minimum.
    template <class T,
              std::enable_if_t<std::is_arithmetic<T>::value>* = nullptr>
    auto min(T x, T y)
    {
        return std::min(x,y);
    }

    /// Compute minimum.
    template <class T,
              std::enable_if_t<Mixin::HasMemFn_get<T>::value>* = nullptr>
    auto min(T x, const T& y)
    {
        get(x) = min(get(x),get(y));
        return x;
    }

    /// Compute minimum.
    template <class T, class S,
              std::enable_if_t<Mixin::HasMemFn_get<T>::value>* = nullptr,
              std::enable_if_t<std::is_arithmetic<S>::value>* = nullptr>
    auto min(T x, S y)
    {
        get(x) = min(get(x),y);
        return x;
    }

    /// Compute minimum.
    template <class T, class S,
              std::enable_if_t<Mixin::HasMemFn_get<T>::value>* = nullptr,
              std::enable_if_t<std::is_arithmetic<S>::value>* = nullptr>
    auto min(S x, T y)
    {
        get(y) = min(x,get(y));
        return y;
    }
}
