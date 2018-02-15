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
#include "optimal_control_fung.hh"

#include <fung/fung.hh>
#include <fung/examples/nonlinear_heat.hh>


struct Ids
{
  static constexpr int state = 0;
  static constexpr int control = 1;
  static constexpr int adjoint = 2;
};

template <class StateVector, class Reference, class ControlVector>
auto tracking_type_cost_functional(double alpha,
                                   const StateVector& y,
                                   const Reference& y_ref,
                                   const ControlVector& u)
{
  using namespace FunG;
  auto f = squared( variable<Ids::state>(y) - constant(y_ref) ) +
      alpha*squared( variable<Ids::control>(u) );
  return finalize(f);
}

int main(int argc, char *argv[])
{
  using namespace Kaskade;
  constexpr int dim = 2;
  int silence = 0;
  std::unique_ptr<boost::property_tree::ptree> pt = getKaskadeOptions(argc, argv, silence, false);

  double desiredAccuracy = getParameter(pt, "desiredAccuracy", 1e-6);
  double eps = getParameter(pt, "eps", 1e-12);
  double alpha = getParameter(pt, "alpha", 1e-6);
  int maxSteps = getParameter(pt, "maxSteps", 500);
  int initialRefinements = getParameter(pt, "initialRefinements", 4);
  int iterativeRefinements = getParameter(pt, "iterativeRefinements", 0);
  int FEorder = getParameter(pt, "FEorder", 1);
  int verbose = getParameter(pt, "verbose", 2);
  double c = getParameter(pt, "cPara", 1);
  double d = getParameter(pt, "dPara", 1);
  double e = getParameter(pt, "ePara", 0.0);
  double desContr = getParameter(pt, "desiredContraction", 0.5);
  double relDesContr = getParameter(pt, "relaxedContraction", desContr+0.1);
  double maxContr = getParameter(pt, "maxContraction", 0.75);

  unsigned no_time_steps = 11;
  double t_end = 1;

  using std::cout;
  using std::endl;


  cout << "cPara: " << c << " dPara: " << d << " ePara: " << e << " alpha:" << alpha << endl;
  cout << "dContr: " << desContr << " rdContr: " << relDesContr << " mContr: " << maxContr << endl;


  typedef Dune::UGGrid<dim> Grid;
  typedef FEFunctionSpace<ContinuousLagrangeMapper<double,Grid::LeafGridView> > H1Space;
  typedef boost::fusion::vector<H1Space const*> Spaces;
  using VY = VariableDescription<0,1,Ids::state>;
  using VU = VariableDescription<0,1,Ids::control>;
  using VP = VariableDescription<0,1,Ids::adjoint>;
  typedef boost::fusion::vector< VY , VU , VP > VariableDescriptions;
  typedef VariableSetDescription<Spaces,VariableDescriptions> Descriptions;

  // ################## Grid Generation (SPACE AND TIME) ##################
  ::Spacy::KaskadeParabolic::GridManager<Spaces> gm (no_time_steps,::Spacy::Real{t_end},initialRefinements,FEorder);
  gm.getTempGrid().print();

  // ################## Domain Generation ##################
  auto domain = Spacy::KaskadeParabolic::makeHilbertSpace(gm,{0u,1u},{2u});


  // ################## Constraint and Cost functional Generation ##################
  Dune::FieldVector<double,1> y0{0},u0{0},y_r{1};
  Dune::FieldMatrix<double,1,dim> dy0{0};
  auto constraint = FunG::heatModel(c, d, y0, dy0);
  std::function<void(const Dune::FieldVector<double,1>&)> update_reference =
      [&y_r](const Dune::FieldVector<double,1>& ref){ y_r = ref; };

  auto costFunctional = tracking_type_cost_functional(alpha,y0,y_r,u0);

  std::cout<<"Creating Functionals"<<std::endl;

  // ################## Normal Step Functional Generation ##################
  using NormalFunctionalDefinition = NormalStepFunctional<Ids,decltype(constraint),decltype(costFunctional),Descriptions>;
  std::function<NormalFunctionalDefinition(typename Descriptions::VariableSet)> normalFuncGenerator = [&constraint,costFunctional,update_reference](typename Descriptions::VariableSet y_ref) {
    return NormalStepFunctional<Ids,decltype(constraint),decltype(costFunctional),Descriptions>(constraint,costFunctional,y_ref,update_reference);
  };

  auto n_func = Spacy::KaskadeParabolic::makeC2Functional(normalFuncGenerator,gm,domain);

  // ################## Tangential Step Functional Generation ##################
  using TangentialFunctionalDefinition = TangentialStepFunctional<Ids,decltype(constraint),decltype(costFunctional),Descriptions>;
  std::function<TangentialFunctionalDefinition(typename Descriptions::VariableSet)> tangentialFuncGenerator = [&constraint,costFunctional,update_reference](typename Descriptions::VariableSet y_ref) {
    return TangentialStepFunctional<Ids,decltype(constraint),decltype(costFunctional),Descriptions>(constraint,costFunctional,y_ref,update_reference);
  };

  auto t_func = Spacy::KaskadeParabolic::makeC2Functional(tangentialFuncGenerator,gm,domain);

  // ################## Solver Generation ##################
  cout << "set up solver" << endl;
  // algorithm and parameters
  auto cs = Spacy::CompositeStep::AffineCovariantSolver( n_func , t_func , domain );
  cs.setRelativeAccuracy(desiredAccuracy);
  cs.set_eps(eps);
  cs.setVerbosityLevel(2);
  cs.setMaxSteps(maxSteps);
  cs.setIterativeRefinements(iterativeRefinements);
  cs.setDesiredContraction(desContr);
  cs.setRelaxedDesiredContraction(relDesContr);
  cs.setMaximalContraction(maxContr);

  cout << "start solver" << endl;
  using namespace std::chrono;
  auto startTime = high_resolution_clock::now();
  auto result = cs();
  std::cout << "computation time: " << duration_cast<seconds>(high_resolution_clock::now() - startTime).count() << "s." << std::endl;


  // print solution to file
  ::Spacy::KaskadeParabolic::OCP::writeVTK<Descriptions>(result,"sol");

  return 0;
}

