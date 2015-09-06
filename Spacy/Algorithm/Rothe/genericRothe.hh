#ifndef SPACY_ALGORITHM_GENERIC_ROTHE_HH
#define SPACY_ALGORITHM_GENERIC_ROTHE_HH

#include "Spacy/dynamicOperator.hh"
#include "Spacy/vector.hh"

namespace Spacy
{
  class TimeIntegrator;

  namespace Rothe
  {
    Vector genericMethod(const DynamicC1Operator& A, TimeIntegrator integrator);
  }
}

#endif // SPACY_ALGORITHM_GENERIC_ROTHE_HH
