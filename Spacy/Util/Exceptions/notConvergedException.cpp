#include "notConvergedException.hh"

namespace Spacy
{
  namespace Exception
  {
    NotConverged::NotConverged(const std::string& nameOfAlgorithm)
      : std::runtime_error( nameOfAlgorithm + " method did not converge.\n")
    {}
  }
}
