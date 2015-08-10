#ifndef ALGORITHM_SPACES_HH
#define ALGORITHM_SPACES_HH

#include <memory>

#include "functionSpace.hh"
#include "FunctionSpaces/RealNumbers/realSpace.hh"

namespace Algorithm
{
  namespace Spaces
  {
    FunctionSpace R(std::make_shared<RealSpace>());
  }
}
#endif // ALGORITHM_SPACES_HH
