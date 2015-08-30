#include "incompatibleSpaceException.hh"

namespace Algorithm
{
  IncompatibleSpaceException::IncompatibleSpaceException(unsigned spaceIndex1, unsigned spaceIndex2)
    : GenericException("Incompatible spaces.",
                       " First spaces  index: " + std::to_string(spaceIndex1) + "\n ------------ Second space index: " + std::to_string(spaceIndex2),
                       "Space indices must coincide.")
  {}
}
