#include<Spacy/Algorithm/CompositeStep/affineCovariantSolver.hh>
#include<Spacy/Algorithm/Newton/newton.hh>

#include<Spacy/Adapter/kaskadeParabolic.hh>
#include<Spacy/Spaces/realSpace.hh>

#include <Spacy/inducedScalarProduct.hh>


namespace Spacy
{
  /** @addtogroup KaskadeParabolicGroup
     * @{
     */

  namespace KaskadeParabolic
  {

    // auxiliary function for debugging
    template<typename Matrix>
    void writeMatlab(const Matrix &matrix, std::string type) {
      std::ofstream myfile;
      myfile.open("matrix" + type + ".dat");
      for (int i = 0; i < matrix.ridx.size(); i++) {
        myfile << matrix.ridx[i] + 1;
        myfile << " ";
        myfile << matrix.cidx[i] + 1;
        myfile << " ";
        myfile << matrix.data[i];
        myfile << std::endl;
      }
      myfile.close();
    }

    /**
         * @brief Model Predictive controller for optimal control problems using composite step solver as inner solver
         * solves optimal control problem, implements part of the control and solves forward problem to simulate a real
         * world application
         */
    template<class NormalStepFunctionalDefinition,class TangentialStepFunctionalDefinition, class ForwardFunctionalDefinition>
    class ModelPredictiveController{
    public:

      /// %Kaskade::VariableSetDescription of y,u and p
      using Descriptions = typename NormalStepFunctionalDefinition::AnsatzVars;
      using Spaces = typename Descriptions::Spaces;

      // generator for TangentialStepFunctional for a reference state to be tracked
      using TFGenerator = std::function<TangentialStepFunctionalDefinition(const typename
      Descriptions::VariableSet)>;

      // generator for NormalStepFunctional for a reference state to be tracked
      using NFGenerator = std::function<NormalStepFunctionalDefinition(const typename
      Descriptions::VariableSet)>;


      /// %Kaskade::VariableSetDescription of control u
      using DescriptionsU = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,1>;

      // generator for forward functional for a given source
      using FFGenerator = std::function<ForwardFunctionalDefinition(const typename DescriptionsU::VariableSet)>;

      /// %Kaskade::VariableSetDescription of state y
      using DescriptionsY = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,0>;
      using CoefficientVectorY = typename DescriptionsY::template CoefficientVectorRepresentation<>::type;


    public:
      /// We do not allow default generation
      ModelPredictiveController() = delete;

