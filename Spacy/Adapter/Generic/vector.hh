#pragma once

#include <Spacy/Util/Base/AddArithmeticOperators.hh>
#include <Spacy/Util/Base/VectorBase.hh>
#include <Spacy/Util/Mixins/Get.hh>
#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Util/voider.hh>

namespace Spacy
{
    namespace Generic
    {
        /// @cond
        template <class> class Vector;

        namespace Detail
        {
            template <class T>
            using TryMemFn_dot = decltype( std::declval<T>().dot(std::declval<T>()) );

            template <class T>
            using TryMemFn_inner = decltype( std::declval<T>().inner(std::declval<T>()) );


            template <class T,class = void> struct HasMemFn_dot : std::false_type {};

            template <class T>
            struct HasMemFn_dot< T , voider< TryMemFn_dot<T> > > : std::true_type {};


            template <class T,class = void> struct HasMemFn_inner : std::false_type {};

            template <class T>
            struct HasMemFn_inner< T , voider< TryMemFn_inner<T> > > : std::true_type {};


            template <class VectorImpl,
                      bool = HasMemFn_dot<VectorImpl>::value,
                      bool = HasMemFn_inner<VectorImpl>::value>
            struct DualPairingImpl
            {
                template <template <class> class Vector>
                static auto apply(const Vector<VectorImpl>& x, const Vector<VectorImpl>& y)
                {
                    return x.get()*y.get();
                }
            };

            template <class VectorImpl, bool hasMemFn_inner>
            struct DualPairingImpl<VectorImpl,true,hasMemFn_inner>
            {
                template <template <class> class Vector>
                static Spacy::Real apply(const Vector<VectorImpl>& x, const Vector<VectorImpl>& y)
                {
                    return x.get().dot(y.get());
                }
            };

            template <class VectorImpl>
            struct DualPairingImpl<VectorImpl,false,true>
            {
                template <template <class> class Vector>
                static Spacy::Real apply(const Vector<VectorImpl>& x, const Vector<VectorImpl>& y)
                {
                    return x.get().inner(y.get());
                }
            };
        }
        /// @endcond

        /**
         * @ingroup GenericGroup
         * @brief Generic vector implementation for %Rn.
         */
        template <class VectorImpl>
        class Vector :
                public Mixin::Get<VectorImpl> ,
                public VectorBase ,
                public AddArithmeticOperators< Vector<VectorImpl> >
        {
        public:
            /**
             * @brief Construct zero vector \f$x\f$ from underlying vector space.
             * @param v inital value
             * @param space underlying vector space
             */
            Vector(const VectorImpl& v, const VectorSpace& space)
                : Mixin::Get<VectorImpl>( v ),
                  VectorBase(space)
            {}

            /**
             * @brief Assign from coefficient vector.
             * @param v coefficient vector
             */
            Vector& operator=(const VectorImpl& v)
            {
                this->get() = v;
                return *this;
            }

            /**
             * @brief Apply as dual element.
             * @param y primal vector
             * @return \f$x(y)\f$
             */
            Spacy::Real operator()(const Vector& y) const
            {
                return Detail::DualPairingImpl<VectorImpl>::apply(*this,y);
            }
        };
    }
}
