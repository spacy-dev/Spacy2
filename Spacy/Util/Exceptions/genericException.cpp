#include "genericException.hh"

namespace Spacy
{
  namespace
  {
    std::string createExceptionMessage(const std::string& exception,
                                       const std::string& message,
                                       const std::string& hint)
    {
      std::string exceptionMessage = " --- Exception: " + exception + " ---\n";
      exceptionMessage += " --- Message: " + message + "\n\n";
      if( hint.empty() ) return exceptionMessage;
      exceptionMessage += " --- Hint: " + hint + "\n";
      return exceptionMessage;
    }
  }

  GenericException::GenericException(const std::string& exception,
                                     const std::string& message,
                                     const std::string& hint)
    : std::runtime_error(createExceptionMessage(exception,message,hint))
  {}
}
