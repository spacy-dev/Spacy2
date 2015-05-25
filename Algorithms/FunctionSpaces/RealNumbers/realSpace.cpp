#include "realSpace.hh"

#include "../../Util/invalidargumentexception.hh"

#include "../../hilbertSpaceNorm.hh"

#include "real.hh"
#include "realProduct.hh"

namespace Algorithm
{
  RealSpace::RealSpace()
  {
    this->setScalarProduct( std::make_shared<RealProduct>() );
  }

  void RealSpace::setNormImpl(std::shared_ptr<AbstractNorm> norm)
  {
    if( dynamic_cast<const HilbertSpaceNorm*>(norm.get()) == nullptr ) throw InvalidArgumentException("RealSpace::setNormImpl");

    this->norm_ = norm;
  }

  void RealSpace::setScalarProductImpl(std::shared_ptr<AbstractScalarProduct> sp)
  {
    sp_ = sp;
  }

  std::shared_ptr<AbstractScalarProduct> RealSpace::getScalarProductImpl() const
  {
    return sp_;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> RealSpace::elementImpl() const
  {
    return std::make_unique<Real>(*this);
  }
}
