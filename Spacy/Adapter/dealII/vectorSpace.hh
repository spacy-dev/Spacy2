#pragma once

#include <deal.II/dofs/dof_tools.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/grid/tria.h>
#include <deal.II/lac/sparsity_pattern.h>
#include <deal.II/lac/dynamic_sparsity_pattern.h>
#include <deal.II/lac/vector.h>

#include <Spacy/vectorSpace.hh>

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
                  sparsity_pattern_(std::make_shared<dealii::SparsityPattern>())
            {
                dof_handler_->distribute_dofs(*finite_element_);
                dealii::DynamicSparsityPattern dsp(dof_handler_->n_dofs());
                dealii::DoFTools::make_sparsity_pattern(*dof_handler_, dsp);
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
            std::shared_ptr<dealii::SparsityPattern>      sparsity_pattern_;
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
    }
    /** @} */
}
