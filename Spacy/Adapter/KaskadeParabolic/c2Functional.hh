#pragma once

#include <memory>

#include "fem/assemble.hh"
#include "fem/istlinterface.hh"
#include "linalg/triplet.hh"

#include <Spacy/c1Operator.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/Mixins/Eps.hh>
#include <Spacy/Util/Mixins/numberOfThreads.hh>
#include <Spacy/Util/Base/FunctionalBase.hh>

#include "directSolver.hh"
#include "vectorSpace.hh"
#include "vector.hh"
#include "linearBlockOperatorSpace.hh"
#include "linearBlockOperator.hh"

namespace Spacy
{
    /** @addtogroup KaskadeGroup
     * @{
     */
    namespace KaskadeParabolic
    {
        /**
         * @brief %Functional interface for %Kaskade 7. Models a twice differentiable functional \f$f:X\rightarrow \mathbb{R}\f$.
         * @tparam FunctionalDefinition functional definition from %Kaskade 7
         * @see ::Spacy::C2Functional
         */
        template <class FunctionalDefinition>
        class C2Functional :
                public FunctionalBase,
                public Mixin::Eps,
                public Mixin::NumberOfThreads
        {
        public:
            /// %Kaskade::VariableSetDescription
            using VariableSetDescription = typename FunctionalDefinition::AnsatzVars;
            /// Coefficient vector type.
            using CoefficientVector = typename VariableSetDescription::template CoefficientVectorRepresentation<>::type;
            /// boost::fusion::vector<const Space0*,const Space1*,...>
            using Spaces = typename VariableSetDescription::Spaces;
            /// %Kaskade::VariationalFunctionalAssembler
            using Assembler = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt<FunctionalDefinition> >;
            /// Matrix type
            using Matrix = ::Kaskade::MatrixAsTriplet<double>;
            /// operator for the description of the second derivative
            using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix,CoefficientVector,CoefficientVector>;

            using Linearization = LinearOperator<VariableSetDescription,VariableSetDescription>;

            /**
             * @brief Construct a twice differentiable functional \f$f: X\rightarrow \mathbb{R}\f$ from %Kaskade 7.
             * @param f operator definition from %Kaskade 7
             * @param domain domain space
             * @param rbegin first row to be considered in the definition of f
             * @param rend one after the last row to be considered in the definition of f
             * @param cbegin first column to be considered in the definition of f
             * @param cend one after the last column to be considered in the definition of f
             *
             * The optional parameters rbegin, rend, cbegin and cend can be used to define operators that correspond to parts of
             * a system of equation.
             */
            C2Functional(const FunctionalDefinition& f, GridManager<typename OperatorDefinition::OriginVars::Spaces>& gm, const VectorSpace& domain, const VectorSpace& range)
                : FunctionalBase(domain),
                  f_(f),
                  gm_(gm)
            {
              this->resizeMembers();
            }

            /**
                   * @brief Copy constructor.
                   * @param B object to copy from
                   */
            C2Functional(const C2Functional& B) = default;

            /**
                   * @brief Copy assignment.
                   * @param B object to copy from
                   */
            C2Functional& operator=(const C2Functional& B) = default;

            /**
                   * @brief Move constructor.
                   * @param B object to move from
                   */
            C2Functional(C2Functional&& B) = default;

            /**
                   * @brief Move assignment.
                   * @param B object to move from
                   */
            C2Functional& operator=(C2Functional&& B) = default;


            /**
             * @brief Apply functional.
             * @param x argument
             * @return \f$f(x)\f$
             */
            Real operator()(const ::Spacy::Vector& x) const
            {
                assembleFunctional(x);

                return value_;
            }

            /**
             * @brief Compute first directional derivative \f$f'(x) \in X^* \f$.
             *
             * @param x current iterate
             * @return \f$f'(x)\f$
             */
            ::Spacy::Vector d1(const ::Spacy::Vector& x) const
            {
                assembleGradient(x);

                return rhs_;
            }

            /**
             * @brief Compute second directional derivative \f$f''(x)dx\in X^* \f$.
             *
             * @param x current iterate
             * @param dx perturbation
             * @return \f$f''(x)dx\f$
             */
            ::Spacy::Vector d2(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const
            {
              return hessian(x)(dx);
            }

            /**
             * @brief Access \f$f''(x)\f$ as linear operator \f$X\rightarrow X^*\f$.
             * @param x point of linearization
             * @see LinearOperator, ::Spacy::LinearOperator
             */
            auto hessian(const ::Spacy::Vector& x) const
            {
                hessian_updated = false;
                assembleHessian(x);

                if(hessian_updated || !H_ptr)
                  makeHessianLBOPointer();

                std::cout<<"returning from hessian"<<std::endl;
                return *H_ptr;
            }


            /**
             * @brief Access onlyLowerTriangle flag.
             * @return true if only the lower triangle of a symmetric matrix is stored in the operator definition, else false
             */
            bool onlyLowerTriangle() const noexcept
            {
                return onlyLowerTriangle_;
            }

            /**
             * @brief Change solver creator.
             * @param f function/functor for the creation of a linear solver
             */
            void setSolverCreator(std::function<LinearSolver(const Linearization&)> f)
            {
                solverCreator_ = std::move(f);
            }

            const GridManager<Spaces>& getGridMan() const
            {
              return gm_;
            }

            void setVerbosity(bool verb) const
            {
              verbose = verb;
            }

            void informAboutRefinement(unsigned k )
            {
              MassY_.insert(MassY_.begin() + k,KaskadeOperator());
              A_.insert(A_.begin() + k,KaskadeOperator());
              FVec_.insert(FVec_.begin() + k,F_);
              MassAssembled_.insert(MassAssembled_.begin() + k , false);

              GradRefined_ = false;
              OpRefined_ = false;
            }

        private:

            void resizeMembers() const
            {
              rhs_ = zero(range());
              auto no_time_steps = gm_.getTempGrid().getDtVec().size();
              rhs_i_.resize(no_time_steps);
              value_i_.resize(no_time_steps);
              gradient_updated.resize(no_time_steps);
              A_.resize(no_time_steps);
              B_.resize(no_time_steps);
              A_t_.resize(no_time_steps);
              B_t_.resize(no_time_steps);
              My_.resize(no_time_steps);
              Mu_.resize(no_time_steps);
              Mass_diag_.resize(no_time_steps);
              Mass_sd_.resize(no_time_steps);
              MassAssembled_.resize(no_time_steps,false);
              FVec_.resize(no_time_steps,F_);
              old_y_f_i_resize(no_time_steps);
              old_u_f_i_.resize(no_time_steps);
              old_p_f_i_resize(no_time_steps);
              old_y_ddf_i_resize(no_time_steps);
              old_u_ddf_i_resize(no_time_steps);
              old_p_ddf_i_resize(no_time_steps);
              return;
            }
            /// Assemble \f$f(x)\f$.
            void assembleFunctional(const ::Spacy::Vector& x) const
            {
              auto dtVec = gm_.getTempGrid().getDtVec();
              auto spacesVec = gm_.getSpacesVec();
              auto x_ps = ::Spacy::cast_ref<PSV>(x);

              auto x_y = (::Spacy::cast_ref<PSV>(x_ps(PRIMAL)))(0);
              auto x_u = (::Spacy::cast_ref<PSV>(x_ps(PRIMAL)))(1);
              auto x_p = x_ps(DUAL);
              value_updated = false;

              for(auto timeStep = 0 ; timeStep < dtVec.size() ; timeStep++)
              {
                if( old_y_f_i_.at(timeStep) && old_y_f_i_.at(timeStep) == y &&
                    old_u_f_i_.at(timeStep) && old_u_f_i_.at(timeStep) == u &&
                    old_p_f_i_.at(timeStep) && old_p_f_i_.at(timeStep) == p )
                  continue;

                space = *spacesVec.at(timeStep);
                value_updated = true;

                // get VariableSetDescription of this Timestep
                VariableSetDescription variableSet(space);
                // Kaskade VariableSet for (y,u,p);
                typename VariableSetDescription::VariableSet x(variableSet);

                //Implementation on as Spacy::KaskadeParabolic::Vector
                auto x_y_impl = cast_ref<VectorImplY>(x_y);
                auto x_u_impl = cast_ref<VectorImplU>(x_u);
                auto x_p_impl = cast_ref<VectorImplP>(x_p);

                boost::fusion::at_c<0>(x.data) = x_y_impl.getCoeffVec(timeStep);
                boost::fusion::at_c<1>(x.data) = x_u_impl.getCoeffVec(timeStep);
                boost::fusion::at_c<2>(x.data) = x_p_impl.getCoeffVec(timeStep);

                Assembler assembler(space);
                assembler.assemble(::Kaskade::linearization(fVec_.at(timeStep),x) , Assembler::VALUE , getNumberOfThreads() );
                value_i_.at(timeStep) = assembler.functional();

                old_y_f_i_.at(timeStep) = x_y;
                old_u_f_i_.at(timeStep) = x_u;
                old_p_f_i_.at(timeStep) = x_p;
              }

              if(value_updated)
              {
                value_ = 0.;
                for (auto i = 1u; i < dtVec_.size(); i++)
                  value_ += dtVec.at(i) * value_i_.at(i);
              }

              if(verbose)
                std::cout<< "assembled the functional value " << value_ <<std::endl;
            }

            /// Assemble discrete representation of \f$f'(x)\f$.
            void assembleGradient(const ::Spacy::Vector& x) const
            {
                using boost::fusion::at_c;
                if( old_X_df_ && old_X_df_ == x ) return;


                old_X_df_ = x;
            }

            /// Assemble discrete representation of \f$f''(x)\f$.
            void assembleHessian(const ::Spacy::Vector& x) const
            {
              auto dtVec = gm_.getTempGrid().getDtVec();
              auto spacesVec = gm_.getSpacesVec();
              auto x_ps = ::Spacy::cast_ref<PSV>(x);

              auto x_y = (::Spacy::cast_ref<PSV>(x_ps(PRIMAL)))(0);
              auto x_u = (::Spacy::cast_ref<PSV>(x_ps(PRIMAL)))(1);
              auto x_p = x_ps(DUAL);

              for(auto timeStep = 0u; timeStep < dtVec.size() ; timeStep++)
              {
                if( old_y_ddf_i_.at(timeStep) && old_y_ddf_i_.at(timeStep) == y &&
                    old_u_ddf_i_.at(timeStep) && old_u_ddf_i_.at(timeStep) == u &&
                    old_p_ddf_i_.at(timeStep) && old_p_ddf_i_.at(timeStep) == p )
                {
                  continue;
                }
                auto spaces = *(spacesVec.at(timeStep));

                hessian_updated = true;

                // Kaskade VariableSet for (y,u,p);
                VariableSetDescription variableSet(spaces);
                typename VariableSetDescription::VariableSet x(variableSet);

                boost::fusion::at_c<0>(x.data) = x_y_impl.getCoeffVec(timeStep);
                boost::fusion::at_c<1>(x.data) = x_u_impl.getCoeffVec(timeStep);
                boost::fusion::at_c<2>(x.data) = x_p_impl.getCoeffVec(timeStep);

                Assembler assembler(spaces);

                assembler.assemble(::Kaskade::linearization(fVec_.at(timeStep),x) , Assembler::MATRIX , getNumberOfThreads() );

                My_.at(timeStep) = Mytype( assembler.template get<Matrix>(onlyLowerTriangle_,0,1,0,1) );
                A_t_.at(timeStep) = ATtype( assembler.template get<Matrix>(onlyLowerTriangle_,0,1,2,3) );
                Mu_.at(timeStep) = Mutype( assembler.template get<Matrix>(onlyLowerTriangle_,1,2,1,2) );
                B_t_.at(timeStep) = BTtype( assembler.template get<Matrix>(onlyLowerTriangle_,1,2,2,3) );
                A_.at(timeStep) = Atype( assembler.template get<Matrix>(onlyLowerTriangle_,2,3,0,1) );
                B_.at(timeStep) = Btype( assembler.template get<Matrix>(onlyLowerTriangle_,2,3,1,2) );

                My_.at(timeStep).get_non_const().setStartToZero();
                Mu_.at(timeStep).get_non_const().setStartToZero();
                A_.at(timeStep).get_non_const().setStartToZero();
                B_.at(timeStep).get_non_const().setStartToZero();
                A_t_.at(timeStep).get_non_const().setStartToZero();
                B_t_.at(timeStep).get_non_const().setStartToZero();

                old_y_ddf_i_.at(timeStep) = x_y;
                old_u_ddf_i_.at(timeStep) = x_u;
                old_p_ddf_i_.at(timeStep) = x_p;
              }
            }

            /// Assemble the mass matrices for the transfer terms
            void assembleMassMatrices() const
            {
              if(verbose)
                std::cout<<" assembling mass matrices "<<std::endl;

              auto dtVec = gm_.getTempGrid().getDtVec();
              auto spacesVec = gm_.getSpacesVec();

              for(auto timeStep = 0u; timeStep < dtVec.size() ; timeStep++)
              {
                if(MassAssembled_.at(timeStep)){ continue;}

                if(verbose)
                  std::cout<<"assembling mass matrices for timestep "<<timeStep<<std::endl;

                space = *(spacesVec.at(timeStep));

                VariableSetDescription variableSet(space);
                typename VariableSetDescription::VariableSet x(variableSet);

                AssemblerSP assemblersp(space);
                assemblersp.assemble(::Kaskade::linearization(scalprod_,x) , Assembler::MATRIX , getNumberOfThreads() );

                Mass_diag_.at(timeStep) = Masstype( assemblersp.template get<Matrix>(onlyLowerTriangle_,2,3,0,1) );
                Mass_diag_.at(timeStep).get_non_const().setStartToZero();

                if(timeStep != dtVec_.size()-1){
                  Mass_sd_.at(timeStep) = Masstype( assemblersp.template get<Matrix>(onlyLowerTriangle_,2,3,0,1) );
                  Mass_sd_.at(timeStep).get_non_const().setStartToZero();
                }

                MassAssembled_.at(timeStep) = true;
              }
              if(verbose)
                std::cout<<" done assembling mass matrices "<<std::endl;
            }

            void makeHessianLBOPointer() const
            {
              auto normfunc = [](const ::Spacy::Vector &v) {
                return Real{0};
              };

              if(verbose)
                std::cout<<" Constructing the LBO Pointer "<<std::endl;
              H_ptr = std::make_shared<OCP::LinearBlockOperator<VariableSetDescription, VariableSetDescription> >(
                    OCP::LinearBlockOperator<VariableSetDescription, VariableSetDescription>(My_, Mu_, A_, B_, A_t_,
                                                                                             B_t_, Mass_diag_, Mass_sd_, gm_.getTempGrid().getDtVec(), gm_.getSpacesVec(),
                                                                                             VectorSpace(
                                                                                               PDE::LinearBlockOperatorCreator<VariableSetDescription, VariableSetDescription>
                                                                                               (this->domain(),
                                                                                                this->range()),
                                                                                               normfunc, true),solverCreator_));
              std::cout<<"done with constructing the LBO pointer"<<std::endl;
              return;
            }

            //Problem definition
            GridManager<Spaces>& gm_;
            FunctionalDefinition f_;
            ScalProdDefinition SP_ = ScalProdDefinition();
            mutable std::vector<FunctionalDefinition> fVec_;

            mutable bool verbose = true;
            mutable bool hessian_updated;
            mutable std::vector<bool> gradient_updated {};
            mutable bool value_updated;

            // For functional value data
            mutable double value_ = 0;
            mutable std::vector<Real> value_i_;

            // For gradient data
            mutable ::Spacy::Vector rhs_{};
            mutable std::vector<::Spacy::Vector>rhs_i_{};

            //For hessian data
            mutable std::vector<Mytype> My_{};
            mutable std::vector<Mutype> Mu_{};
            mutable std::vector<Atype> A_{};
            mutable std::vector<Btype> B_{};
            mutable std::vector<ATtype> A_t_{};
            mutable std::vector<BTtype> B_t_{};
            mutable std::vector<Masstype> Mass_diag_{};
            mutable std::vector<Masstype> Mass_sd_{};
            //The hessian as forward operator built from the above vectors of blocks
            mutable std::shared_ptr< OCP::LinearBlockOperator<VariableSetDescription ,VariableSetDescription> > H_ptr = nullptr;



            mutable std::vector<::Spacy::Vector>
            old_y_f_i_{}, old_u_f_i_{}, old_p_f_i_{},
            old_y_ddf_i_{}, old_u_ddf_i_{}, old_p_ddf_i_{};

            mutable bool GradRefined_ = false;
            mutable bool OpRefined_ = false;
            mutable std::vector<bool> MassAssembled_;
            mutable unsigned refinedIndex_;

            bool onlyLowerTriangle_ = false;

            std::function<LinearSolver(const Linearization&)> solverCreator_ = [](const Linearization& f)
            {
                return makeDirectSolver<VariableSetDescription,VariableSetDescription>( f.A() ,
                                                                                        f.range() ,
                                                                                        f.domain() /*,
                                                                                                                                          MatrixProperties::GENERAL */);

            };
//            std::shared_ptr<VectorSpace> operatorSpace_ = nullptr;
        };

        /**
         * @brief Convenient generation of a twice differentiable functional \f$f: X\rightarrow \mathbb{R}\f$ for %Kaskade 7.
         * @param f operator definition from %Kaskade 7
         * @param domain domain space
         * @param rbegin first row to be considered in the definition of f
         * @param rend one after the last row to be considered in the definition of f
         * @param cbegin first column to be considered in the definition of f
         * @param cend one after the last column to be considered in the definition of f
         * @return @ref C2Functional "::Spacy::Kaskade::C2Functional<FunctionalDefinition>( f, domain, rbegin, rend, cbegin, cend )"
         *
         * The optional parameters rbegin, rend, cbegin and cend can be used to define operators that correspond to parts of
         * a system of equation.
         */
        template <class FunctionalDefinition>
        auto makeC2Functional(FunctionalDefinition& f,  GridManager<typename OperatorDefinition::OriginVars::Spaces>& gm, const VectorSpace& domain,const VectorSpace& range)
        {
          return C2Functional<FunctionalDefinition>(F,gm,domain,range);
        }
    }
    /** @} */
}
