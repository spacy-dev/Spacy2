#include "productSpace.hh"

#include "../../Util/invalidargumentexception.hh"

#include "../../hilbertSpaceNorm.hh"

#include "productSpaceElement.hh"
#include "productSpaceProduct.hh"

#include <utility>

namespace Algorithm
{
  ProductSpace::ProductSpace(std::vector<std::unique_ptr<AbstractBanachSpace> >&& primalSpaces)
    : primalSpaces_(std::move(primalSpaces))
  {
    this->setScalarProduct( std::make_shared<ProductSpaceProduct>() );
  }

  ProductSpace::ProductSpace(std::vector<std::unique_ptr<AbstractBanachSpace> >&& primalSpaces,
               std::vector<std::unique_ptr<AbstractBanachSpace> >&& dualSpaces)
    : primalSpaces_(std::move(primalSpaces)), dualSpaces_(std::move(dualSpaces))
  {
    this->setScalarProduct( std::make_shared<ProductSpaceProduct>() );
  }

//  ProductSpace::ProductSpace(std::vector<AbstractBanachSpace>&& primalSpaces,
//               std::vector<AbstractBanachSpace>&& dualSpaces)
//  {
//    for(auto& space : primalSpaces) primalSpaces_.emplace_back<std::shared_ptr<AbstractBanachSpace> >(space.clone());
//    for(auto& space : dualSpaces) dualSpaces_.emplace_back<std::shared_ptr<AbstractBanachSpace> >(space.clone());

//    this->setScalarProduct( std::make_shared<ProductSpaceProduct>() );
//  }

  const std::vector<std::unique_ptr<AbstractBanachSpace> >& ProductSpace::getPrimalSpaces() const
  {
    return primalSpaces_;
  }

  const std::vector<std::unique_ptr<AbstractBanachSpace> >& ProductSpace::getDualSpaces() const
  {
    return dualSpaces_;
  }

  void ProductSpace::setNormImpl(std::shared_ptr<AbstractNorm> norm)
  {
    if( dynamic_cast<const HilbertSpaceNorm*>(norm.get()) == nullptr ) throw InvalidArgumentException("ProductSpace::setNormImpl");

    this->norm_ = norm;
  }

  void ProductSpace::setScalarProductImpl(std::shared_ptr<AbstractDualPairing> sp)
  {
    sp_ = sp;
  }

  std::shared_ptr<AbstractDualPairing> ProductSpace::getScalarProductImpl() const
  {
    return sp_;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpace::elementImpl() const
  {
    return std::make_unique<ProductSpaceElement>(*this);
  }
}
