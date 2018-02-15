#define FUSION_MAX_VECTOR_SIZE 15

#include <chrono>
#include <functional>
#include <iostream>

#include <dune/grid/config.h>
#include <dune/grid/uggrid.hh>

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Spaces/productSpace.hh>
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

  double desiredAccuracy = getParameter(pt, "desiredAccuracy", 1e-6);
  double eps = getParameter(pt, "eps", 1e-12);
  double alpha = getParameter(pt, "alpha", 0.5);
  int maxSteps = getParameter(pt, "maxSteps", 500);
  int initialRefinements = getParameter(pt, "initialRefinements", 4);
  int iterativeRefinements = getParameter(pt, "iterativeRefinements", 0);
  int FEorder = getParameter(pt, "FEorder", 1);
  int verbose = getParameter(pt, "verbose", 2);
  double d = getParameter(pt, "dPara", 1e-1);
  double mu = getParameter(pt, "muPara", 0);
  double desContr = getParameter(pt, "desiredContraction", 0.5);
  double relDesContr = getParameter(pt, "relaxedContraction", desContr+0.1);
  double maxContr = getParameter(pt, "maxContraction", 0.75);
  double expgridparam = getParameter(pt, "expGrid",-0.345);

  using std::cout;
  using std::endl;
  cout << " dPara: " << d << " muPara: " << mu << " alpha:" << alpha << endl;

  using Grid = Dune::UGGrid<dim>;
  using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,Grid::LeafGridView> >;
  using Spaces = boost::fusion::vector<H1Space const*>;
  using VY = VariableDescription<0,1,stateId>;
  using VU = VariableDescription<0,1,controlId>;
  using VP = VariableDescription<0,1,adjointId>;
  using VariableDescriptions = boost::fusion::vector< VY , VU , VP > ;
  using Descriptions = VariableSetDescription<Spaces,VariableDescriptions>;


  ::Spacy::KaskadeParabolic::GridManager<Spaces> gm (11,::Spacy::Real{30.},initialRefinements,FEorder);
  auto domain2 = Spacy::KaskadeParabolic::makeHilbertSpace(gm,{0u,1u},{2u});


  using NormalFunctionalDefinition =  NormalStepFunctional<stateId,controlId,adjointId,double,Descriptions>;
  std::function<NormalFunctionalDefinition(const typename Descriptions::VariableSet)> normalFuncGenerator = [&d, &mu, &alpha](const typename Descriptions::VariableSet y_ref) {
    return NormalFunctionalDefinition(alpha,y_ref,d,mu);
  };

  using TangentialFunctionalDefinition =  TangentialStepFunctional<stateId,controlId,adjointId,double,Descriptions>;

  std::function<TangentialFunctionalDefinition(const typename Descriptions::VariableSet)> tangentialFuncGenerator =[&d, &mu, &alpha](const typename Descriptions::VariableSet y_ref) {
    return TangentialFunctionalDefinition(alpha,y_ref,d,mu);
  };


  //####################### OCP SOLUTION #######################
  std::cout<<"Creating Functionals"<<std::endl;
  auto n_func2 = Spacy::KaskadeParabolic::makeC2Functional(normalFuncGenerator,gm,domain2);
  auto t_func2 = Spacy::KaskadeParabolic::makeC2Functional(tangentialFuncGenerator,gm,domain2);

  cout << "set up solver" << endl;
  // algorithm and parameters
  auto cs2 = Spacy::CompositeStep::AffineCovariantSolver( n_func2 , t_func2 , domain2 );
  cs2.setRelativeAccuracy(desiredAccuracy);
  cs2.set_eps(eps);
  cs2.setVerbosityLevel(2);
  cs2.setMaxSteps(maxSteps);
  cs2.setIterativeRefinements(iterativeRefinements);
  cs2.setDesiredContraction(desContr);
  cs2.setRelaxedDesiredContraction(relDesContr);
  cs2.setMaximalContraction(maxContr);
  auto uniform_solution = cs2();
  ::Spacy::KaskadeParabolic::OCP::writeVTK<Descriptions>(uniform_solution,"sol");


  using HeatFunctionalDefinition = LinearModelPDE<double,::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,0> , ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,1>  >;
  std::function<HeatFunctionalDefinition(const typename ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,1>::VariableSet )> forwardFunctionalGenerator
      = [&d,&mu](const typename ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,1>::VariableSet control){
    return HeatFunctionalDefinition(d,mu,control);
  };
  //####################### PDE SOLUTION #######################
  auto domain_forward_ = Spacy::KaskadeParabolic::makeHilbertSpace(gm);
  auto z = zero(domain2);
  auto z_ps = ::Spacy::cast_ref<::Spacy::ProductSpace::Vector> (z);
  ::Spacy::C1Operator  A = ::Spacy::KaskadeParabolic::makeC1Operator<HeatFunctionalDefinition>(forwardFunctionalGenerator, gm , domain_forward_ , domain_forward_.dualSpace(),
                                                                                  ::Spacy::cast_ref<::Spacy::ProductSpace::Vector>(z_ps.component(0)).component(1));
  domain_forward_.setScalarProduct( Spacy::InducedScalarProduct( ::Spacy::cast_ref<::Spacy::KaskadeParabolic::C1Operator<HeatFunctionalDefinition> >(A).massMatrix() ));


  auto result_ps = ::Spacy::cast_ref<::Spacy::ProductSpace::Vector> ( uniform_solution);
  ::Spacy::cast_ref<::Spacy::KaskadeParabolic::C1Operator<HeatFunctionalDefinition> >(A).setSource((::Spacy::cast_ref<::Spacy::ProductSpace::Vector>(result_ps.component(0))).component(1));

  auto p = Spacy::Newton::Parameter{};
  p.setRelativeAccuracy(1e-12);
  auto y = covariantNewton(A,p);

  using DescriptionsY = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,0>;
  using CoefficientVectorY = typename DescriptionsY::template CoefficientVectorRepresentation<>::type;

//  ::Spacy::KaskadeParabolic::PDE::writeVTK(::Spacy::cast_ref<::Spacy::KaskadeParabolic::Vector<Descriptions> > (y),"sol_forward");
  ::Spacy::KaskadeParabolic::PDE::writeVTK<DescriptionsY> (y,"sol_forward");

  auto y1 = (::Spacy::cast_ref<::Spacy::ProductSpace::Vector>(result_ps.component(0))).component(0);
  auto y1impl = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::Vector<DescriptionsY> >(y1);
  auto y2impl = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::Vector<DescriptionsY> >(y);


  for(auto i = 0u; i< gm.getSpacesVec().size(); i++)
  {
    CoefficientVectorY y1coeff(
          Descriptions::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
    CoefficientVectorY y2coeff(
          Descriptions::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));

    boost::fusion::at_c<0>(y1coeff.data) = y1impl.getCoeffVec(i);
    boost::fusion::at_c<0>(y2coeff.data) = y2impl.getCoeffVec(i);

    y1coeff -= y2coeff;

    auto My = y1coeff;
    ::Spacy::cast_ref<::Spacy::KaskadeParabolic::C1Operator<HeatFunctionalDefinition> >(A).linearization(zero(domain_forward_)).getKaskOp("MassY", i).apply(y1coeff, My);
    std::cout<<"norm "<<i<<y1coeff*My<<std::endl;
  }



  std::cout<<"returning from main"<<std::endl;
  return 0;
}

