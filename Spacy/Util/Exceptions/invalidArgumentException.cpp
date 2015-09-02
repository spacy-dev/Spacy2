#include "invalidArgumentException.hh"

namespace Spacy
{
  InvalidArgumentException::InvalidArgumentException(const std::string& function)
    : std::runtime_error("In " + function + ": Invalid arguments.\n")
  {}
}
