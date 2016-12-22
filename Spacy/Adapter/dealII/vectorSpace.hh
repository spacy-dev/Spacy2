#pragma once

#include <deal.II/dofs/dof_tools.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_system.h>
#include <deal.II/fe/fe_values_extractors.h>
#include <deal.II/grid/tria.h>
#include <deal.II/lac/vector.h>
// For boundary values
#include <deal.II/base/function.h>
#include <deal.II/numerics/vector_tools.h>

#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/ProductSpace/vectorSpace.hh>

#include "vector.hh"

#include <map>
#include <memory>

namespace Spacy
{
    /** @addtogroup dealIIGroup, VectorSpaceGroup @{ */
    namespace dealII
    {
        template <int dim>
        struct BoundaryPart
        {
            BoundaryPart(dealii::types::boundary_id boundary_id,
                         std::shared_ptr< dealii::Function<dim, double> >&& boundary_function)
                : id(boundary_id),
                  function(std::move(boundary_function))
            {}

            dealii::types::boundary_id id;
            std::shared_ptr< dealii::Function<dim, double> > function;
        };

        /// %Vector creator for the deal.II library.
        template <int dim, int variable_dim>
        class VectorCreator
        {
        public:
            VectorCreator(const dealii::Triangulation<dim>& triangulation,
                          int fe_order,
                          int first_component,
                          std::vector< BoundaryPart<dim> >&& boundary_values)
                : fe_order_(fe_order),
                  fe_(std::make_shared< dealii::FE_Q<dim> >(fe_order_)),
                  fe_system_(std::make_shared< dealii::FESystem<dim> >(*fe_, variable_dim)),
                  boundary_values_(std::make_shared< std::vector< BoundaryPart<dim> > >(std::move(boundary_values))),
                  first_component_(first_component)
            {
                triangulation_->copy_triangulation(triangulation);
                dof_handler_ = std::make_shared< dealii::DoFHandler<dim> >(*triangulation_);
                dof_handler_->distribute_dofs(*fe_system_);
            }

            VectorCreator(const dealii::Triangulation<dim>& triangulation, int fe_order, int first_component=0)
                : VectorCreator(triangulation, fe_order, first_component,
                                {BoundaryPart<dim>{dealii::types::boundary_id(0),
                                                   std::make_shared< dealii::ZeroFunction<dim> >(variable_dim)}})
            {}

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
                return *fe_;
            }

            auto extractor() const
            {
                return std::conditional_t<variable_dim==1,
                                          dealii::FEValuesExtractors::Scalar,
                                          dealii::FEValuesExtractors::Vector>(first_component_);
            }

            int firstComponent() const
            {
                return first_component_;
            }

            auto boundaryValues(const dealii::DoFHandler<dim>& dof_handler) const
            {
                std::map<dealii::types::global_dof_index,double> boundary_values;
                for(const auto& value : *boundary_values_)
                    dealii::VectorTools::interpolate_boundary_values( dof_handler,
                                                                      value.id,
                                                                      *value.function,
                                                                      boundary_values );
                return boundary_values;
            }

            const auto& triangulation() const
            {
                return *triangulation_;
            }

        private:
            int fe_order_;
            std::shared_ptr< dealii::Triangulation<dim> > triangulation_ = std::make_shared< dealii::Triangulation<dim> >();
            std::shared_ptr< dealii::FE_Q<dim> >        fe_;
            std::shared_ptr< dealii::FESystem<dim> >    fe_system_;
            std::shared_ptr< dealii::DoFHandler<dim> >      dof_handler_ = nullptr;
            std::shared_ptr< std::vector< BoundaryPart<dim> > > boundary_values_ = std::make_shared< std::vector< BoundaryPart<dim> > >();
            int first_component_;
        };



