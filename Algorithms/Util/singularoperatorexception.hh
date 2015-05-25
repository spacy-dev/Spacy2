#ifndef SINGULAROPERATOREXCEPTION_HH
#define SINGULAROPERATOREXCEPTION_HH

#include <stdexcept>
#include <string>

namespace Algorithm
{
  /// Exception to be thrown if singular operators are inverted.
  class SingularOperatorException : public std::runtime_error
  {
  public:
    SingularOperatorException(const std::string&);
  };
}

#endif // SINGULAROPERATOREXCEPTION_HH
