#include "invalidArgumentException.hh"

namespace Spacy
{
  namespace Exception
  {
    InvalidArgument::InvalidArgument(const std::string& function)
      : std::runtime_error("In " + function + ": Invalid arguments.\n")
    {}
  }
}
