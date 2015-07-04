#include "abstractFunctional.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractFunctional::AbstractFunctional(std::shared_ptr<AbstractBanachSpace> domain)
      : domain_(domain)
    {}

    AbstractFunctional::~AbstractFunctional(){}

    double AbstractFunctional::operator ()(const AbstractFunctionSpaceElement& x) const
    {
      if( getDomain().index() != x.getSpace().index() ) throw InvalidArgumentException("AbstractFunctional::operator() (incompatible space ids)");

      return d0(x);
    }

    const AbstractBanachSpace& AbstractFunctional::getDomain() const
    {
      return *domain_;
    }

    std::shared_ptr<AbstractBanachSpace> AbstractFunctional::getSharedDomain() const
    {
      return domain_;
    }
  }
}
