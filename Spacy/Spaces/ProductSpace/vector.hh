#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <Spacy/vector.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/Base/VectorBase.hh>
#include <Spacy/Util/Exceptions/invalidArgumentException.hh>

namespace Spacy
{
    /** @addtogroup ProductSpaceGroup
     * @{
     */
    enum { PRIMAL=0 , DUAL=1 };

    namespace ProductSpace
    {
        /// @cond
        class VectorCreator;
        /// @endcond

        /**
         * @brief Product space vector.
         *
         * Represents a vector \f$x=(x_0,x_1,\ldots,x_n)\f$ of a product space \f$X = \{X_0,X_1,\ldots,X_n\}\f$.
         */
        class Vector : public VectorBase
        {
        public:

            using iterator = typename std::vector< ::Spacy::Vector >::iterator;

            using const_iterator = typename std::vector< ::Spacy::Vector >::const_iterator;

            /**
             * @brief Construct product space vector.
             * @param space associated vector space
             */
            explicit Vector(const VectorSpace& space);

//            Vector(const Vector& other);
//            Vector(Vector&& other);

//            Vector& operator=(const Vector&) = default;
//            Vector& operator=(Vector&&) = default;

            /// @return \f$x+y\f$
            Vector& operator+=(const Vector& y);

            //    /// Axpy-operation \f$x = x + ay\f$.
            //    AbstractVector& axpy(double a, const AbstractVector& y);

            /// @return \f$x-y\f$
            Vector& operator-=(const Vector& y);

            /// @return \f$ax\f$
            Vector& operator*=(double a);

            /// @return \f$-x\f$
            Vector operator- () const;

            /// Equality comparison (possibly up to the maximal attainable accuracy).
            bool operator==(const Vector& y) const;

            unsigned numberOfVariables() const;

            /**
             * @brief Access k-th component.
             * @return associated vector \f$x_k\f$
             */
            ::Spacy::Vector& component(unsigned k);

            /**
             * @brief Access k-th component.
             * @return associated vector \f$x_k\f$
             */
            const ::Spacy::Vector& component(unsigned k) const;

            /**
             * @brief Access VectorCreator object.
             * @see ProductSpace::VectorCreator
             */
            const VectorCreator& creator() const;

            /// Apply as dual element.
            Real operator()(const Vector& y) const;

            iterator begin();

            iterator end();

            const_iterator cbegin() const;

            const_iterator cend() const;

        private:
            std::vector< ::Spacy::Vector > components_ = {};
        };


        template <class T>
        struct BasicComponentView
        {
            explicit BasicComponentView(std::vector< T* > components)
                : components_(std::move(components))
            {}

            T& operator[](unsigned k)
            {
                return *components_[k];
            }

            const T& operator[](unsigned k) const
            {
                return *components_[k];
            }

        private:
            std::vector< T* > components_;
        };

        template <class T>
        struct BasicComponentView<const T>
        {
            explicit BasicComponentView(std::vector< const T* > components)
                : components_(std::move(components))
            {}

            const T& operator[](unsigned k) const
            {
                return *components_[k];
            }

        private:
            std::vector< const T* > components_;

        };


        template <class SingleSpaceVector>
        void extractSingleSpaceVectors( ::Spacy::Vector& x, std::vector<SingleSpaceVector*>& components)
        {
            if( is<SingleSpaceVector>(x))
            {
                components.push_back( &cast_ref<SingleSpaceVector>(x) );
                return;
            }

            if( is<ProductSpace::Vector>(x) )
            {
                auto& x_ = cast_ref<ProductSpace::Vector>(x);
                std::for_each( std::begin(x_), std::end(x_),
                               [&components](auto& x){ extractSingleSpaceVectors(x,components); });
                return;
            }

            throw Exception::InvalidArgument("ProductSpace::Vector::extractSingleSpaceVectors");
        }

        template <class SingleSpaceVector>
        void extractConstSingleSpaceVectors( const ::Spacy::Vector& x, std::vector<const SingleSpaceVector*>& components)
        {
            if( is<SingleSpaceVector>(x))
            {
                components.push_back( &cast_ref<SingleSpaceVector>(x) );
                return;
            }

            if( is<ProductSpace::Vector>(x) )
            {
                const auto& x_ = cast_ref<ProductSpace::Vector>(x);
                std::for_each( x_.cbegin(), x_.cend(),
                               [&components](const auto& x){ extractConstSingleSpaceVectors(x,components); });
                return;
            }

            throw Exception::InvalidArgument("ProductSpace::Vector::extractConstSingleSpaceVectors");
        }

        template <class SingleSpaceVector>
        BasicComponentView<SingleSpaceVector> extractSingleComponentView( ::Spacy::Vector& x)
        {
            std::vector<SingleSpaceVector*> components;
            extractSingleSpaceVectors(x,components);
            return BasicComponentView<SingleSpaceVector>(components);
        }

        template <class SingleSpaceVector>
        BasicComponentView<const SingleSpaceVector> extractSingleComponentView( const ::Spacy::Vector& x)
        {
            std::vector<const SingleSpaceVector*> components;
            extractConstSingleSpaceVectors(x,components);
            return BasicComponentView<const SingleSpaceVector>(components);
        }
    }

    /// @return cast_ref<ProductSpace::Vector>(v).component(PRIMAL);
    ::Spacy::Vector& primalComponent(::Spacy::Vector& v);

    /// @return cast_ref<ProductSpace::Vector>(v).component(PRIMAL);
    const ::Spacy::Vector& primalComponent(const ::Spacy::Vector& v);

    /// @return cast_ref<ProductSpace::Vector>(v).component(DUAL);
    ::Spacy::Vector& dualComponent(::Spacy::Vector& v);

    /// @return cast_ref<ProductSpace::Vector>(v).component(DUAL);
    const ::Spacy::Vector& dualComponent(const ::Spacy::Vector& v);

    /** @} */
}