        template <int variable_dim, int dim>
        VectorSpace makeHomogeneousDirichletHilbertSpace(const dealii::Triangulation<dim>& triangulation, int fe_order, int first_component=0)
        {
            return Spacy::makeHilbertSpace(VectorCreator<dim,variable_dim>(triangulation, fe_order, first_component,
                            {BoundaryPart<dim>{dealii::types::boundary_id(0),
                                               std::make_shared< dealii::ZeroFunction<dim> >(variable_dim)}}),
                                           [](const Spacy::Vector& x, const Spacy::Vector& y)
                                           { return x(y); });
        }

        /**
         * @brief Convenient generation of a single vector space from dealii::Triangulation<dim>.
         * @param triangulation triangulation underlying the FE-space
         * @param fe_order order of the finite elements
         * @param variable_dim dimension of the variable
         * @return @ref ::Spacy::makeHilbertSpace() "::Spacy::makeHilbertSpace( VectorCreator<dim>{triangulation, fe:order} , l2Product{} )"
         */
        template <int variable_dim, int dim>
        VectorSpace makeHilbertSpace(const dealii::Triangulation<dim>& triangulation, int fe_order, int first_component=0)
        {
            return Spacy::makeHilbertSpace(VectorCreator<dim, variable_dim>{triangulation, fe_order, first_component},
                                           [](const Spacy::Vector& x, const Spacy::Vector& y)
                                           { return x(y); });
        }

        /**
         * @brief Convenient generation of a product vector space from dealii::Triangulation<dim>.
         * @param triangulation triangulation underlying the FE-space
         * @param fe_order order of the finite elements
         */
        template <int dim, int... variable_dims>
        VectorSpace makeHilbertSpace(const dealii::Triangulation<dim>& triangulation, const std::vector< std::pair<int,int> >& fe_order_and_first_component)
        {
            std::vector< std::shared_ptr<VectorSpace> > spaces =
            { std::make_shared<VectorSpace>(makeHilbertSpace<variable_dims,dim>(triangulation,
              fe_order_and_first_component[variable_dims].first,
              fe_order_and_first_component[variable_dims].second))... };

            return ProductSpace::makeHilbertSpace(spaces);
        }

        /**
         * @brief Convenient generation of a product vector space from dealii::Triangulation<dim>.
         * @param triangulation triangulation underlying the FE-space
         * @param global_ids global ids associated with variables (for reordering, subspace projections,...)
         * @param fe_order order of the finite elements
         */
//        template <int dim>
//        VectorSpace makeHilbertSpace(dealii::Triangulation<dim>& triangulation,
//                                     const std::vector<unsigned>& global_ids,
//                                     int fe_order)
//        {
//            std::vector< std::shared_ptr<VectorSpace> > spaces;
//            spaces.reserve(global_ids.size());
//            for(auto i=0u; i<global_ids.size(); ++i)
//                spaces.emplace_back(std::make_shared<VectorSpace>(makeHilbertSpace(triangulation, fe_order)));
//            return ProductSpace::makeHilbertSpace(spaces, global_ids);
//        }

        /**
         * @brief Convenient generation of a primal-dual product vector space from dealii::Triangulation<dim>.
         * @param triangulation triangulation underlying the FE-space
         * @param primal_ids global ids associated with primal variables
         * @param dual_ids global ids associated with dual variables
         * @param fe_order order of the finite elements
         */
//        template <int dim>
//        VectorSpace makeHilbertSpace(dealii::Triangulation<dim>& triangulation,
//                                     const std::vector<unsigned>& primal_ids,
//                                     const std::vector<unsigned>& dual_ids,
//                                     int fe_order)
//        {
//            std::vector< std::shared_ptr<VectorSpace> > spaces;
//            spaces.reserve(primal_ids.size() + dual_ids.size());
//            for(auto i=0u; i<primal_ids.size() + dual_ids.size(); ++i)
//                spaces.emplace_back(std::make_shared<VectorSpace>(makeHilbertSpace(triangulation, fe_order)));
//            return ProductSpace::makeHilbertSpace(spaces, primal_ids, dual_ids);
//        }
    }
    /** @} */
}
