#pragma once

#include <deal.II/base/tensor.h>
#include <deal.II/grid/tria.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/fe/fe_values.h>
#include <deal.II/lac/vector.h>
#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/block_sparse_matrix.h>
#include <deal.II/lac/block_vector.h>
#include <deal.II/lac/dynamic_sparsity_pattern.h>
// For boundary values
#include <deal.II/numerics/matrix_tools.h>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/Base/functionalBase.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/Exceptions/callOfUndefinedFunctionException.hh>

#include "linearOperator.hh"
#include "copy.hh"
#include "util.hh"
#include "vector.hh"
#include "vectorSpace.hh"

#include <algorithm>
#include <memory>
#include <vector>
#include <type_traits>

namespace Spacy
{
    namespace dealII
    {
        namespace Detail
        {
            template <int dim, class Components, int row, int column,
                      int row_dim = GetDim<row, Components>::value,
                      int column_dim = GetDim<column, Components>::value>
            struct LocalFunGFunctionalDerivativeAssembly : LocalAssemblyBase<dim,row,row_dim,column,column_dim>
            {
                using Value = std::conditional_t<row_dim==1, double, dealii::Tensor<1,dim> >;
                using Gradient = std::conditional_t<row_dim==1, dealii::Tensor<1,dim>, dealii::Tensor<2,dim> >;

                explicit LocalFunGFunctionalDerivativeAssembly(const VectorSpace& domain,
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

                template <class FunGFunctional, class Index>
                void update_functional(FunGFunctional& functional, Index q_index)
                {
                    functional.template update<row>(std::make_tuple(old_solution_values_[q_index],
                                                                    old_solution_gradients_[q_index]));
                }

                template <class FunGFunctional, class Index>
                void update_gradient_and_hessian(FunGFunctional& functional, Index q_index)
                {
                    const auto& row_value_view = this->row_fe_values[this->row_space_creator_.extractor()];
                    const auto& column_value_view = this->column_fe_values[this->column_space_creator_.extractor()];
                    for(Index i=0; i<this->row_dofs_per_cell_; ++i)
                    {
                        this->cell_rhs(i) += functional.template d1<row>(
                                                std::make_tuple(get_value<dim,row_dim>(row_value_view, i, q_index),
                                                                row_value_view.gradient(i, q_index))
                                           ) * this->row_fe_values.JxW(q_index);

                        for(Index j=0; j<this->column_dofs_per_cell_; ++j)
                        {
                            this->cell_matrix(i,j) += functional.template d2<row,column>(
                                                    std::make_tuple(get_value<dim,row_dim>(row_value_view, i, q_index),
                                                                    row_value_view.gradient(i, q_index)),
                                                    std::make_tuple(get_value<dim,column_dim>(column_value_view, j, q_index),
                                                                    column_value_view.gradient(j, q_index))
                                                ) * this->row_fe_values.JxW(q_index);
                        }
                    }
                }


            private:
                std::vector<Value> old_solution_values_;
                std::vector<Gradient> old_solution_gradients_;
            };


            template <int dim, class Components, int id, int variable_dim = GetDim<id, Components>::value>
            struct LocalFunGFunctionalValueAssembly;

            template <int dim, class Components, int id>
            struct LocalFunGFunctionalValueAssembly<dim, Components, id, dim> : LocalAssemblyBase<dim,id,dim>
            {
                static const constexpr int variable_dim = GetDim<id, Components>::value;
                using Value = dealii::Tensor<1,dim>;
                using Gradient = dealii::Tensor<2,dim>;

                explicit LocalFunGFunctionalValueAssembly(const VectorSpace& domain, const dealii::FESystem<dim>& fe_system)
                    : LocalAssemblyBase<dim,id,dim>(domain, fe_system),
                      old_solution_values_(this->n_q_points_),
                      old_solution_gradients_(this->n_q_points_)
                {}

                void init_old_solution(const dealii::Vector<double>& x)
                {
                    this->row_fe_values[this->row_space_creator_.extractor()].get_function_values(x, old_solution_values_);
                    this->row_fe_values[this->row_space_creator_.extractor()].get_function_gradients(x, old_solution_gradients_);
                }


                template <class FunGFunctional, class Index>
                void update_functional(FunGFunctional& functional, Index q_index)
                {
                    functional.template update<id>(std::make_tuple(old_solution_values_[q_index],
                                                                   old_solution_gradients_[q_index]));
                }

            private:
                std::vector<Value> old_solution_values_;
                std::vector<Gradient> old_solution_gradients_;
            };

