#define FUSION_MAX_VECTOR_SIZE 15

#include <chrono>
#include <functional>
#include <iostream>

#include <dune/grid/config.h>
#include <dune/grid/uggrid.hh>

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Adapter/kaskadeParabolic.hh>
#include <Spacy/Algorithm/CompositeStep/affineCovariantSolver.hh>

#include <fem/gridmanager.hh>
#include <fem/lagrangespace.hh>
#include <fem/forEach.hh>
#include <io/vtk.hh>
#include <utilities/kaskopt.hh>
#include <utilities/gridGeneration.hh>

#define NCOMPONENTS 1
#include "Spacy/Adapter/KaskadeParabolic/Constraints/lqOcpHeader.hh"



int main(int argc, char *argv[])
{
  using namespace Kaskade;
  static constexpr int stateId = 0;
  static constexpr int controlId = 1;
  static constexpr int adjointId = 2;

  constexpr int dim = 2;
  int silence = 0;
  std::unique_ptr<boost::property_tree::ptree> pt = getKaskadeOptions(argc, argv, silence, false);

  double alpha = 1e-2;

  //  // Setting 1
  double d = 0.01;
  double mu = 0.;
  ::Spacy::Real t_end = 3;
  ::Spacy::Real tau = 0.5;
  unsigned mpcsteps = 4;
  auto expparam = -1.;
  ::std::vector<unsigned> NVec = {3,6,11,21,41};

  //Setting 2
  //  ::Spacy::Real tau = 2;
  //  unsigned mpcsteps = 4;
  //  ::Spacy::Real t_end = 8;
  //  auto expparam = -0.5;
  //   ::std::vector<unsigned> NVec = {3,6,11,21,41};

  //Setting 3
//  ::Spacy::Real tau = 1;
//  unsigned mpcsteps = 8;
//  auto expparam = -1.5;
//  double alpha = getParameter(pt, "alpha", 0.01);
//  double d = getParameter(pt, "dPara", 1e-1);
//  double mu = getParameter(pt, "muPara", -4.);
//  double t_end = getParameter(pt, "t_end", 4);
//  ::std::vector<unsigned> NVec = {8,21,41,61};

  std::cout<< "alpha "<< alpha<<" dPara "<<d<<" muPara "<<mu<<std::endl;

  using Grid = Dune::UGGrid<dim>;
  using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,Grid::LeafGridView> >;
  using Spaces = boost::fusion::vector<H1Space const*>;
  using VY = VariableDescription<0,1,stateId>;
  using VU = VariableDescription<0,1,controlId>;
  using VP = VariableDescription<0,1,adjointId>;
  using VariableDescriptions = boost::fusion::vector< VY , VU , VP > ;
  using  Descriptions = VariableSetDescription<Spaces,VariableDescriptions>;

  // ############### Normal Functional Generator (For optimal control problem in MPC-feedback-loop) ##############
  using NormalFunctionalDefinition =  NormalStepFunctional<stateId,controlId,adjointId,double,Descriptions>;
  std::function<NormalFunctionalDefinition(const typename Descriptions::VariableSet)> normalFuncGenerator = [&d,&mu,&alpha](const typename Descriptions::VariableSet y_ref) {
    return NormalFunctionalDefinition(alpha,y_ref,d,mu);
  };

  // ############### Tangential Functional Generator (For optimal control problem in MPC-feedback-loop) ###############
  using TangentialFunctionalDefinition =  TangentialStepFunctional<stateId,controlId,adjointId,double,Descriptions>;
  std::function<TangentialFunctionalDefinition(const typename Descriptions::VariableSet)> tangentialFuncGenerator = [&d,&mu,&alpha](const typename Descriptions::VariableSet y_ref) {
    return TangentialFunctionalDefinition(alpha,y_ref,d,mu);
  };

  // ############### Forward Functional Generator (For forward solution/simulation of real world in MPC-feedback-loop) ###############
  using HeatFunctionalDefinition = LinearModelPDE<double,::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,0> , ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,1>  >;
  std::function<HeatFunctionalDefinition(const typename ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,1>::VariableSet )> forwardFunctionalGenerator
      = [&d,&mu](const typename ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,1>::VariableSet control){
    return HeatFunctionalDefinition(d,mu,control);
  };



  //  for(auto N : NVec)
  //  {
  //    auto tg = ::Spacy::KaskadeParabolic::TempGrid(0,t_end,N,expparam);
  //    tg.print();
  //  }

  /// SAVE OBJECTIVE FUNCTION VALUE
  std::ofstream obj;
  obj.open("data/Objective_Function.txt", std::ofstream::out | std::ofstream::app);
  obj << "alpha: " <<alpha << ", d: " <<d<<", mu: "<<mu<<", t_end: "<<t_end <<", Mpc Steps: "<<mpcsteps<<", tau: "<<tau<<std::endl;
  obj.close();

  for(auto N : NVec)
  {

    // Solve with an uniform grid
    std::cout<<"##########################"<<std::endl;
    std::cout<<"####    UNIFORM   "<<N<<" ####"<<std::endl;
    std::cout<<"##########################"<<std::endl;
    ::Spacy::KaskadeParabolic::ModelPredictiveController<NormalFunctionalDefinition,TangentialFunctionalDefinition,HeatFunctionalDefinition>
        mpc_uni(normalFuncGenerator,tangentialFuncGenerator,forwardFunctionalGenerator,t_end,N,mpcsteps,tau,50);
    mpc_uni.MPCloop();

    // Solve with an exponential grid
    std::cout<<"##########################"<<std::endl;
    std::cout<<"####  Exponential "<<N<<" ####"<<std::endl;
    std::cout<<"##########################"<<std::endl;
    ::Spacy::KaskadeParabolic::ModelPredictiveController<NormalFunctionalDefinition,TangentialFunctionalDefinition,HeatFunctionalDefinition>
        mpc_exp(normalFuncGenerator,tangentialFuncGenerator,forwardFunctionalGenerator,t_end,N,mpcsteps,tau,50,"exponential",expparam);
    //    mpc_exp(normalFuncGenerator,tangentialFuncGenerator,forwardFunctionalGenerator,t_end,N,mpcsteps,tau,"exponential",-0.5);
    mpc_exp.MPCloop();
  }


  /// Solve with extremely fine grid for reference
  std::cout<<"##########################"<<std::endl;
  std::cout<<"####  Exact sol. "<<401<<" ####"<<std::endl;
  std::cout<<"##########################"<<std::endl;
  {
    ::Spacy::KaskadeParabolic::ModelPredictiveController<NormalFunctionalDefinition,TangentialFunctionalDefinition,HeatFunctionalDefinition>
        mpc_uni(normalFuncGenerator,tangentialFuncGenerator,forwardFunctionalGenerator,t_end,401,mpcsteps,tau,100/*,"exponential",-0.5*/);
    mpc_uni.MPCloop();
  }

  std::cout<<"returning from main"<<std::endl;
  return 0;
}

