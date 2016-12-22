#pragma once

#include <deal.II/lac/vector.h>
#include <deal.II/lac/block_vector.h>
#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/block_sparse_matrix.h>
#include <deal.II/lac/dynamic_sparsity_pattern.h>

#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Spaces/productSpace.hh>

#include "util.hh"
#include "vectorSpace.hh"

namespace Spacy
{
    namespace dealII
    {
        template <int dim, class VariableDims, int id=VariableDims::size-1, bool is_product_space = VariableDims::size!=1>
        struct InitBlockVector
        {
            static const constexpr int variable_dim = GetDim<id,VariableDims>::value;

            static void apply(const VectorSpace& V, dealii::BlockVector<double>& x)
            {
                x.block(id).reinit(creator< VectorCreator<dim,variable_dim> >(extractSubSpace(V,id)).degreesOfFreedom());
                InitBlockVector<dim,VariableDims,id-1,is_product_space>::apply(V,x);
            }
        };

        template <int dim, class VariableDims, bool is_product_space>
        struct InitBlockVector<dim,VariableDims,-1,is_product_space>
        {
            static void apply(const VectorSpace&, dealii::BlockVector<double>&)
            {}
        };

        template <int dim, class VariableDims>
        struct InitBlockVector<dim,VariableDims,0,false>
        {
            static const constexpr int variable_dim = GetDim<0,VariableDims>::value;

            static void apply(const VectorSpace& V, dealii::Vector<double>& x)
            {
                x.reinit(creator< VectorCreator<dim,variable_dim> >(V).degreesOfFreedom());
            }
        };


        template <int dim, class VariableDims, int row=VariableDims::size-1, int column=VariableDims::size-1>
        struct InitBlocks
        {
            static void apply(const VectorSpace& V,
                              dealii::BlockDynamicSparsityPattern& dsp)
            {
                dsp.block(row,column).reinit( creator< VectorCreator<dim, GetDim<row,VariableDims>::value> >(extractSubSpace(V,row)).degreesOfFreedom(),
                                              creator< VectorCreator<dim, GetDim<column,VariableDims>::value> >(extractSubSpace(V,column)).degreesOfFreedom() );
                InitBlocks<dim, VariableDims,
                        row==0 ? VariableDims::size-1 : row-1,
                        row==0 ? column-1 : column>::apply(V,dsp);
            }
        };


        template <int dim, class VariableDims, int row>
        struct InitBlocks<dim,VariableDims,row,-1>
        {
            static void apply(const VectorSpace&,
                              dealii::BlockDynamicSparsityPattern&)
            {}
        };


        template <int dim, class VariableDims, int size=VariableDims::size>
        struct InitSparsityPattern
        {
            static void apply(const VectorSpace& V,
                              const dealii::DoFHandler<dim>& dof_handler,
                              dealii::BlockSparsityPattern& sparsity_pattern)
            {
                dealii::BlockDynamicSparsityPattern dsp(size,size);
                InitBlocks<dim,VariableDims>::apply(V,dsp);
                dsp.collect_sizes();
                dealii::DoFTools::make_sparsity_pattern (dof_handler, dsp);
                sparsity_pattern.copy_from(dsp);
            }

        };

        template <int dim, class VariableDims>
        struct InitSparsityPattern<dim,VariableDims,1>
        {
            static void apply(const VectorSpace&,
                              const dealii::DoFHandler<dim>& dof_handler,
                              dealii::SparsityPattern& sparsity_pattern)
            {
                dealii::DynamicSparsityPattern dsp(dof_handler.n_dofs());
                dealii::DoFTools::make_sparsity_pattern(dof_handler, dsp);
                sparsity_pattern.copy_from(dsp);
            }
        };
    }
}
