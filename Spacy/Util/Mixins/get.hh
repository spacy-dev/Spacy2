#pragma once

#include <ostream>
#include <utility>
#include <type_traits>

#include <Spacy/Util/voider.hh>

namespace Spacy
{
    namespace Mixin
    {
        /// Stores an object of type Type and provides access via member function get().
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
        struct HasMemFn_get< T, void_t< TryMemFn_get<T> > > : std::is_arithmetic< typename std::decay< TryMemFn_get<T> >::type >
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
        auto operator+(const T& x, S y) -> decltype(get(x) + y)
        {
            return get(x) + y;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator+(const T& x, S y) -> decltype(y + x)
        {
            return y + x;
        }

        template <class T,
                  typename std::enable_if<provide_operators<T>::value>::type* = nullptr>
        T operator+(T x, const T& y)
        {
            get(x) += get(y);
            return x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator-(const T& x, S y) -> decltype(get(x) - y)
        {
            return get(x) - y;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator-(S x, const T& y) -> decltype(x - get(y))
        {
            return x - get(y);
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
        auto operator*(const T& x, S y) -> decltype(get(x) * y)
        {
            return get(x) * y;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator*(S x, const T& y) -> decltype(y * x)
        {
            return y * x;
        }

        template <class T,
                  typename std::enable_if<provide_operators<T>::value>::type* = nullptr>
        T operator*(T x, const T& y)
        {
            get(x) *= get(y);
            return x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        T operator/(T x, S y)
        {
            get(x) /= y;
            return x;
        }

        template <class T, class S,
                  typename std::enable_if<are_admissible<T,S>::value>::type* = nullptr>
        auto operator/(S x, const T& y) -> decltype(x / get(y))
        {
            return x / get(y);
        }

        template <class T,
                  typename std::enable_if<provide_operators<T>::value>::type* = nullptr>
        T operator/(T x, const T& y)
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

        template <class T,
                  typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
        bool operator<(const Get<T>& x, const Get<T>& y)
        {
            return get(x) < get(y);
        }

        template <class T,
                  typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
        bool operator>(const Get<T>& x, const Get<T>& y)
        {
            return y < x;
        }

        template <class T,
                  typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
        bool operator<=(const Get<T>& x, const Get<T>& y)
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

        template <class T,
                  typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
        bool operator>=(const Get<T>& x, const Get<T>& y)
        {
            return y <= x;
        }

        template <class T,
                  typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
        bool operator==(const Get<T>& x, const Get<T>& y)
        {
            return get(x) == get(y);
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
                  typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
        std::ostream& operator<<(std::ostream& os, const Get<T>& x)
        {
            return os << get(x);
        }
    }
}
