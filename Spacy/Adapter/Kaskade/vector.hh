#pragma once

#include <Spacy/Spaces/ScalarSpace/real.hh>
#include <Spacy/Util/Base/AddArithmeticOperators.hh>
#include <Spacy/Util/Base/VectorBase.hh>
#include <Spacy/vectorSpace.hh>

#include "util.hh"

#include "io/vtk.hh"

namespace Spacy
{
    namespace Kaskade
    {
        /// \cond
        template <class> class VectorCreator;
        /// \endcond

        /**
         * @ingroup KaskadeGroup VectorSpaceGroup
         * @brief Coefficient vector implementation for %Kaskade 7 (single space).
         * @tparam Description %Kaskade::VariableSetDescription
         */
        template <class Description>
        class Vector :
                public VectorBase ,
                public AddArithmeticOperators< Vector<Description> >
        {
            using VectorImpl = typename Description::template CoefficientVectorRepresentation<>::type;
            using Variable = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Variables,0>::type> >;
            using Space = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Spaces,Variable::spaceIndex>::type> >;
            using VariableSet = typename Description::VariableSet;

        public:
            /**
             * @brief Construct vector \f$x\f$ from underlying vector space.
             * @param space underlying vector space
             */
            Vector(const VectorSpace& space)
                : VectorBase(space),
                  variableSet_( creator< VectorCreator<Description> >(space).get() ),
                  description_( std::make_shared<Description>( creator< VectorCreator<Description> >(space).get()) ),
                  v_( Description::template CoefficientVectorRepresentation<>::init( variableSet_.descriptions.spaces ))
            {}

            //      /// Assign from coefficient vector of %Kaskade 7.
            //      Vector& operator=(const typename Description::template CoefficientVectorRepresentation<>::type& v)
            //      {
            //        variableSet_
            ////        v_ = v;
            //        return *this;
            //      }

            /// Access coefficient vector.
            VariableSet& get()
            {
                //        v_ = VectorImpl(variableSet_);
                //        return v_;
                return variableSet_;
            }

            /// Access coefficient vector.
            const VariableSet& get() const
            {
                //        v_ = VectorImpl(variableSet_);
                //        return v_;
                return variableSet_;
            }


            /**
             * @brief Apply as dual element.
             * @param y primal vector
             * @return \f$x(y)\f$
             */
            Real operator()(const Vector& y) const
            {
                VectorImpl v( Description::template CoefficientVectorRepresentation<>::init( variableSet_.descriptions.spaces ));
                VectorImpl w( Description::template CoefficientVectorRepresentation<>::init( variableSet_.descriptions.spaces ));
                variableSetToCoefficients(get(),v);
                variableSetToCoefficients(y.get(),w);
                return v*w;
                //        return get() * y.get();
                //        auto v = VectorImpl(variableSet_), w = VectorImpl(variableSet_);
                //        return v*w;
            }
            //      Vector& axpy(double a, const AbstractVector& y)
            //      {
            //        v_.axpy(a,castTo< Vector<Description> >(y).v_);
            //        return *this;
            //      }

        private:
            template <class Desc>
            friend void writeVTK(const Vector<Desc>& x, const char* fileName);
            VariableSet variableSet_;
            std::shared_ptr<Description> description_;
            mutable VectorImpl v_;
        };
    }
}
