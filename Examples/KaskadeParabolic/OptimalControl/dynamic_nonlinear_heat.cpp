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
#include "dynamic_nonlinear_control.hh"

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
    double alpha = getParameter(pt, "alpha", 1e-2);
    int maxSteps = getParameter(pt, "maxSteps", 500);
    int initialRefinements = getParameter(pt, "initialRefinements", 5);
    int iterativeRefinements = getParameter(pt, "iterativeRefinements", 0);
    int FEorder = getParameter(pt, "FEorder", 1);
    int verbose = getParameter(pt, "verbose", 2);
    double c = getParameter(pt, "cPara", 1e0);
    double d = getParameter(pt, "dPara", 1e0);
    double e = getParameter(pt, "ePara", 0.0);
    double desContr = getParameter(pt, "desiredContraction", 0.5);
    double relDesContr = getParameter(pt, "relaxedContraction", desContr+0.1);
    double maxContr = getParameter(pt, "maxContraction", 0.75);

    unsigned no_time_steps = 11;
    double dt_size = 0.1;

    using std::cout;
    using std::endl;


    cout << "cPara: " << c << " dPara: " << d << " ePara: " << e << " alpha:" << alpha << endl;
    cout << "dContr: " << desContr << " rdContr: " << relDesContr << " mContr: " << maxContr << endl;


    typedef Dune::UGGrid<dim> Grid;
    typedef FEFunctionSpace<ContinuousLagrangeMapper<double,Grid::LeafGridView> > H1Space;
    typedef boost::fusion::vector<H1Space const*> Spaces;
//    using VU = VariableDescription<0,1,Ids::control>;
//    using VY = VariableDescription<0,1,Ids::state>;
//    using VP = VariableDescription<0,1,Ids::adjoint>;
//    typedef boost::fusion::vector< VY , VU , VP > VariableDescriptions;
//    typedef VariableSetDescription<Spaces,VariableDescriptions> VariableSetDesc;

    ::Spacy::KaskadeParabolic::GridManager<Spaces> gm (no_time_steps,dt_size,initialRefinements,FEorder);

    auto domain = Spacy::KaskadeParabolic::makeHilbertSpace(gm,{0u,1u},{2u});

    auto x = zero(domain);

    auto x2 = zero(domain);


    std::cout<<"returning from main"<<std::endl;
    return 0;
}

