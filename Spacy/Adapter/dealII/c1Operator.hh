#pragma once

#include <deal.II/base/tensor.h>
#include <deal.II/grid/tria.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/fe/fe_values.h>
#include <deal.II/lac/vector.h>
#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/dynamic_sparsity_pattern.h>

#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/Base/operatorBase.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/Exceptions/callOfUndefinedFunctionException.hh>

#include "linearOperator.hh"
#include "vector.hh"
#include "vectorSpace.hh"

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

namespace Spacy
{
    /** @addtogroup dealIIGroup @{ */
    namespace dealII
    {
        struct LinearOperatorCreator
        {
            Vector operator()(const VectorSpace*) const
            {
                throw CallOfUndefinedFunctionException("dealII::LinearOperatorCreator::operator()(const VectorSpace*)");
            }
        };

        template <class Implementation, int dim>
        class C1Operator : public OperatorBase
        {
        public:
            C1Operator(Implementation&& operator_impl, const VectorSpace& domain, const VectorSpace& range)
                : OperatorBase(domain,range),
                  A_(std::make_shared< dealii::SparseMatrix<double> >()),
                  operatorSpace_( std::make_shared<VectorSpace>( LinearOperatorCreator(),
                  [](const Spacy::Vector& v)
                  {
                      return cast_ref< LinearOperator<dim> >(v).get().frobenius_norm();
                  } , true ) ),
                  operator_(std::move(operator_impl))
            {
                const auto& dealIICreator = creator< VectorCreator<dim> >(domain);
                A_->reinit(dealIICreator.sparsityPattern());
                dummy_.reinit(dealIICreator.degreesOfFreedom());
                b_.reinit(dealIICreator.degreesOfFreedom());
            }

            /// Compute \f$A(x)\f$.
            ::Spacy::Vector operator()(const ::Spacy::Vector& x) const
            {
                assemble(x);
                return Vector(b_,range());
            }

            /// Compute \f$A'(x)dx\f$.
            ::Spacy::Vector d1(const ::Spacy::Vector &x, const ::Spacy::Vector &dx) const
            {
                assemble(x);

                const auto& dx_ = cast_ref<Vector>(dx);
                auto y = zero(range());
                auto& y_ = cast_ref<Vector>(y);
                A_->vmult(get(y_), get(dx_));
                return y;
            }

            /**
             * @brief Access \f$A'(x)\f$ as linear operator \f$X\rightarrow Y\f$
             * @see LinearOperator, ::Spacy::LinearOperator
             */
            auto linearization(const ::Spacy::Vector& x) const
            {
                assemble(x);
                return LinearOperator<dim>{ *A_ , *operatorSpace_ , domain(), range() };
            }

        private:
            template <class T>
            void update_cell_matrix(T q_index, T dofs_per_cell,
                                    const dealii::FEValues<dim>& fe_values,
                                    const std::vector< dealii::Tensor<1,dim> >& old_solution_gradients,
                                    dealii::FullMatrix<double>& cell_matrix) const
            {
                for(auto i=0u; i<dofs_per_cell; ++i)
                    for(auto j=0u; j<dofs_per_cell; ++j)
                        cell_matrix(i,j) += ( fe_values.shape_grad(i, q_index) *
                                              operator_.d1(old_solution_gradients[q_index],
                                                          fe_values.shape_grad(j, q_index)) *
                                              fe_values.JxW(q_index) );
            }

            template <class T>
            void update_cell_rhs(T q_index, T dofs_per_cell,
                                 const dealii::FEValues<dim>& fe_values,
                                 const std::vector< dealii::Tensor<1,dim> >& old_solution_gradients,
                                 dealii::Vector<double>& cell_rhs) const
            {
                for(auto i=0u; i<dofs_per_cell; ++i)
                {
                    cell_rhs(i) += ( fe_values.shape_grad(i, q_index) *
                                     operator_(old_solution_gradients[q_index]) *
                                     fe_values.JxW(q_index) );
                    cell_rhs(i) -= ( fe_values.shape_value(i, q_index) *
                                     1 * fe_values.JxW (q_index) );
                }
            }

            /// Assemble discrete representation of \f$A(x)\f$.
            void assemble(const ::Spacy::Vector& x) const
            {
                if( oldX_ && (oldX_ == x) ) return;
                *A_ = 0;
                b_ = 0;

                const auto& dealIICreator = creator< VectorCreator<dim> >(domain());
                dealii::QGauss<dim> quadratureFormula(2*dealIICreator.FEOrder());
                dealii::FEValues<dim> fe_values(dealIICreator.finiteElement(),
                                                quadratureFormula,
                                                dealii::update_values | dealii::update_gradients | dealii::update_JxW_values);

                const auto dofs_per_cell = dealIICreator.finiteElement().dofs_per_cell;
                auto n_q_points = quadratureFormula.size();

                dealii::FullMatrix<double> cell_matrix(dofs_per_cell, dofs_per_cell);
                dealii::Vector<double> cell_rhs(dofs_per_cell);

                std::vector< dealii::Tensor<1,dim> > old_solution_gradients(n_q_points);
                std::vector<dealii::types::global_dof_index> local_dof_indices(dofs_per_cell);

                for(auto cell = dealIICreator.dofHandler().begin_active();
                    cell < dealIICreator.dofHandler().end();
                    ++cell)
                {
                    fe_values.reinit(cell);
                    fe_values.get_function_gradients(get(cast_ref<Vector>(x)),
                                                     old_solution_gradients);
                    cell_matrix = 0;
                    cell_rhs = 0;

                    for(auto q_index=0u; q_index<n_q_points; ++q_index)
                    {
                        update_cell_matrix(q_index, dofs_per_cell,
                                           fe_values, old_solution_gradients, cell_matrix);
                        update_cell_rhs(q_index, dofs_per_cell,
                                        fe_values, old_solution_gradients, cell_rhs);
                    }

                    cell->get_dof_indices(local_dof_indices);

                    for(auto i=0u; i<dofs_per_cell; ++i)
                        for(auto j=0u; j<dofs_per_cell; ++j)
                            A_->add(local_dof_indices[i], local_dof_indices[j],
                                               cell_matrix(i,j));

                    for (auto i=0u; i<dofs_per_cell; ++i)
                        b_(local_dof_indices[i]) += cell_rhs(i);
                }

                oldX_ = x;
            }

            mutable std::shared_ptr< dealii::SparseMatrix<double> > A_;
            mutable dealii::Vector<double>       dummy_;
            mutable dealii::Vector<double>       b_;
            mutable ::Spacy::Vector oldX_;
            std::shared_ptr<VectorSpace> operatorSpace_;
            Implementation operator_;
        };
    }
    /** @} */
}
