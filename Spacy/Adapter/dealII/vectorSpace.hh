#pragma once

#include <deal.II/dofs/dof_tools.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/grid/tria.h>
#include <deal.II/lac/sparsity_pattern.h>
#include <deal.II/lac/block_sparsity_pattern.h>
#include <deal.II/lac/dynamic_sparsity_pattern.h>
#include <deal.II/lac/vector.h>

#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/ProductSpace/vectorSpace.hh>

#include "vector.hh"

#include <memory>

namespace Spacy
{
    /** @addtogroup dealIIGroup, VectorSpaceGroup @{ */
    namespace dealII
    {
        /// %Vector creator for the deal.II library.
        template <int dim>
        class VectorCreator
        {
        public:
            explicit VectorCreator(dealii::Triangulation<dim>& triangulation, int fe_order)
                : fe_order_(fe_order),
                  finite_element_(std::make_shared< dealii::FE_Q<dim> >(fe_order_)),
                  dof_handler_(std::make_shared< dealii::DoFHandler<dim> >(triangulation)),
                  sparsity_pattern_(std::make_shared<dealii::BlockSparsityPattern>())
            {
                dof_handler_->distribute_dofs(*finite_element_);

                dealii::BlockDynamicSparsityPattern dsp(1,1);
                dsp.block(0, 0).reinit (dof_handler_->n_dofs(), dof_handler_->n_dofs());
                dsp.collect_sizes ();

                dealii::DoFTools::make_sparsity_pattern (*dof_handler_, dsp);
                sparsity_pattern_->copy_from(dsp);
            }

            ::Spacy::Vector operator()(const VectorSpace* space) const
            {
                return Vector(dealii::Vector<double>(degreesOfFreedom()), *space);
            }

            auto FEOrder() const
            {
                return fe_order_;
            }

            auto degreesOfFreedom() const
            {
                return dof_handler_->n_dofs();
            }

            const auto& dofHandler() const
            {
                return *dof_handler_;
            }

            const auto& finiteElement() const
            {
                return *finite_element_;
            }

            const auto& sparsityPattern() const
            {
                return *sparsity_pattern_;
            }

        private:
            int fe_order_;
            std::shared_ptr< dealii::FE_Q<dim> >          finite_element_;
            std::shared_ptr< dealii::DoFHandler<dim> >    dof_handler_;
            std::shared_ptr<dealii::BlockSparsityPattern> sparsity_pattern_;
        };

        /**
         * @brief Convenient generation of a single vector space from dealii::Triangulation<dim>.
         * @param triangulation triangulation underlying the FE-space
         * @param fe_order order of the finite elements
         * @return @ref ::Spacy::makeHilbertSpace() "::Spacy::makeHilbertSpace( VectorCreator<dim>{triangulation, fe:order} , l2Product{} )"
         */
        template <int dim>
        VectorSpace makeHilbertSpace(dealii::Triangulation<dim>& triangulation, int fe_order)
        {
            return Spacy::makeHilbertSpace(VectorCreator<dim>{triangulation, fe_order},
                                           [](const Spacy::Vector& x, const Spacy::Vector& y)
                                           { return x(y); });
        }

        /**
         * @brief Convenient generation of a product vector space from dealii::Triangulation<dim>.
         * @param triangulation triangulation underlying the FE-space
         * @param fe_order order of the finite elements
         */
        template <int dim>
        VectorSpace makeHilbertSpace(dealii::Triangulation<dim>& triangulation, unsigned number_of_variables, int fe_order)
        {
            std::vector< std::shared_ptr<VectorSpace> > spaces;
            spaces.reserve(number_of_variables);
            for(auto i=0u; i<number_of_variables; ++i)
                spaces.emplace_back(std::make_shared<VectorSpace>(makeHilbertSpace(triangulation, fe_order)));
            return ProductSpace::makeHilbertSpace(spaces);
        }

        /**
         * @brief Convenient generation of a product vector space from dealii::Triangulation<dim>.
         * @param triangulation triangulation underlying the FE-space
         * @param global_ids global ids associated with variables (for reordering, subspace projections,...)
         * @param fe_order order of the finite elements
         */
        template <int dim>
        VectorSpace makeHilbertSpace(dealii::Triangulation<dim>& triangulation,
                                     const std::vector<unsigned>& global_ids,
                                     int fe_order)
        {
            std::vector< std::shared_ptr<VectorSpace> > spaces;
            spaces.reserve(global_ids.size());
            for(auto i=0u; i<global_ids.size(); ++i)
                spaces.emplace_back(std::make_shared<VectorSpace>(makeHilbertSpace(triangulation, fe_order)));
            return ProductSpace::makeHilbertSpace(spaces, global_ids);
        }

        /**
         * @brief Convenient generation of a primal-dual product vector space from dealii::Triangulation<dim>.
         * @param triangulation triangulation underlying the FE-space
         * @param primal_ids global ids associated with primal variables
         * @param dual_ids global ids associated with dual variables
         * @param fe_order order of the finite elements
         */
        template <int dim>
        VectorSpace makeHilbertSpace(dealii::Triangulation<dim>& triangulation,
                                     const std::vector<unsigned>& primal_ids,
                                     const std::vector<unsigned>& dual_ids,
                                     int fe_order)
        {
            std::vector< std::shared_ptr<VectorSpace> > spaces;
            spaces.reserve(primal_ids.size() + dual_ids.size());
            for(auto i=0u; i<primal_ids.size() + dual_ids.size(); ++i)
                spaces.emplace_back(std::make_shared<VectorSpace>(makeHilbertSpace(triangulation, fe_order)));
            return ProductSpace::makeHilbertSpace(spaces, primal_ids, dual_ids);
        }
    }
    /** @} */
}
