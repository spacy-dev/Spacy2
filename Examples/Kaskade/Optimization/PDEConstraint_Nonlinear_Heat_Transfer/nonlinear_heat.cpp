/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*  This file is part of the library KASKADE 7                               */
/*    see http://www.zib.de/Numerik/numsoft/kaskade7/                        */
/*                                                                           */
/*  Copyright (C) 2002-2013 Zuse Institute Berlin                            */
/*                                                                           */
/*  KASKADE 7 is distributed under the terms of the ZIB Academic License.    */
/*    see $KASKADE/academic.txt                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define FUSION_MAX_VECTOR_SIZE 15

#include <chrono>
#include <iostream>

#include <dune/grid/config.h>
#include <dune/grid/uggrid.hh>

#include <Spacy/Adapter/kaskade.hh>
#include <Spacy/Algorithm/CompositeStep/affineCovariantSolver.hh>

#include "fem/gridmanager.hh"
#include "fem/lagrangespace.hh"
#include "fem/forEach.hh"
#include "io/vtk.hh"
#include "utilities/kaskopt.hh"
#include "utilities/gridGeneration.hh"


#define NCOMPONENTS 1
#include "nonlinear_control.hh"


int main(int argc, char *argv[])
{
  using namespace Kaskade;
  
  static constexpr int stateId = 0;
  static constexpr int controlId = 1;
  static constexpr int adjointId = 2;
  
  constexpr int dim = 2;
  int silence = 0;
  std::unique_ptr<boost::property_tree::ptree> pt = getKaskadeOptions(argc, argv, silence, false);
  
  double desiredAccuracy = getParameter(pt, "desiredAccuracy", 1e-6);
  double eps = getParameter(pt, "eps", 1e-12);
  double alpha = getParameter(pt, "alpha", 1e-2);
  int maxSteps = getParameter(pt, "maxSteps", 500);
  int initialRefinements = getParameter(pt, "initialRefinements", 7);
  int iterativeRefinements = getParameter(pt, "iterativeRefinements", 0);
  int FEorder = getParameter(pt, "FEorder", 1);
  int verbose = getParameter(pt, "verbose", 2);
  double c = getParameter(pt, "cPara", 1e0);
  double d = getParameter(pt, "dPara", 1e0);
  double e = getParameter(pt, "ePara", 0.0);
  double desContr = getParameter(pt, "desiredContraction", 0.5);
  double relDesContr = getParameter(pt, "relaxedContraction", desContr+0.1);
  double maxContr = getParameter(pt, "maxContraction", 0.75);

  using std::cout;
  using std::endl;


  cout << "cPara: " << c << " dPara: " << d << " ePara: " << e << " alpha:" << alpha << endl; 
  cout << "dContr: " << desContr << " rdContr: " << relDesContr << " mContr: " << maxContr << endl; 


  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  /* * * * * * * * * * * * * * * * * * * * * * grid generation * * * * * * * * * * * * * * * * * * * * * */
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  typedef Dune::UGGrid<dim> Grid;
  typedef Grid::LeafGridView GridView;
  
  GridManager<Grid> gm( createUnitSquare<Grid>(1.,false));
  gm.enforceConcurrentReads(true);
  gm.globalRefine(initialRefinements);

  std::cout << "vertices: " << gm.grid().size(dim) << std::endl;

  std::cout << std::setprecision(10);
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  /* * * * * * * * * * * * * * * * * * * * * * function spaces * * * * * * * * * * * * * * * * * * * * * */
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  typedef FEFunctionSpace<ContinuousLagrangeMapper<double,GridView> > H1Space;
  
  typedef boost::fusion::vector<H1Space const*> Spaces;
  using VU = VariableDescription<0,1,controlId>;
  using VY = VariableDescription<0,1,stateId>;
  using VP = VariableDescription<0,1,adjointId>;
  typedef boost::fusion::vector< VY , VU , VP > VariableDescriptions;
  typedef VariableSetDescription<Spaces,VariableDescriptions> Descriptions;


  typedef Descriptions::VariableSet VarSet;

  // construct variable list.
  std::string names[] = { "y", "u", "p"};
  
  auto const& leafView = gm.grid().leafGridView();
  // construct involved spaces.
  H1Space h1Space(gm,leafView,FEorder);
  Spaces spaces(&h1Space);
  Descriptions desc(spaces,names);

  // Reference solution
  cout << "interpolate" << endl;
  VarSet x_ref(desc);
  interpolateGloballyFromFunctor<PlainAverage>(boost::fusion::at_c<stateId>(x_ref.data), [](auto const& cell, auto const& xLocal) -> Dune::FieldVector<double,1>
  {
    auto x = cell.geometry().global(xLocal);
    return Dune::FieldVector<double,1>(12*(1-x[1])*x[1]*(1-x[0])*x[0]);
  });

  cout << "create domain" << endl;
  auto domain = Spacy::Kaskade::makeHilbertSpace<Descriptions>(spaces, {0u,1u}, {2u});
  // Normal step functional with cg solver
  //auto fn = Spacy::Kaskade::makeLagrangeCGFunctional<stateId,controlId,adjointId>( NormalStepFunctional<stateId,controlId,adjointId,double,Descriptions>(alpha,x_ref,c,d) , domain );


  cout << "create functional" << endl;
  // Normal step functional with direct solver
  auto fn = Spacy::Kaskade::makeC2Functional( NormalStepFunctional<stateId,controlId,adjointId,double,Descriptions>(alpha,x_ref,c,d,e) , domain );
  
  //~ using Impl = decltype(fn);
  //~ cout << "copy constructible: " << std::is_copy_constructible<Impl>::value << endl;
  //~ cout << "copy assignable: " << std::is_copy_assignable<Impl>::value << endl;
  //~ cout << "callable: " << Spacy::HasMemOp_callable<Impl,Spacy::Vector,Spacy::Real>::value << endl;
  //~ cout << "domain: " << Spacy::HasMemFn_domain<Impl>::value << endl;
  //~ cout << "d1: " << Spacy::HasMemFn_d1_Functional<Impl,Spacy::Vector>::value << endl;
  //~ cout << "d2: " << Spacy::HasMemFn_d2_Functional<Impl,Spacy::Vector>::value << endl;
  //~ cout << "hessian: " << Spacy::HasMemFn_hessian<Impl,Spacy::Vector>::value << endl;
 //  auto solverCreator = Spacy::Kaskade::Lagrange::CGCreator<NormalStepFunctional<stateId,controlId,adjointId,double,Descriptions>,stateId,controlId,adjointId>{};
 // solverCreator.setVerbosity(true);
 // fn.setSolverCreator( solverCreator );
  
  // Lagrange functional
  cout << "make tangential functional " << endl;
  auto ft = Spacy::Kaskade::makeC2Functional( TangentialStepFunctional<stateId,controlId,adjointId,double,Descriptions>(alpha,x_ref,c,d,e) , domain );

  cout << "set up solver" << endl;
  // algorithm and parameters
  auto cs = Spacy::CompositeStep::AffineCovariantSolver( fn , ft , domain );
  cs.setRelativeAccuracy(desiredAccuracy);
  cs.setEps(eps);
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

  VarSet x(desc);
  Spacy::Kaskade::copy(result,x);


  IoOptions options;
  options.outputType = IoOptions::ascii;
  std::string outfilename("nonlinear_control");
  writeVTKFile(gm.grid().leafGridView(),x,outfilename,options,FEorder);
  writeVTKFile(gm.grid().leafGridView(),x_ref,"reference",options,FEorder);
}

