#pragma once

#include "c1Operator.hh"

namespace Spacy
{
    /** @addtogroup dealIIGroup @{ */
    namespace dealII
    {
        template <class FunGOperator, int dim, int n_components=1>
        class C1FunGOperator : public OperatorBase
        {
            using Value = std::conditional_t< n_components == 1, double, dealii::Vector<double> >;
            using Gradient = dealii::Tensor<n_components,dim>;

        public:
            C1FunGOperator(FunGOperator&& operator_impl, const VectorSpace& domain, const VectorSpace& range)
                : OperatorBase(domain,range),
                  A_(std::make_shared< dealii::BlockSparseMatrix<double> >()),
                  operatorSpace_( std::make_shared<VectorSpace>( LinearOperatorCreator(),
                  [](const Spacy::Vector& v)
                  {
                      return cast_ref< LinearOperator<dim> >(v).get().block(0,0).frobenius_norm();
                  } , true ) ),
                  operator_(std::move(operator_impl)),
                  boundary_values_(zero(domain))
            {
                const auto& dealIICreator = creator< VectorCreator<dim> >(domain);
                A_->reinit(dealIICreator.sparsityPattern());
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

                auto y = zero(range());
                A_->block(0,0).vmult(get(cast_ref<Vector>(y)),
                                     get(cast_ref<Vector>(dx)));

                return y;
            }

            /**
             * @brief Access \f$A'(x)\f$ as linear operator \f$X\rightarrow Y\f$
             * @see LinearOperator, ::Spacy::LinearOperator
             */
            auto linearization(const ::Spacy::Vector& x) const
            {
                assemble(x);
                return LinearOperator<dim>{ *A_ , *operatorSpace_ , boundary_values_, domain(), range() };
            }

        private:
            template <class T>
            void update_cell_matrix(T q_index, T dofs_per_cell,
                                    const dealii::FEValues<dim>& fe_values,
                                    dealii::FullMatrix<double>& cell_matrix) const
            {
                for(auto i=0u; i<dofs_per_cell; ++i)
                    for(auto j=0u; j<dofs_per_cell; ++j)
                        cell_matrix(i,j) += operator_.template d1<0>( std::make_tuple(fe_values.shape_value(j, q_index),
                                                                                      fe_values.shape_grad(j, q_index)) ) *
                                            fe_values.shape_grad(i, q_index) * fe_values.JxW(q_index);
            }

            template <class T>
            void update_cell_rhs(T q_index, T dofs_per_cell,
                                 const dealii::FEValues<dim>& fe_values,
                                 dealii::Vector<double>& cell_rhs) const
            {
                for(auto i=0u; i<dofs_per_cell; ++i)
                {
                    cell_rhs(i) += operator_() * fe_values.shape_grad(i, q_index) * fe_values.JxW(q_index);
                    cell_rhs(i) -= 1 * fe_values.shape_value(i, q_index) * fe_values.JxW (q_index) );
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

                std::vector<Value> old_solution_values(n_q_points);
                std::vector<Gradient> old_solution_gradients(n_q_points);
                std::vector<dealii::types::global_dof_index> local_dof_indices(dofs_per_cell);

                for(auto cell = dealIICreator.dofHandler().begin_active();
                    cell < dealIICreator.dofHandler().end();
                    ++cell)
                {
                    fe_values.reinit(cell);
                    const auto& x_ = get(cast_ref<Vector>(x));
                    fe_values.get_function_values(x_, old_solution_values);
                    fe_values.get_function_gradients(x_, old_solution_gradients);
                    cell_matrix = 0;
                    cell_rhs = 0;

                    for(auto q_index=0u; q_index<n_q_points; ++q_index)
                    {
                        operator_.template update<0>(std::make_tuple(old_solution_values[q_index],
                                                                     old_solution_gradients[q_index]));
                        update_cell_matrix(q_index, dofs_per_cell,
                                           fe_values,
                                           cell_matrix);
                        update_cell_rhs(q_index, dofs_per_cell,
                                        fe_values,
                                        cell_rhs);
                    }

                    cell->get_dof_indices(local_dof_indices);

                    for(auto i=0u; i<dofs_per_cell; ++i)
                        for(auto j=0u; j<dofs_per_cell; ++j)
                            A_->block(0,0).add(local_dof_indices[i], local_dof_indices[j],
                                               cell_matrix(i,j));

                    for (auto i=0u; i<dofs_per_cell; ++i)
                        b_(local_dof_indices[i]) += cell_rhs(i);
                }

                dealii::MatrixTools::apply_boundary_values(dealIICreator.boundaryValues(),
                                                           A_->block(0,0),
                                                           get(cast_ref<Vector>(boundary_values_)),
                                                           b_);


                oldX_ = x;
            }

            mutable std::shared_ptr< dealii::BlockSparseMatrix<double> > A_;
            mutable dealii::Vector<double>       b_;
            mutable ::Spacy::Vector oldX_;
            std::shared_ptr<VectorSpace> operatorSpace_;
            mutable FunGOperator operator_;
            mutable Spacy::Vector boundary_values_;
        };
    }
    /** @} */
}
