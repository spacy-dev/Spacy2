#pragma once

#include <deal.II/lac/block_sparse_matrix.h>
#include <deal.II/lac/solver_cg.h>
#include <deal.II/lac/precondition.h>

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/Base/OperatorBase.hh>
#include <Spacy/Util/Mixins/maxSteps.hh>
#include <Spacy/Util/Mixins/accuracy.hh>

#include "vector.hh"
#include "vectorSpace.hh"

namespace Spacy
{
    /** @addtogroup dealIIGroup @{ */
    namespace dealII
    {
        template <int dim>
        class CGSolver :
                public OperatorBase,
                public Mixin::AbsoluteAccuracy,
                public Mixin::MaxSteps
        {
        public:
            CGSolver(const dealii::BlockSparseMatrix<double>& A,
                     const Spacy::Vector& boundary_values,
                     const VectorSpace& domain,
                     const VectorSpace& range)
                : OperatorBase(range, domain),
                  A_(A.get_sparsity_pattern()),
                  boundary_values_(boundary_values)
            {
                A_.copy_from(A);
            }

            CGSolver(const CGSolver& other)
                : OperatorBase(other),
                  A_(other.A_.get_sparsity_pattern()),
                  boundary_values_(other.boundary_values_)
            {
                checkSpaceCompatibility(domain(), other.domain());
                checkSpaceCompatibility(range(), other.range());

                A_.copy_from(other.A_);
            }

            CGSolver& operator=(const CGSolver& other)
            {
                checkSpaceCompatibility(domain(), other.domain());
                checkSpaceCompatibility(range(), other.range());

                A_.copy_from(other.A_);
                boundary_values_ = other.boundary_values_;
                return *this;
            }

            ::Spacy::Vector operator()(Spacy::Vector x) const
            {
                auto& x_ = cast_ref<Vector>(x);

                dealii::SolverControl params(getMaxSteps(),
                                             get(getAbsoluteAccuracy()));
                dealii::SolverCG<> solver(params);

                auto y = zero(range());
                auto& y_ = cast_ref<Vector>(y);

                solver.solve(A_.block(0,0), get(y_), get(x_),
                             dealii::PreconditionIdentity());

                return y + boundary_values_;
            }

        private:
            mutable dealii::BlockSparseMatrix<double> A_;
            Spacy::Vector boundary_values_;
        };
    }
    /** @} */
}