      /**
             * @brief Construct a MPC- controller
             * @param normalFuncGenerator generates a (Normal)StepFunctional for a given reference y_ref in C2Functional
             * @param tangentialFuncGenerator generates a (Tangential)StepFunctional for a given reference y_ref in C2Functional
             * @param t_end Optimal Control time horizon
             * @param N Number of desired grid points for optimal control problem
             * @param MPC_Steps Number of MPC feedback loops to be performed
             * @param tau length of feedback (Resulting trajectory will be MPC_Steps*tau long)
             * @param N_tau_ number of grid points for simulation of real world
             * @param domain domain space
             * @param gridtype Kind of temporal grid type (uniform or exponential possible)
             * @param expfactor if exponential grid is chosen, the distribution of gridpoints can be steered with this parameter
             */
      ModelPredictiveController(NFGenerator& normalFuncGenerator, TFGenerator& tangentialFuncGenerator, FFGenerator& forwardFuncGenerator,
                                ::Spacy::Real t_end, unsigned N, unsigned MPC_Steps, ::Spacy::Real tau, unsigned N_tau_,std::string gridtype="uniform", ::Spacy::Real expfactor=::Spacy::Real{-0.345})
        : N_tau(N_tau_),no_mpc_steps(MPC_Steps),tau_(tau),gm_(GridManager<Spaces>(N,t_end,4,1, gridtype,expfactor)),gm_fine_(GridManager<Spaces>((no_mpc_steps*N_tau_)+1,no_mpc_steps*tau,4,1)),
          gm_forward_(GridManager<Spaces>(N_tau_+1,tau,4,1)), nfGen_(normalFuncGenerator), tfGen_(tangentialFuncGenerator), ffGen_(forwardFuncGenerator)
      {
        if(verbose_)
        {
          std::cout<<" OCP : "<<N<<" grid points on [0,"<<t_end<<"] "+gridtype<<std::endl;
          std::cout<<" MPC trajectory : "<<no_mpc_steps*N_tau+1<<" grid points on [0,"<<no_mpc_steps*tau<<"]"<<std::endl;
          std::cout<<"  Creating Domains "<<std::endl;
        }

        domain_ = makeHilbertSpace(gm_,{0u,1u},{2u});
        domain_fine_= makeHilbertSpace(gm_fine_,{0u,1u},{2u});

        if(verbose_)
          std::cout<<"  Creating Functionals "<<std::endl;
        nsf = makeC2Functional(nfGen_,gm_,domain_);
        tsf = makeC2Functional(tfGen_,gm_,domain_);

        if(verbose_)
          std::cout<< "Creating Stuff for forward solution"<<std::endl;
        domain_forward_ = Spacy::KaskadeParabolic::makeHilbertSpace(gm_forward_);
        auto z = zero(domain_fine_);
        auto z_ps = ::Spacy::cast_ref<::Spacy::ProductSpace::Vector> (z);
        A = makeC1Operator<ForwardFunctionalDefinition>(ffGen_, gm_forward_ , domain_forward_ , domain_forward_.dualSpace(),
                                                        ::Spacy::cast_ref<::Spacy::ProductSpace::Vector>(z_ps.component(PRIMAL)).component(1));

        ::Spacy::cast_ref<C1Operator<ForwardFunctionalDefinition> >(A).setVerbosity(false);

        domain_forward_.setScalarProduct( Spacy::InducedScalarProduct( ::Spacy::cast_ref<::Spacy::KaskadeParabolic::C1Operator<ForwardFunctionalDefinition> >(A).massMatrix() ));

        writeMatlab(::Spacy::cast_ref<C1Operator<ForwardFunctionalDefinition> >(A).linearization(zero(domain_forward_)).getKaskOp("A", 0).get(),"Mass"+std::to_string(0));
        writeMatlab(::Spacy::cast_ref<C1Operator<ForwardFunctionalDefinition> >(A).linearization(zero(domain_forward_)).getKaskOp("A", 2).get(),"Mass"+std::to_string(0));


        gridtype_ = gridtype;
        N_ = N;
      }


