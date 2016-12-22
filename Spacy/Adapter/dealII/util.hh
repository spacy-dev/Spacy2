#pragma once

#include <deal.II/fe/fe_values.h>
#include <deal.II/lac/vector.h>
#include <deal.II/lac/block_vector.h>
#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/block_sparse_matrix.h>
#include <deal.II/lac/dynamic_sparsity_pattern.h>
#include <deal.II/lac/full_matrix.h>
#include <deal.II/base/quadrature_lib.h>
#include <deal.II/numerics/matrix_tools.h>

#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Spaces/productSpace.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/invoke.hh>
#include "vectorSpace.hh"

#include <type_traits>

namespace Spacy
{
    namespace dealII
    {
        template <class VariableDims, int size = VariableDims::size>
        struct Traits
        {
            using Vector = dealii::BlockVector<double>;
            using Matrix = dealii::BlockSparseMatrix<double>;
            using SparsityPattern = dealii::BlockSparsityPattern;
        };

        template <class VariableDims>
        struct Traits<VariableDims, 1>
        {
            using Vector = dealii::Vector<double>;
            using Matrix = dealii::SparseMatrix<double>;
            using SparsityPattern = dealii::SparsityPattern;
        };

        template <int... variable_dims>
        struct VariableDim;

        template <int variable_dim, int... variable_dims>
        struct VariableDim<variable_dim, variable_dims...>
        {
            static const constexpr int size = 1 + VariableDim<variable_dims...>::size;
            using Indices = std::make_integer_sequence<int,size>;
        };

        template <int variable_dim>
        struct VariableDim<variable_dim>
        {
            static const constexpr int size = 1;
            using Indices = std::make_integer_sequence<int,1>;
        };

        template <int id, class Components, int id_in_component=0>
        struct GetDim : std::integral_constant<int,-1>
        {};

        template <int id, int id_in_component, int dim, template <int...> class Components, int... ids>
        struct GetDim<id, Components<dim, ids...>, id_in_component>
        {
            static const constexpr int value = id==id_in_component ? dim : GetDim<id, VariableDim<ids...>, id_in_component+1>::value;
        };

        template <int... variable_dims>
        struct CountDimsImpl;

        template <int variable_dim, int... variable_dims>
        struct CountDimsImpl<variable_dim, variable_dims...>
        {
            static const constexpr int value = variable_dim + CountDimsImpl<variable_dims...>::value;
        };

        template <int variable_dim>
        struct CountDimsImpl<variable_dim>
        {
            static const constexpr int value = variable_dim;
        };

        template <class>
        struct CountDims;

        template <int... variable_dims>
        struct CountDims< VariableDim<variable_dims...> >
        {
            static const constexpr int value = CountDimsImpl<variable_dims...>::value;
        };


        template <class StartIterator, class EndIterator, class Functor>
        void for_each( StartIterator start, EndIterator end, Functor f)
        {
            for(;start!=end; ++start)
                f(start);
        }


        template <int dim, class VariableDims, int id = VariableDims::size-1>
        struct GetBoundaryMap
        {
            static const constexpr int variable_dim = GetDim<id,VariableDims>::value;

            static void apply(const VectorSpace& space,
                              const dealii::DoFHandler<dim>& dof_handler,
                              std::map<dealii::types::global_dof_index,double>& boundary_map)
            {
                const auto& creator_ = space.creator();

                if( is< VectorCreator<dim,variable_dim> >(creator_) ) {
                    auto local_map = cast_ref< VectorCreator<dim,variable_dim> >(creator_).boundaryValues(dof_handler);
                    for_each(begin(local_map), end(local_map),
                             [&boundary_map](const auto& entry)
                    {
                        boundary_map[entry.first] = entry.second;
                    });
                    GetBoundaryMap<dim, VariableDims, id-1>::apply(space, dof_handler, boundary_map);
                }

                if( is<ProductSpace::VectorCreator>(creator_)) {
                    const auto& product_creator = cast_ref<ProductSpace::VectorCreator>(creator_);
                    for(const auto& subSpace : product_creator.subSpaces())
                        GetBoundaryMap<dim, VariableDims, id>::apply(*subSpace, dof_handler, boundary_map);
                }
            }
        };

