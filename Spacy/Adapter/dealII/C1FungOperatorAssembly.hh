#pragma once

#include <deal.II/base/tensor.h>

#include "util.hh"

#include <tuple>
#include <vector>

namespace Spacy
{
    namespace dealII
    {
        namespace Detail
        {
            template <int dim, class Components, int row, int column,
                      int row_dim = GetDim<row, Components>::value,
                      int column_dim = GetDim<column, Components>::value>
            struct LocalFunGOperatorAssembly : LocalAssemblyBase<dim,row,row_dim,column,column_dim>
            {
                using Value = std::conditional_t<row_dim==1, double, dealii::Tensor<1,dim> >;
                using Gradient = std::conditional_t<row_dim==1, dealii::Tensor<1,dim>, dealii::Tensor<2,dim> >;

                explicit LocalFunGOperatorAssembly(const VectorSpace& domain,
                                                               const dealii::FESystem<dim>& fe_system)
                    : LocalAssemblyBase<dim,row,row_dim,column,column_dim>(domain, fe_system),
                      old_solution_values_(this->n_q_points_),
                      old_solution_gradients_(this->n_q_points_)
                {
                }

                template <class Vector>
                void init_old_solution(const Vector& x)
                {
                    this->row_fe_values[this->row_space_creator_.extractor()].get_function_values(x, old_solution_values_);
                    this->row_fe_values[this->row_space_creator_.extractor()].get_function_gradients(x, old_solution_gradients_);
                }

                template <class FunGOperator, class Index>
                void update_functional(FunGOperator& A, Index q_index)
                {
                    A.template update<row>(std::make_tuple(old_solution_values_[q_index],
                                                           old_solution_gradients_[q_index]));
                }

                template <class FunGOperator, class Index>
                void update_gradient_and_hessian(FunGOperator& A, Index q_index)
                {
                    const auto& row_value_view = this->row_fe_values[this->row_space_creator_.extractor()];
                    const auto& column_value_view = this->column_fe_values[this->column_space_creator_.extractor()];
                    for(Index i=0; i<this->row_dofs_per_cell_; ++i)
                    {
                        this->cell_rhs(i) += A() * row_value_view.gradient(i, q_index) * this->row_fe_values.JxW(q_index);
                        this->cell_rhs(i) -= 1 * row_value_view.value(i, q_index) * this->row_fe_values.JxW (q_index);

                        for(Index j=0; j<this->column_dofs_per_cell_; ++j)
                        {
                            this->cell_matrix(i,j) +=
                                    A.template d1<row>( std::make_tuple(column_value_view.value(j, q_index),
                                                                    column_value_view.gradient(j, q_index)) ) *
                                    column_value_view.gradient(i, q_index) * this->row_fe_values.JxW(q_index);
                        }
                    }
                }


            private:
                std::vector<Value> old_solution_values_;
                std::vector<Gradient> old_solution_gradients_;
            };

            template <int dim, class VariableDims, int row=VariableDims::size-1, int column=VariableDims::size-1>
            struct OperatorUpdate
            {
                explicit OperatorUpdate(const VectorSpace& domain,
                                          const dealii::FESystem<dim>& fe_system)
                    : impl_(domain, fe_system),
                      next_(domain, fe_system)
                {}

                void init_old_solution(const dealii::Vector<double>& x)
                {
                    impl_.init_old_solution(x);
                    next_.init_old_solution(x);
                }

                void init_old_solution(const dealii::BlockVector<double>& x)
                {
                    impl_.init_old_solution(x);
                    next_.init_old_solution(x);
                }

                template <class FunGOperator>
                void update_functional(FunGOperator& A, unsigned q_index)
                {
                    impl_.update_functional(A, q_index);
                    next_.update_functional(A, q_index);
                }

                template <class CellIterator>
                void reinit(const CellIterator& cell)
                {
                    impl_.reinit(cell);
                    next_.reinit(cell);
                }

                template <class FunGOperator, class Index>
                void update_gradient_and_hessian(FunGOperator& A, Index q_index)
                {
                    impl_.update_gradient_and_hessian(A, q_index);
                    next_.update_gradient_and_hessian(A, q_index);
                }

                template <class Value, class Gradient>
                void transfer_local_data(Value& value, Gradient& gradient)
                {
                    impl_.transfer_local_data(value, gradient);
                    next_.transfer_local_data(value, gradient);
                }

                LocalFunGOperatorAssembly<dim, VariableDims, row, column> impl_;
                std::conditional_t<row==0,
                                   OperatorUpdate<dim, VariableDims, VariableDims::size-1, column-1>,
                                   OperatorUpdate<dim, VariableDims, row-1, column> > next_;
            };

            template <int dim, class VariableDims, int row>
            struct OperatorUpdate<dim, VariableDims, row, -1>
            {
                explicit OperatorUpdate(const VectorSpace&,
                                          const dealii::FESystem<dim>&)
                {}

                template <class CellIterator>
                void reinit(const CellIterator&)
                {}

                template <class Vector>
                void init_old_solution(const Vector&)
                {}

                template <class FunGOperator>
                void update_functional(FunGOperator&, unsigned)
                {}

                template <class FunGOperator, class Index>
                void update_gradient_and_hessian(FunGOperator&, Index)
                {}

                template <class Value, class Gradient>
                void transfer_local_data(Value&, Gradient&)
                {}
            };
        }
    }
}