      /**
             * @brief Perform an MPC Feedback loop
             *
             * 1. Solve optimal control problem on [0,T] -> control u
             * 2. solve forward problem on [0,tau] with control u as source (Real world simulation) -> state y
             * 3. set initial value on y(tau) and go to 1, repeat MPC_Steps times
             */
      void MPCloop()
      {
        auto x= zero(domain_);
        auto vertices_finegrid = gm_fine_.getTempGrid().getVertexVec();
        trajectory = zero(domain_fine_);
        auto trajectory_impl_triplet = getImpl<Descriptions>(trajectory);

        ::Spacy::Real currenttime = 0.;
        auto currentindex = 0u;

        for(auto i = 0u; i < no_mpc_steps ; i++)
        {
          std::cout<<"    ##########################"<<std::endl;
          std::cout<<"    ####    MPC loop    "<<i<<" ####"<<std::endl;
          std::cout<<"    ##########################"<<std::endl;

          // ######## OPTIMAL CONTROL PROBLEM SOLUTION ########
          /// Construct and call composite step solver
          auto cs = Spacy::CompositeStep::AffineCovariantSolver(nsf,tsf,domain_);
          cs.setRelativeAccuracy(1e-6);
          cs.set_eps(1e-12);
          cs.setVerbosityLevel(2);
          cs.setMaxSteps(500);
          cs.setIterativeRefinements(0);
          cs.setDesiredContraction(0.5);
          cs.setRelaxedDesiredContraction(0.6);
          cs.setMaximalContraction(0.75);

          auto result = cs(x);

          // save open loop solution of ocp
          OCP::printNormSolution<Descriptions>(result,::Spacy::cast_ref<C2Functional<NormalStepFunctionalDefinition> >(nsf).hessian(result),gm_,"open_loop_" + gridtype_ +"_"+ std::to_string(N_)+"_"+std::to_string(i));


          // ######## SIMULATION OF MODEL (FORWARD PROBLEM)########
          auto result_ps = ::Spacy::cast_ref<::Spacy::ProductSpace::Vector>(result);
          auto y = solveForwardProblem((::Spacy::cast_ref<::Spacy::ProductSpace::Vector>(result_ps.component(PRIMAL))).component(1));


          // ######## SAVE TRAJECTORY ########
          if(verbose_)
            std::cout<<"Saving in Trajectory"<<std::endl;

          // y from forward solution (fine resolution)
          // u from OCP (piecewise constant, OCP resolution)
          // p from OCP (linear interpolated, OCP resolution, not really needed)

          // for u,p
          auto result_impl = getImpl<Descriptions>(result);
          // for y

          Vector<DescriptionsY> y_impl =  ::Spacy::cast_ref<Vector<DescriptionsY> >(y);
          if(verbose_)
            std::cout<<"Currenttime is "<<currenttime<<std::endl;

          // first initial value needs to be saved, all resulting initial values are given by last value of preceding loop,
          // hence no need to save them again
          if(i == 0)
          {
            if(verbose_)
              std::cout<<"Indizes in fine grid from "<<currentindex<<" to "<<currentindex + N_tau<<" of "<<gm_fine_.getTempGrid().getDtVec().size()<<std::endl;
            for(auto k = 0; k < N_tau+1 ;k++)
            {
              std::get<0>(trajectory_impl_triplet).getCoeffVec_nonconst(currentindex+k) = y_impl.getCoeffVec(k);
              std::get<1>(trajectory_impl_triplet).getCoeffVec_nonconst(currentindex+k) = boost::fusion::at_c<0>(std::get<1>(result_impl).evaluate_u(vertices_finegrid.at(currentindex+k)-currenttime).data).coefficients();
              std::get<2>(trajectory_impl_triplet).getCoeffVec_nonconst(currentindex+k) = boost::fusion::at_c<0>(std::get<2>(result_impl).evaluate_linearInterpolated(vertices_finegrid.at(currentindex+k)-currenttime).data).coefficients();
            }
            currentindex += N_tau+1;

          }
          else
          {
            if(verbose_)
              std::cout<<"Indizes in fine grid from "<<currentindex<<" to "<<currentindex + N_tau<<" of "<<gm_fine_.getTempGrid().getDtVec().size()<<std::endl;
            for(auto k = 0; k < N_tau ;k++)
            {
              std::get<0>(trajectory_impl_triplet).getCoeffVec_nonconst(currentindex+k) = y_impl.getCoeffVec(k);
              std::get<1>(trajectory_impl_triplet).getCoeffVec_nonconst(currentindex+k) = boost::fusion::at_c<0>(std::get<1>(result_impl).evaluate_u(vertices_finegrid.at(currentindex+k)-currenttime).data).coefficients();
              std::get<2>(trajectory_impl_triplet).getCoeffVec_nonconst(currentindex+k) = boost::fusion::at_c<0>(std::get<2>(result_impl).evaluate_linearInterpolated(vertices_finegrid.at(currentindex+k)-currenttime).data).coefficients();
            }
            currentindex += N_tau;

          }

          // Proceed in time
          currenttime += tau_;
        }

        /// Compute Cost Funcitonal Value of the "Real World" Trajectory
        auto objectivefunctional = makeC2Functional(tfGen_,gm_fine_,domain_fine_);
        if(verbose_)
          std::cout<<"VALUE of Objective Function "<<objectivefunctional(trajectory)<<std::endl;

        /// Save it in a file
        std::ofstream obj;
        obj.open("data/Objective_Function.txt", std::ofstream::out | std::ofstream::app);
        obj << N_ <<std::setw(15)<< " "+gridtype_ + " "<<objectivefunctional(trajectory)<<std::endl;
        obj.close();



        /// Save grids in file
        std::ofstream grids;
        grids.open("data/grids.txt", std::ofstream::out | std::ofstream::app);
        auto vertices = gm_.getTempGrid().getVertexVec();
        grids <<std::setw(15)<< N_ << " "+gridtype_ + " ";

        for(auto& vert: vertices)
        {
          grids << get(vert) << " ";
        }
        grids<<std::endl;
        grids.close();

        /// Print spatial norm over time of trajectory over time into file
        auto normfunctional = makeC2Functional(nfGen_,gm_fine_,domain_fine_);
        OCP::printNormSolution<Descriptions>(trajectory,normfunctional.hessian(trajectory),gm_fine_,"mpc_trajec_" + gridtype_ +"_"+ std::to_string(N_));
        /// Plot trajectory as VTK File
        OCP::writeVTK<Descriptions>(trajectory,"solution");

        /// Visualize turnpike of Optimal Control problem (Just last value of trajectory, hoping it has approached the TP)
        auto turnpike = zero(domain_fine_);
        auto turnpike_impl_triple = getImpl<Descriptions>(turnpike);
        for(auto k = 0; k < gm_fine_.getTempGrid().getDtVec().size() ;k++)
        {
          std::get<0>(turnpike_impl_triple).getCoeffVec_nonconst(k) = std::get<0>(trajectory_impl_triplet).getCoeffVec(gm_fine_.getTempGrid().getDtVec().size()-1);
          std::get<1>(turnpike_impl_triple).getCoeffVec_nonconst(k) = std::get<1>(trajectory_impl_triplet).getCoeffVec(gm_fine_.getTempGrid().getDtVec().size()-1);
          std::get<2>(turnpike_impl_triple).getCoeffVec_nonconst(k) = std::get<2>(trajectory_impl_triplet).getCoeffVec(gm_fine_.getTempGrid().getDtVec().size()-1);
        }

        /// Print the cost of being at turnpike at all time
        if(verbose_)
          std::cout<<"VALUE of constant Turnpike Function "<<objectivefunctional(turnpike)<<std::endl;
        /// Print spatial norm of turnpike over time on file
        OCP::printNormSolution<Descriptions>(turnpike,normfunctional.hessian(trajectory),gm_fine_,"turnpike" + gridtype_ +"_"+ std::to_string(N_));

      }

