// enable logging

#include <Spacy/spacy.h>
#include <Spacy/Algorithm/TrustRegion/TrustRegionSolver.hh>

using namespace Spacy;

int main()
{
    // nonlinear operator  A  :  R -> R,
    //                     x |-> A(x) = x*x - 4
    auto f = Scalar::C2Functional( [] (double x) { return (x-1)*(x-1)*(x-1)*(x-1) + x + 3; },
                                          [] (double x) { return 4*(x-1)*(x-1)*(x-1) + 1; },
                                          [] (double x) { return 12*(x-1)*(x-1); } );

    auto solver = Algorithm::TrustRegionSolver(f);
    solver.setMaxSteps(10);

    auto x = solver(Real(0));

    // print result using the logging mechanism of Spacy
    LOG("Result", "x =", cast_ref<Real>(x) );
}
