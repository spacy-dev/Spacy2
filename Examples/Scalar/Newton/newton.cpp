// enable logging
#define SPACY_ENABLE_LOGGING

#include <Spacy/linearOperator.hh>
#include <Spacy/c1Operator.hh>
#include <Spacy/Adapter/scalar.hh>
#include <Spacy/Algorithm/Newton/newton.hh>
#include <Spacy/Algorithm/Newton/terminationCriteria.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/log.hh>

int main()
{
    DISABLE_LOG_TAG(Spacy::Newton::Termination::covariant_log_tag);

    // nonlinear operator  A  :  R -> R,
    //                     x |-> A(x) = x*x - 4
    auto A = Spacy::Scalar::C1Operator( [] (double x) { return x*x+5*x+4; },
                                        [] (double x) { return 2*x+5; } );

    // solve Ax=0 with Newton's method with x0=0
    auto x = localNewton(A);

    // print result using the logging mechanism of Spacy
    LOG("Result", "x =", Spacy::cast_ref<Spacy::Real>(x) );
}
