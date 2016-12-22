#pragma once

#include <deal.II/base/tensor.h>
#include <deal.II/grid/tria.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/fe/fe_values.h>
#include <deal.II/lac/vector.h>
#include <deal.II/lac/full_matrix.h>
#include <deal.II/numerics/vector_tools.h>
// For boundary values
#include <deal.II/numerics/matrix_tools.h>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/Base/FunctionalBase.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/Exceptions/callOfUndefinedFunctionException.hh>

#include "linearOperator.hh"
#include "copy.hh"
#include "init.hh"
#include "util.hh"
#include "vector.hh"
#include "vectorSpace.hh"
#include "c2FunGFunctionalAssembly.hh"

#include <algorithm>
#include <memory>
#include <vector>
#include <type_traits>

namespace Spacy
{
    /** @addtogroup dealIIGroup @{ */
    namespace dealII
    {
        template <class FunGFunctional, int dim, class VariableDims = VariableDim<1> >
        class C2FunGFunctional;

        template <class FunGFunctional, int dim, int... variable_dims>
        class C2FunGFunctional< FunGFunctional, dim, VariableDim<variable_dims...> >
                : public FunctionalBase
        {
            using VariableDims = VariableDim<variable_dims...>;
            using Gradient = typename Traits<VariableDims>::Vector;
            using Hessian = typename Traits<VariableDims>::Matrix;
            using SparsityPattern = typename Traits<VariableDims>::SparsityPattern;
            
        public:
            C2FunGFunctional(FunGFunctional&& functional_impl, const VectorSpace& domain)
                : FunctionalBase(domain),
                  functional_(std::move(functional_impl)),
                  boundary_values_(zero(domain)),
                  operator_space_( std::make_shared<VectorSpace>( LinearOperatorCreator(),
                                                                 [](const Spacy::Vector&)
            {
                return Real(0);//cast_ref< LinearOperator<dim,VariableDims> >(v).get()/*.block(max_variable_id,max_variable_id)*/.frobenius_norm();
            } , true ) ),
                  fe_system_(std::make_shared< dealii::FESystem<dim> >(get_finite_element_system<dim,VariableDims>(domain))),
                  dof_handler_(std::make_shared< dealii::DoFHandler<dim> >(creator< VectorCreator<dim,GetDim<0,VariableDims>::value> >(extractSubSpace(domain,0)).triangulation())),
                  gradient_(VariableDims::size==1 ? GetDim<0,VariableDims>::value : VariableDims::size)
            {
                dof_handler_->distribute_dofs(*fe_system_);
                InitBlockVector<dim, VariableDims>::apply(domain, gradient_);
                InitSparsityPattern<dim, VariableDims>::apply(domain, *dof_handler_, sparsity_pattern_);
                hessian_.reinit(sparsity_pattern_);
            }

            C2FunGFunctional(const C2FunGFunctional& other)
                : FunctionalBase(other.domain()),
                  functional_(other.functional_),
                  boundary_values_(other.boundary_values_),
                  operator_space_(other.operator_space_),
                  gradient_(VariableDims::size==1 ? GetDim<0,VariableDims>::value : VariableDims::size),
                  fe_system_(other.fe_system_),
                  dof_handler_(other.dof_handler_)
            {
                InitBlockVector<dim, VariableDims>::apply(domain(), gradient_);
                InitSparsityPattern<dim, VariableDims>::apply(domain(), *dof_handler_, sparsity_pattern_);
                hessian_.reinit(sparsity_pattern_);
            }
            
            /// Compute \f$f(x)\f$.
            Real operator()(const ::Spacy::Vector& x) const
            {
                assemble_value(x);
                return value_;
            }

            /// Compute \f$f'(x)dx\f$.
            ::Spacy::Vector d1(const ::Spacy::Vector &x) const
            {
                assemble(x);

                auto y = zero(domain().dualSpace());
                copy(gradient_, y);
                return y;
            }

            /// Compute \f$f''(x)(dx,dy)\f$.
            ::Spacy::Vector d2(const ::Spacy::Vector &x, const ::Spacy::Vector &dx) const
            {
                assemble(x);

                auto dx_ = gradient_;
                copy(dx, dx_);

                auto y_ = gradient_;
                hessian_.vmult(y_, dx_);

                auto y = zero(domain().dualSpace());
                copy(y_, y);
                return y;
            }
            
            /**
             * @brief Access \f$f''(x)\f$ as linear operator \f$X\rightarrow X^*\f$
             * @see LinearOperator, ::Spacy::LinearOperator
             */
            auto hessian(const ::Spacy::Vector& x) const
            {
                assemble(x);
                return LinearOperator<dim,VariableDims>{ hessian_ , *operator_space_ , boundary_values_, domain(), domain().dualSpace() };
            }
            
        private:
            void assemble_value_impl(const Gradient& x) const
            {
                Detail::FunctionalUpdate<dim,VariableDims> functional_update(domain(), *fe_system_);

                for_each(dof_handler_->begin_active(),
                         dof_handler_->end(),
                         [this,&x,&functional_update]
                         (const auto& cell)
                {
                    functional_update.reinit(cell);
                    functional_update.init_old_solution(x);

                    for(auto q_index=0u; q_index<functional_update.impl_.n_q_points_; ++q_index)
                    {
                        functional_update.update_functional(functional_, q_index);
                        value_ += functional_() * functional_update.impl_.row_fe_values.JxW(q_index);
                    }
                });
            }

            void assemble_derivative_impl(const Gradient& x) const
            {
                Detail::DerivativeUpdate<dim,VariableDims> derivative_update(domain(), *fe_system_);

                for_each(dof_handler_->begin_active(),
                         dof_handler_->end(),
                         [this,&x,&derivative_update]
                         (const auto& cell)
                {
                    derivative_update.reinit(cell);
                    derivative_update.init_old_solution(x);

                    for(auto q_index=0u; q_index<derivative_update.impl_.n_q_points_; ++q_index)
                    {
                        derivative_update.update_functional(functional_, q_index);
                        derivative_update.update_gradient_and_hessian(functional_, q_index);
                    }

                    derivative_update.transfer_local_data(gradient_, hessian_);
                });

                auto boundary_vals = gradient_;
                boundary_vals = 0;

                dealii::MatrixTools::apply_boundary_values(get_boundary_map<dim, VariableDims>(domain(), *dof_handler_),
                                                           hessian_,
                                                           boundary_vals,
                                                           gradient_);
                copy(boundary_vals, boundary_values_);
            }



            /// Assemble discrete representation of \f$f(x)\f$.
            void assemble_value(const ::Spacy::Vector& x) const
            {
                if( oldX_value_ && (oldX_value_ == x) )
                    return;
                value_ = 0;

                auto x_ = gradient_;
                copy(x, x_);
                assemble_value_impl(x_);

                oldX_value_ = x;
            }

            /// Assemble discrete representation of \f$f'(x)\f$ and \f$f''(x)\f$.
            void assemble(const ::Spacy::Vector& x) const
            {
                if( oldX_ && (oldX_ == x) )
                    return;
                hessian_ = 0;
                gradient_ = 0;
                boundary_values_ *= 0;

                auto x_ = gradient_;
                copy(x, x_);
                assemble_derivative_impl(x_);

                oldX_ = x;
            }
            
            mutable FunGFunctional functional_;
            mutable Spacy::Vector boundary_values_;
            std::shared_ptr<VectorSpace> operator_space_;
            std::shared_ptr< dealii::FESystem<dim> > fe_system_;
            std::shared_ptr< dealii::DoFHandler<dim> > dof_handler_;
            SparsityPattern sparsity_pattern_;

            mutable double value_;
            mutable Gradient gradient_;
            mutable Hessian hessian_;

            mutable ::Spacy::Vector oldX_value_, oldX_;
        };
    }
    /** @} */
}