        template <int dim, class VariableDims>
        struct GetBoundaryMap<dim, VariableDims, -1>
        {
            static void apply(const VectorSpace&,
                              const dealii::DoFHandler<dim>&,
                              std::map<dealii::types::global_dof_index,double>&)
            {}
        };


        template <int dim, class VariableDims>
        auto get_boundary_map(const VectorSpace& space,
                              const dealii::DoFHandler<dim>& dof_handler)
        {
            std::map<dealii::types::global_dof_index,double> boundary_map;
            GetBoundaryMap<dim,VariableDims>::apply(space, dof_handler, boundary_map);
            return boundary_map;
        }

        template <int dim, class VariableDims, int id>
        struct GetFiniteElement
        {
            static auto apply(const VectorSpace& V)
            {
                const constexpr int variable_dim = GetDim<id,VariableDims>::value;
                return std::make_tuple( creator< VectorCreator<dim,variable_dim> >(extractSubSpace(V,id)).finiteElement(),
                                                        variable_dim );
            }
        };


        template <int,class>
        struct GetFiniteElementSystem;

        template <int dim, int... variable_dims>
        struct GetFiniteElementSystem<dim, VariableDim<variable_dims...> >
        {
            using VariableDims = VariableDim<variable_dims...>;

            static auto apply(const VectorSpace& V)
            {
                return create< dealii::FESystem<dim> >( get_args(V, typename VariableDims::Indices()) );
            }

        private:
            template <int... indices>
            static auto get_args(const VectorSpace& V, std::integer_sequence<int, indices...>)
            {
                return std::tuple_cat( GetFiniteElement<dim,VariableDims,indices>::apply(V)... );
            }
        };


        template <int dim, class VariableDims>
        auto get_finite_element_system(const VectorSpace& V)
        {
            return GetFiniteElementSystem<dim,VariableDims>::apply(V);
        }


        namespace Detail
        {
            template <int dim, class ValueView, int row_dim, class Index>
            auto get_value_impl(const ValueView& value_view, Index i, Index q_index,
                                std::integral_constant<int, row_dim>)
            {
                dealii::Tensor<1,row_dim> value;
                value = value_view.value(i, q_index);
                return value;
            }

            template <int dim, class ValueView, class Index>
            auto get_value_impl(const ValueView& value_view, Index i, Index q_index,
                                std::integral_constant<int, 1>)
            {
                return value_view.value(i, q_index);
            }

            template <int dim, int row_dim, class ValueView, class Index>
            auto get_value(const ValueView& value_view, Index i, Index q_index)
            {
                return get_value_impl<dim>(value_view, i, q_index, std::integral_constant<int,row_dim>());
            }

