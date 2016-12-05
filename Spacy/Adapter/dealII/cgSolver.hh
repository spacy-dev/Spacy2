#pragma once

#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/solver_cg.h>
#include <deal.II/lac/precondition.h>
// For boundary values
#include <deal.II/base/function.h>
#include <deal.II/numerics/vector_tools.h>
#include <deal.II/numerics/matrix_tools.h>

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/Base/operatorBase.hh>
#include <Spacy/Util/Mixins/maxSteps.hh>
#include <Spacy/Util/Mixins/accuracy.hh>

#include "vector.hh"
#include "vectorSpace.hh"

#include <map>

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
            CGSolver(const dealii::SparseMatrix<double>& A,
                     const VectorSpace& domain,
                     const VectorSpace& range)
                : OperatorBase(range, domain),
                  A_(A.get_sparsity_pattern())
            {
                A_.copy_from(A);
            }

            CGSolver(const CGSolver& other)
                : OperatorBase(other),
                  A_(other.A_.get_sparsity_pattern())
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
                return *this;
            }

            ::Spacy::Vector operator()(Spacy::Vector x) const
            {
                auto& x_ = cast_ref<Vector>(x);

                dealii::SolverControl params(get(getMaxSteps()),
                                             get(getAbsoluteAccuracy()));
                dealii::SolverCG<> solver(params);

                auto y = zero(range());
                auto& y_ = cast_ref<Vector>(y);

                const auto& dealIICreator = creator< VectorCreator<dim> >(domain());
                std::map<dealii::types::global_dof_index,double> boundary_values;
                dealii::VectorTools::interpolate_boundary_values( dealIICreator.dofHandler(),
                                                                  0,
                                                                  dealii::ZeroFunction<dim>(),
                                                                  boundary_values );
                dealii::MatrixTools::apply_boundary_values (boundary_values,
                                                            A_,
                                                            get(y_),
                                                            get(x_));

                solver.solve(A_, get(y_), get(x_),
                             dealii::PreconditionIdentity());

                return y;
            }

        private:
            mutable dealii::SparseMatrix<double> A_;
        };
    }
    /** @} */
}