            template <int dim, class Components, int id>
            struct LocalFunGFunctionalValueAssembly<dim, Components, id, 1> : LocalAssemblyBase<dim,id,1>
            {
                static const constexpr int variable_dim = 1;
                using Value = double;
                using Gradient = dealii::Tensor<variable_dim,dim>;

                explicit LocalFunGFunctionalValueAssembly(const VectorSpace& domain, const dealii::FESystem<dim>& fe_system)
                    : LocalAssemblyBase<dim, id, 1> (domain, fe_system),
                      old_solution_values_(this->n_q_points_),
                      old_solution_gradients_(this->n_q_points_)
                {}

                void init_old_solution(const dealii::Vector<double>& x)
                {
                    this->row_fe_values[this->row_space_creator_.extractor()].get_function_values(x, old_solution_values_);
                    this->row_fe_values[this->row_space_creator_.extractor()].get_function_gradients(x, old_solution_gradients_);
                }


                template <class FunGFunctional, class Index>
                void update_functional(FunGFunctional& functional, Index q_index)
                {
                    functional.template update<id>(std::make_tuple(old_solution_values_[q_index],
                                                                   old_solution_gradients_[q_index]));
                }

            private:
                std::vector<Value> old_solution_values_;
                std::vector<Gradient> old_solution_gradients_;
            };

            template <int dim, class VariableDims, int id = VariableDims::size-1>
            struct FunctionalUpdate
            {
                explicit FunctionalUpdate(const VectorSpace& domain,
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
                    impl_.init_old_solution(x.block(id));
                    next_.init_old_solution(x);
                }

                template <class FunGFunctional>
                void update_functional(FunGFunctional& functional, unsigned q_index)
                {
                    impl_.update_functional(functional, q_index);
                    next_.update_functional(functional, q_index);
                }

                template <class CellIterator>
                void reinit(const CellIterator& cell)
                {
                    impl_.row_fe_values.reinit(cell);
                    impl_.column_fe_values.reinit(cell);
                    next_.reinit(cell);
                }

                Detail::LocalFunGFunctionalValueAssembly<dim, VariableDims, id> impl_;
                FunctionalUpdate<dim,VariableDims,id-1> next_;
            };

            template <int dim, class VariableDims>
            struct FunctionalUpdate<dim, VariableDims, -1>
            {

                explicit FunctionalUpdate(const VectorSpace&,
                                          const dealii::FESystem<dim>&)
                {}

                template <class CellIterator>
                void reinit(const CellIterator&)
                {}

                template <class Vector>
                void init_old_solution(const Vector&)
                {}

                template <class FunGFunctional>
                void update_functional(FunGFunctional&, unsigned)
                {}
            };

            template <int dim, class VariableDims, int row=VariableDims::size-1, int column=VariableDims::size-1>
            struct DerivativeUpdate
            {
                explicit DerivativeUpdate(const VectorSpace& domain,
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

                template <class FunGFunctional>
                void update_functional(FunGFunctional& functional, unsigned q_index)
                {
                    impl_.update_functional(functional, q_index);
                    next_.update_functional(functional, q_index);
                }

                template <class CellIterator>
                void reinit(const CellIterator& cell)
                {
                    impl_.reinit(cell);
                    next_.reinit(cell);
                }

                template <class FunGFunctional, class Index>
                void update_gradient_and_hessian(FunGFunctional& functional, Index q_index)
                {
                    impl_.update_gradient_and_hessian(functional, q_index);
                    next_.update_gradient_and_hessian(functional, q_index);
                }

                template <class Gradient, class Hessian>
                void transfer_local_data(Gradient& gradient, Hessian& hessian)
                {
                    impl_.transfer_local_data(gradient, hessian);
                    next_.transfer_local_data(gradient, hessian);
                }

                LocalFunGFunctionalDerivativeAssembly<dim, VariableDims, row, column> impl_;
                std::conditional_t<row==0,
                                   DerivativeUpdate<dim, VariableDims, VariableDims::size-1, column-1>,
                                   DerivativeUpdate<dim, VariableDims, row-1, column> > next_;
            };

            template <int dim, class VariableDims, int row>
            struct DerivativeUpdate<dim, VariableDims, row, -1>
            {
                explicit DerivativeUpdate(const VectorSpace&,
                                          const dealii::FESystem<dim>&)
                {}

                template <class CellIterator>
                void reinit(const CellIterator&)
                {}

                template <class Vector>
                void init_old_solution(const Vector&)
                {}

                template <class FunGFunctional>
                void update_functional(FunGFunctional&, unsigned)
                {}

                template <class FunGFunctional, class Index>
                void update_gradient_and_hessian(FunGFunctional&, Index)
                {}

                template <class Gradient, class Hessian>
                void transfer_local_data(Gradient&, Hessian&)
                {}
            };
        }
    }
}
