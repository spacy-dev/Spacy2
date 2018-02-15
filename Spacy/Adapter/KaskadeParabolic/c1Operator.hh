#pragma once

#include <utility>
#include <vector>

#include <fem/assemble.hh>
#include <fem/istlinterface.hh>
#include <linalg/triplet.hh>

#include <Spacy/c1Operator.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Util/Mixins/numberOfThreads.hh>
#include <Spacy/Util/Mixins/Get.hh>

#include "gridManager.hh"
#include "directSolver.hh"
#include "linearBlockOperator.hh"
#include "linearBlockOperatorSpace.hh"
#include "util.hh"
#include<Spacy/zeroVectorCreator.hh>


namespace Spacy
{
  /** @addtogroup KaskadeParabolicGroup
     * @{
     */
  namespace KaskadeParabolic
  {
    /**
         * @brief %Operator interface for %Kaskade 7. Models a dynamical differentiable operator \f$A:X\rightarrow Y\f$ semi discretized in time.
         * @see Spacy::C1Operator
         */
    template <class OperatorDefinition>
    class C1Operator :
        public OperatorBase,
        public Mixin::NumberOfThreads
    {
      using VariableSetDescription = typename OperatorDefinition::OriginVars;
      using TestVariableSetDescription = typename OperatorDefinition::TestVars;
      using CoefficientVector = typename VariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using TCoeff = typename TestVariableSetDescription::template CoefficientVectorRepresentation<>::type;

      using Spaces = typename VariableSetDescription::Spaces;
      using Assembler = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt<OperatorDefinition> >;
      using Matrix = ::Kaskade::MatrixAsTriplet<double>;
      using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix,CoefficientVector,CoefficientVector>;
      using Linearization = PDE::LinearBlockOperator<VariableSetDescription,VariableSetDescription>;

      using ScalProdDefinition = PDE::ScalarProdFunctional<double,VariableSetDescription>;
      using AssemblerSP = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt<ScalProdDefinition> >;

      using VYSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<VariableSetDescription ,0>;
      using CoefficientVectorY = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
      using KaskadeOperatorYY = ::Kaskade::MatrixRepresentedOperator<Matrix,CoefficientVectorY,CoefficientVectorY>;

      using VectorImpl_domain = Vector<VariableSetDescription>;
      using VectorImpl_range = Vector<TestVariableSetDescription>;

      using VectorImpl_source = Vector<typename OperatorDefinition::SourceVars>;

    public:

      /**
       * @brief Construct dynamical operator for %Kaskade 7 semi discretized in time.
       * @param f operator definition from %Kaskade 7
       * @param gm Grid manager holding temporal and spatial grids
       * @param domain domain space
       * @param range range space
       *
       */

      C1Operator(std::function<OperatorDefinition(const typename OperatorDefinition::SourceVars::VariableSet)> & F,  GridManager<typename OperatorDefinition::OriginVars::Spaces>& gm,
                 const VectorSpace& domain,const VectorSpace& range, const Spacy::Vector& u)
        : OperatorBase(domain,range), F_(F), gm_(gm),y0_(VariableSetDescription::template CoefficientVectorRepresentation<>::init(*gm_.getSpacesVec().at(0))), u_(u)
      {
        this->resizeMembers();
      }

      /**
             * @brief Copy constructor.
             * @param B object to copy from
             */
      C1Operator(const C1Operator& B) = default;

      /**
             * @brief Copy assignment.
             * @param B object to copy from
             */
      C1Operator& operator=(const C1Operator& B) = default;

      /**
             * @brief Move constructor.
             * @param B object to move from
             */
      C1Operator(C1Operator&& B) = default;

      /**
             * @brief Move assignment.
             * @param B object to move from
             */
      C1Operator& operator=(C1Operator&& B) = default;



      /**
             * @brief Apply operator.
             * @param x argument
             * @return \f$A(x)\f$
             */

      ::Spacy::Vector operator()(const ::Spacy::Vector& x) const
      {
        assembleOperator(x);
        return rhs_;
      }


      /**
             * @brief Compute \f$A'(x)dx\f$.
             * @param x current iterate
             * @param dx correction
             * @return \f$A'(x)dx\f$
             */
      ::Spacy::Vector d1(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const
      {
        return linearization(x)(dx);
      }

      /// Access \f$A'(x)\f$ as linear operator \f$X\rightarrow Y\f$
      auto linearization(const ::Spacy::Vector& x) const
      {
        gradient_updated = false;
        assembleGradient(x);

        if(gradient_updated || !G_ptr)
          makeGradientLBOPointer();

        if(verbose)
          std::cout<<"returning from linearization"<<std::endl;
        return *G_ptr;
      }

      // make Norm Operator
      auto massMatrix() const
      {
        assembleMassMatrices();
        if(!M_ptr)
          makeMassLBOPointer();

        if(verbose)
          std::cout<<" returning Mass LBO "<<std::endl;
        return *M_ptr;
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

      /**
             * @brief Get the Grid manager
             * @return Grid Manager holding spatial and temporal grid information
             */
      const GridManager<Spaces>& getGridMan() const
      {
        return gm_;
      }

      /**
             * @brief Set Verbosity.
             * @param verb verbosity to be set
             */
      void setVerbosity(bool verb) const
      {
        verbose = verb;
      }

      /**
             * @brief Tell the operator about a refinement of the temporal grid (TO BE REPLACED WITH boost::signal impl)
             * @param k number of the interval that was refined
             */
      void informAboutRefinement(unsigned k )
      {
        MassY_.insert(MassY_.begin() + k,KaskadeOperator());
        A_.insert(A_.begin() + k,KaskadeOperator());

        auto u_impl = cast_ref<VectorImpl_source>(u_);
        auto vertices = gm_.getTempGrid().getVertexVec();
        FVec_.insert(FVec_.begin() + k,F_(u_impl.evaluate_u(vertices.at(k))));
        MassAssembled_.insert(MassAssembled_.begin() + k , false);

        GradRefined_ = false;
        OpRefined_ = false;
      }


      void setInitialCondition(auto y0_coeff)
      {
        boost::fusion::at_c<0>(y0_.data) = y0_coeff;
        OpRefined_ = false;

        return;
      }

      void setSource(const ::Spacy::Vector& u)
      {
        u_ = u;
        OpRefined_ = false;
        auto u_impl = cast_ref<VectorImpl_source>(u_);
        auto vertices = gm_.getTempGrid().getVertexVec();
        for(auto i = 0u;i<FVec_.size();i++)
        {
          FVec_.at(i) = (F_(u_impl.evaluate_u(vertices.at(i))));
        }

        return;
      }

    private:

      /// resize the members vectors a size equal to #timesteps
      void resizeMembers() const
      {
        rhs_ = zero(range());
        auto no_time_steps = gm_.getTempGrid().getDtVec().size();
        A_.resize(no_time_steps);
        MassY_.resize(no_time_steps);
        MassAssembled_.resize(no_time_steps,false);

        FVec_.reserve(no_time_steps);
        auto u_impl = cast_ref<VectorImpl_source>(u_);

        auto vertices = gm_.getTempGrid().getVertexVec();
        for(auto i = 0u;i<no_time_steps;i++)
        {
          FVec_.push_back(F_(u_impl.evaluate_u(vertices.at(i))));
        }
        return;
      }

      /// Assemble discrete representation of \f$A(x)\f$.
      void assembleOperator(const ::Spacy::Vector& x) const
      {
        if( old_X_A_ && (old_X_A_==x) && OpRefined_) return;

        if(verbose)
          std::cout<<"assembling Operator"<<std::endl;
        assembleMassMatrices();

        auto x_impl = cast_ref<VectorImpl_domain>(x);
        auto& rhs_impl = cast_ref<VectorImpl_range>(rhs_);
        auto dtVec = gm_.getTempGrid().getDtVec();
        auto spacesVec = gm_.getSpacesVec();

        // Loop over TimeSteps ~= Components of rhs_
        for(auto timeStep = 0u; timeStep < dtVec.size(); timeStep++) {

          //1) assemble elliptic part of PDE
          if(verbose)
            std::cout<<"assembling elliptic part for timestep "<<timeStep<<std::endl;

          Assembler assembler(*(spacesVec.at(timeStep)));

          assembler.assemble(::Kaskade::linearization(FVec_.at(timeStep), x_impl.get_nonconst(timeStep)), Assembler::RHS,
                             getNumberOfThreads());

          rhs_impl.getCoeffVec_nonconst(timeStep) = boost::fusion::at_c<0>(CoefficientVectorY(assembler.rhs()).data);
          // multiply by dt (Integration of constant function)
          rhs_impl.get_nonconst(timeStep)*= get(dtVec.at(timeStep));

          // Time Transfer Terms
          if(verbose)
            std::cout<<"assembling transfer part for timestep "<<timeStep<<std::endl;

          CoefficientVectorY y_curr_ (VYSetDescription::template CoefficientVectorRepresentation<>::init(*(spacesVec.at(timeStep))));
          boost::fusion::at_c<0>(y_curr_.data) = x_impl.getCoeffVec(timeStep);
          CoefficientVectorY y_curr_dual(
                VYSetDescription::template CoefficientVectorRepresentation<>::init(
                  *(spacesVec.at(timeStep))));
          MassY_.at(timeStep).apply(y_curr_,y_curr_dual);

          // initial Condition
          if(timeStep==0){
            //            typename VariableSetDescription::VariableSet x0(VariableSetDescription(*(spacesVec.at(0)),{"y"}));

            //            ::Kaskade::interpolateGloballyWeak<::Kaskade::PlainAverage>(boost::fusion::at_c<0>(x0.data),
            //                                                                        ::Kaskade::makeWeakFunctionView( [](auto const& cell, auto const& xLocal) -> Dune::FieldVector<double,1>
            //            {
            //              auto x = cell.geometry().global(xLocal);
            //              //return Dune::FieldVector<double,1>(1);
            //              //return Dune::FieldVector<double,1>(12*(1-x[1])*x[1]*(1-x[0])*x[0]);
            //              return Dune::FieldVector<double,1>(0);
            //            }));
            //            CoefficientVectorY y0(boost::fusion::at_c<0>(x0.data).coefficients());

            CoefficientVectorY y0_dual(
                  VYSetDescription::template CoefficientVectorRepresentation<>::init(
                    *(spacesVec.at(timeStep))));
            MassY_.at(timeStep).apply(y0_,y0_dual);

            assert(y_curr_.dim() == y_curr_dual.dim());
            y_curr_dual-=y0_dual;
          }
          else{
            CoefficientVectorY y_before_(
                  VYSetDescription::template CoefficientVectorRepresentation<>::init(
                    *(spacesVec.at(timeStep-1))));
            boost::fusion::at_c<0>(y_before_.data)  = x_impl.getCoeffVec(timeStep-1);

            CoefficientVectorY y_before_dual(
                  VYSetDescription::template CoefficientVectorRepresentation<>::init(
                    (*spacesVec.at(timeStep-1))));
            MassY_.at(timeStep-1).apply(y_before_,y_before_dual);

            assert(y_curr_dual.dim() == y_before_dual.dim());
            y_curr_dual -= y_before_dual;
          }

          rhs_impl.getCoeffVec_nonconst(timeStep) += boost::fusion::at_c<0>(y_curr_dual.data);
        }
        old_X_A_ = x;
        OpRefined_ = true;
      }

      /// Assemble discrete representation of \f$A'(x)\f$.
      void assembleGradient(const ::Spacy::Vector& x) const
      {
        if( old_X_dA_ && (old_X_dA_==x) && GradRefined_) return;


        if(verbose)
          std::cout<<"assembling Gradient"<<std::endl;

        assembleMassMatrices();

        gradient_updated = true;

        auto x_impl = cast_ref<VectorImpl_domain>(x);

        auto dtVec = gm_.getTempGrid().getDtVec();
        auto spacesVec = gm_.getSpacesVec();

        for(auto timeStep = 0u; timeStep < dtVec.size(); timeStep++)
        {
          if(verbose)
            std::cout<<"assembling Gradient "<<timeStep<<std::endl;
          Assembler assembler(*(spacesVec.at(timeStep)));
          assembler.assemble(::Kaskade::linearization(FVec_.at(timeStep),x_impl.get(timeStep)) , Assembler::MATRIX , getNumberOfThreads() );
          A_.at(timeStep) = KaskadeOperator(assembler.template get<Matrix>(onlyLowerTriangle_,0,1,0,1) );
        }

        old_X_dA_ = x;
        GradRefined_ = true;
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

          VariableSetDescription variableSet(*(spacesVec.at(timeStep)));
          typename VariableSetDescription::VariableSet x_(variableSet);
          AssemblerSP assemblersp(*(spacesVec.at(timeStep)));

          //assemblersp.assemble(::Kaskade::linearization(this->SP_,x_) , Assembler::MATRIX , getNumberOfThreads() );
          assemblersp.assemble(::Kaskade::linearization(SP_,x_) , Assembler::MATRIX , getNumberOfThreads() );

          MassY_.at(timeStep) =  KaskadeOperator( assemblersp.template get<Matrix>(onlyLowerTriangle_) );

          MassAssembled_.at(timeStep) = true;
        }
        if(verbose)
          std::cout<<" done assembling mass matrices "<<std::endl;
      }

      /// construct a shared ptr on the LinearBlockOperator holding the derivative
      void makeGradientLBOPointer() const
      {
        auto normfunc = [](const ::Spacy::Vector &v) {
          return Real{0};
        };

        if(verbose)
          std::cout<<" Constructing the LBO Pointer "<<std::endl;
        G_ptr = std::make_shared<PDE::LinearBlockOperator<VariableSetDescription, VariableSetDescription> >(
              PDE::LinearBlockOperator<VariableSetDescription, VariableSetDescription>(MassY_,A_, gm_.getTempGrid().getDtVec(), gm_.getSpacesVec(),
                                                                                       VectorSpace(
                                                                                         PDE::LinearBlockOperatorCreator<VariableSetDescription, VariableSetDescription>
                                                                                         (this->domain(),
                                                                                          this->range()),
                                                                                         normfunc, true),solverCreator_));
        if(verbose)
          std::cout<<"done with constructing the Gradient LBO pointer"<<std::endl;
        return;
      }


      /// construct a shared ptr on the LinearBlockOperator holding the Mass matrix
      void makeMassLBOPointer() const
      {
        auto normfunc = [](const ::Spacy::Vector &v) {
          return Real{0};
        };

        if(verbose)
          std::cout<<" Constructing the LBO Pointer "<<std::endl;
        M_ptr = std::make_shared<PDE::NormOperator<VariableSetDescription, VariableSetDescription> >(
              PDE::NormOperator<VariableSetDescription, VariableSetDescription>(MassY_, gm_.getTempGrid().getDtVec(), gm_.getSpacesVec(),
                                                                                VectorSpace(
                                                                                  PDE::NormOperatorCreator<VariableSetDescription, VariableSetDescription>
                                                                                  (this->domain(),
                                                                                   this->range()),
                                                                                  normfunc, true)));
        if(verbose)
          std::cout<<"done with constructing the Mass LBO pointer"<<std::endl;
        return;
      }

      GridManager<Spaces>& gm_;
      std::function<OperatorDefinition(const typename OperatorDefinition::SourceVars::VariableSet)> F_;
      ScalProdDefinition SP_ = ScalProdDefinition();
      mutable std::vector<OperatorDefinition> FVec_;

      CoefficientVector y0_;
      mutable ::Spacy::Vector u_;

      mutable bool verbose = false;
      mutable bool gradient_updated;

      mutable std::shared_ptr< PDE::LinearBlockOperator<VariableSetDescription ,VariableSetDescription> > G_ptr = nullptr;
      mutable std::shared_ptr< PDE::NormOperator<VariableSetDescription ,VariableSetDescription> > M_ptr = nullptr;

      mutable std::vector<KaskadeOperator> A_{};
      mutable std::vector<KaskadeOperator> MassY_{};
      mutable ::Spacy::Vector rhs_;
      mutable ::Spacy::Vector old_X_A_{}, old_X_dA_{};

      bool onlyLowerTriangle_ = false;

      mutable bool GradRefined_ = false;
      mutable bool OpRefined_ = false;
      mutable std::vector<bool> MassAssembled_;
      mutable unsigned refinedIndex_;

      std::function<LinearSolver(const Linearization&)> solverCreator_ = [](const Linearization& M)
      {
        return PDE::DirectSolver<OperatorDefinition>(M);
      };
    };

    /**
         * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow Y\f$ from %Kaskade 7.
         * @param F operator definition from %Kaskade 7
         * @param gm Grid manager holding temporal and spatial grids
         * @param domain domain space \f$X\f$
         * @param range range space \f$Y\f$
         * @return @ref C1Operator "::Spacy::KaskadeParabolic::C1Operator<OperatorDefinition>( F, gm, domain, range)"
         *
         */
    template <class OperatorDefinition>
    auto makeC1Operator(std::function<OperatorDefinition(const typename OperatorDefinition::SourceVars::VariableSet)> & F,  GridManager<typename OperatorDefinition::OriginVars::Spaces>& gm,
                        const VectorSpace& domain,const VectorSpace& range, const ::Spacy::Vector& u)
    {
      return C1Operator<OperatorDefinition>(F,gm,domain,range,u);
    }
  }
  /** @} */
}