            template <int dim, int row, int row_dim, int column=row, int column_dim=row_dim>
            struct LocalAssemblyBase
            {
                explicit LocalAssemblyBase(const VectorSpace& space_row,
                                           const dealii::FESystem<dim>& fe_system)
                    : space_row_(extractSubSpace(space_row, row)),
                      space_column_(space_row_),
                      row_space_creator_( creator< VectorCreator<dim,row_dim> >(space_row_) ),
                      column_space_creator_( creator< VectorCreator<dim,column_dim> >(space_column_) ),
                      quadrature_formula(row_space_creator_.FEOrder() + column_space_creator_.FEOrder()),
                      row_fe_values(fe_system, quadrature_formula,
                                    dealii::update_values | dealii::update_gradients | dealii::update_JxW_values),
                      column_fe_values(fe_system, quadrature_formula,
                                       dealii::update_values | dealii::update_gradients | dealii::update_JxW_values),
                      row_dofs_per_cell_(fe_system.dofs_per_cell),
                      column_dofs_per_cell_(fe_system.dofs_per_cell),
                      n_q_points_(quadrature_formula.size()),
                      cell_rhs(row_dofs_per_cell_),
                      cell_matrix(row_dofs_per_cell_,column_dofs_per_cell_),
                      row_local_dof_indices_(row_dofs_per_cell_),
                      column_local_dof_indices_(column_dofs_per_cell_)
                {}

//                explicit LocalAssemblyBase(const VectorSpace& space_row,
//                                           const VectorSpace& space_column)
//                    : space_row_(extractSubSpace(space_row, row)),
//                      space_column_(extractSubSpace(space_column, column)),
//                      row_space_creator_( creator< VectorCreator<dim,row_dim> >(space_row_) ),
//                      column_space_creator_( creator< VectorCreator<dim,column_dim> >(space_column_) ),
//                      quadrature_formula(row_space_creator_.FEOrder() + column_space_creator_.FEOrder()),
//                      row_fe_values(row_space_creator_.finiteElement(), quadrature_formula,
//                                    dealii::update_values | dealii::update_gradients | dealii::update_JxW_values),
//                      column_fe_values(column_space_creator_.finiteElement(), quadrature_formula,
//                                       dealii::update_values | dealii::update_gradients | dealii::update_JxW_values),
//                      row_dofs_per_cell_(row_space_creator_.finiteElement().dofs_per_cell),
//                      column_dofs_per_cell_(column_space_creator_.finiteElement().dofs_per_cell),
//                      n_q_points_(quadrature_formula.size()),
//                      cell_rhs(row_dofs_per_cell_),
//                      cell_matrix(row_dofs_per_cell_,column_dofs_per_cell_),
//                      row_local_dof_indices_(row_dofs_per_cell_),
//                      column_local_dof_indices_(column_dofs_per_cell_)
//                {}

                template <class CellIterator>
                void reinit(const CellIterator& cell)
                {
                    row_fe_values.reinit(cell);
                    cell->get_dof_indices(row_local_dof_indices_);
                    column_fe_values.reinit(cell);
                    cell->get_dof_indices(column_local_dof_indices_);

                    cell_rhs = 0;
                    cell_matrix = 0;
                }

                void transfer_local_data(dealii::Vector<double>& gradient, dealii::SparseMatrix<double>& hessian)
                {
                    for(auto i=0u; i<row_dofs_per_cell_; ++i)
                    {
                        gradient[row_local_dof_indices_[i]] += cell_rhs(i);
                        for(auto j=0u; j<column_dofs_per_cell_; ++j)
                            hessian.add(row_local_dof_indices_[i],
                                        column_local_dof_indices_[j],
                                        cell_matrix(i,j));
                    }
                }

                void transfer_local_data(dealii::BlockVector<double>& gradient, dealii::BlockSparseMatrix<double>& hessian)
                {
                    for(auto i=0u; i<row_dofs_per_cell_; ++i)
                    {
                        gradient.block(row)[row_local_dof_indices_[i]] += cell_rhs(i);

                        for(auto j=0u; j<column_dofs_per_cell_; ++j)
                            hessian.block(row,column).add(row_local_dof_indices_[i],
                                                          column_local_dof_indices_[j],
                                                          cell_matrix(i,j));
                    }
                }

                const VectorSpace& space_row_;
                const VectorSpace& space_column_;
                const VectorCreator<dim,row_dim>& row_space_creator_;
                const VectorCreator<dim,column_dim>& column_space_creator_;
                const dealii::QGauss<dim> quadrature_formula;
                dealii::FEValues<dim> row_fe_values;
                dealii::FEValues<dim> column_fe_values;
                const unsigned row_dofs_per_cell_;
                const unsigned column_dofs_per_cell_;
                const unsigned n_q_points_;
                dealii::Vector<double> cell_rhs;
                dealii::FullMatrix<double> cell_matrix;
                std::vector<dealii::types::global_dof_index> row_local_dof_indices_;
                std::vector<dealii::types::global_dof_index> column_local_dof_indices_;
            };
        }
    }
}
