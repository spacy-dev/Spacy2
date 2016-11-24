// enable logging
#define SPACY_ENABLE_LOGGING

#include <Spacy/spacy.h>

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
