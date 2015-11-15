#ifndef SPACY_MOCK_LINEAR_SOLVER_HH
#define SPACY_MOCK_LINEAR_SOLVER_HH

namespace Spacy
{
  class Vector;
}

namespace Mock
{
  struct IndefiniteLinearSolver
  {
    Spacy::Vector operator()(const Spacy::Vector& x) const;

    bool isPositiveDefinite() const;
  };
}

#endif // SPACY_MOCK_LINEAR_SOLVER_HH
