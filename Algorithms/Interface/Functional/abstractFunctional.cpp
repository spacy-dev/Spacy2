#include "abstractFunctional.hh"

#include "Interface/abstractFunctionSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractFunctional::AbstractFunctional(std::shared_ptr<AbstractFunctionSpace> domain)
      : domain_(domain)
    {}

    AbstractFunctional::~AbstractFunctional(){}

    double AbstractFunctional::operator ()(const AbstractFunctionSpaceElement& x) const
    {
      if( domain().index() != x.space().index() ) throw InvalidArgumentException("AbstractFunctional::operator() (incompatible space ids)");

      return d0(x);
    }

    const AbstractFunctionSpace& AbstractFunctional::domain() const
    {
      return *domain_;
    }

    std::shared_ptr<AbstractFunctionSpace> AbstractFunctional::sharedDomain() const
    {
      return domain_;
    }
  }
}
