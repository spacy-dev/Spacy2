#ifndef ALGORITHM_SPACES_HH
#define ALGORITHM_SPACES_HH

#include <memory>

#include "banachSpace.hh"
#include "hilbertSpace.hh"
#include "FunctionSpaces/RealNumbers/realSpace.hh"

namespace Algorithm
{
  namespace Spaces
  {
    HilbertSpace R(std::make_unique<RealSpace>());
  }
}
#endif // ALGORITHM_SPACES_HH
