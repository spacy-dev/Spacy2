#pragma once

#include <deal.II/lac/block_sparse_matrix.h>

#include <Spacy/Util/Base/operatorBase.hh>
#include <Spacy/Util/Base/vectorBase.hh>
#include <Spacy/Util/Mixins/get.hh>
#include <Spacy/linearSolver.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/Exceptions/callOfUndefinedFunctionException.hh>
#include <Spacy/vectorSpace.hh>

#include "cgSolver.hh"
#include "UMFPACKSolver.hh"
#include "vector.hh"

#include <algorithm>

namespace Spacy
{
    namespace dealII
    {
        template <int dim>
        class LinearOperator :
                public OperatorBase,
                public VectorBase,
                public Mixin::Get< dealii::BlockSparseMatrix<double> >
        {
        public:
            LinearOperator(const dealii::BlockSparseMatrix<double>& A,
                           const VectorSpace& operatorSpace,
                           const Spacy::Vector& boundary_values,
                           const VectorSpace& domain,
                           const VectorSpace& range)
                : OperatorBase(domain,range),
                  VectorBase(operatorSpace),
                  Mixin::Get< dealii::BlockSparseMatrix<double> >(A.get_sparsity_pattern()),
                  boundary_values_(boundary_values)
            {
                get().copy_from(A);
            }

            LinearOperator(const LinearOperator& other)
                : OperatorBase(other.domain(), other.range()),
                  VectorBase(other.space()),
                  Mixin::Get< dealii::BlockSparseMatrix<double> >(other.get().get_sparsity_pattern()),
                  boundary_values_(other.boundary_values_)
            {
                checkSpaceCompatibility(domain(), other.domain());
                checkSpaceCompatibility(range(), other.range());

                get().copy_from(other.get());
            }

            LinearOperator& operator=(const LinearOperator& other)
            {
                checkSpaceCompatibility(domain(), other.domain());
                checkSpaceCompatibility(range(), other.range());

                get().copy_from(other.get());
                boundary_values_ = other.boundary_values_;
                return *this;
            }

            /**
             * @brief Compute \f$A(x)\f$.
             * @param x operator argument
             */
            ::Spacy::Vector operator()(const ::Spacy::Vector& x) const
            {
                const auto& x_ = cast_ref<Vector>(x);
                auto y = zero(range());
                auto& y_ = cast_ref<Vector>(y);
                get().block(0,0).vmult(y_.get(), x_.get());
                return y;
            }

            Real operator()(const LinearOperator&) const
            {
                throw CallOfUndefinedFunctionException("dealII::LinearOperator::operator()(const LinearOperator&)");
            }

            LinearSolver solver() const
            {
                return UMFPackSolver<dim>(get().block(0,0), boundary_values_, domain(), range());
//                return CGSolver<dim>(get(), boundary_values_, domain(), range());
            }

            /**
             * @brief In-place summation \f$ x+=y\f$.
             * @param y vector to add to this vector
             * @return \f$ x+=y\f$.
             */
            LinearOperator& operator+=(const LinearOperator& y)
            {
              checkSpaceCompatibility(static_cast<const LinearOperator*>(this)->space(),y.space());
              get().add(1, y.get());
              return *this;
            }

            /**
             * @brief In-place subtraction \f$ x-=y\f$.
             * @param y vector to subtract from this vector
             * @return \f$ x-=y\f$.
             */
            LinearOperator& operator-=(const LinearOperator& y)
            {
              checkSpaceCompatibility(static_cast<const LinearOperator*>(this)->space(),y.space());
              get().add(-1, y.get());
              return *this;
            }

            /**
             * @brief In-place multiplication \f$ x*=a\f$.
             * @param a scaling factor
             * @return \f$ x*=a\f$.
             */
            LinearOperator& operator*=(double a)
            {
                get() *= a;
                return *this;
            }

            /**
             * @brief Negation \f$ -x\f$.
             * @return \f$ -x \f$.
             */
            LinearOperator operator-() const
            {
              LinearOperator y(*this);
              y.get() *= -1;
              return y;
            }

            /**
             * @brief Comparison operator \f$ x==y\f$.
             * @param y vector to compare with this vector
             * @return \f$ x==y\f$.
             */
            bool operator==(const LinearOperator& y) const
            {
              checkSpaceCompatibility(space(),y.space());
              auto max = std::max( (*this)(*this).get(), y(y).get() );
              if( max == 0 )
                max = 1;
              auto dx = y;
              dx -= *this;
              return dx(dx).get() < max*y.space().eps()*y.space().eps();
            }

        private:
            Spacy::Vector boundary_values_;
        };
    }
    }