      /**
             * @brief Solve Forward problem for given source/control
             * @param u source/control
             */
      ::Spacy::Vector solveForwardProblem(const ::Spacy::Vector& u)
      {
        if(verbose_)
          std::cout<<" Solving forward problem "<<std::endl;

        /// Set accuracy of newton solver
        auto p = Spacy::Newton::Parameter{};
        p.setRelativeAccuracy(1e-12);

        // set source term
        ::Spacy::cast_ref<C1Operator<ForwardFunctionalDefinition> >(A).setSource(u);

        /// Call newton solver
        if(verbose_)
          std::cout<<"Starting Newton "<<std::endl;
        auto y = covariantNewton(A,p);

        //set initial value for next forward solution(A) and for OCP (tsf)
        // initial data only enters d1, hence not needed to set for normalstepfunctional as d1 is not called
        auto y_impl = ::Spacy::cast_ref<Vector<DescriptionsY> >(y);
        if(::Spacy::is<C2Functional<TangentialStepFunctionalDefinition> >(tsf))
        {
          auto& tsf_impl = ::Spacy::cast_ref<C2Functional<TangentialStepFunctionalDefinition> >(tsf);
          auto& A_impl = ::Spacy::cast_ref<C1Operator<ForwardFunctionalDefinition> >(A);

          tsf_impl.setInitialCondition(boost::fusion::at_c<0>(y_impl.get(gm_forward_.getTempGrid().getDtVec().size()-1).data).coefficients());
          A_impl.setInitialCondition(boost::fusion::at_c<0>(y_impl.get(gm_forward_.getTempGrid().getDtVec().size()-1).data).coefficients());
        }
        return y;
      }

    private:
      bool verbose_ = false;

      /// OPTIMAL CONTROL PROBLEMS PARAMETERS
      GridManager<Spaces> gm_;
      TFGenerator tfGen_;
      NFGenerator nfGen_;
      FFGenerator ffGen_;
      ::Spacy::VectorSpace domain_;
      mutable ::Spacy::C2Functional tsf, nsf;

      /// Implementation horizon
      ::Spacy::Real tau_;
      unsigned no_mpc_steps;

      //for saving the trajectory in a fine resolution
      GridManager<Spaces> gm_fine_;
      ::Spacy::VectorSpace domain_fine_;
      ::Spacy::Vector trajectory;

      // for forward solution
      GridManager<Spaces> gm_forward_;
      ::Spacy::VectorSpace domain_forward_;
      ::Spacy::C1Operator A;
      unsigned N_tau;

      //for output purposes, Gridpoints of OCP and gridtype of OCP timegrid
      unsigned N_;
      std::string gridtype_;
    };


  }
}
