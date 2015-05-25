#ifndef ALGORITHM_FUNCTIONSPACES_VECTOR_SPACE_VECTOR_SPACE_HH
#define ALGORITHM_FUNCTIONSPACES_VECTOR_SPACE_VECTOR_SPACE_HH

#include <memory>

#include "../../Interface/abstractHilbertSpace.hh"
#include "../../hilbertSpaceNorm.hh"
#include "../../Util/invalidargumentexception.hh"

#include "l2Product.hh"
#include "vectorSpaceElement.hh"

namespace Algorithm
{
  template <class Vector>
  class VectorSpace : public AbstractHilbertSpace
  {
  public:
    VectorSpace()
    {
      this->setScalarProduct( std::make_shared< l2Product<Vector> >() );
    }

  private:
    void setNormImpl(std::shared_ptr<AbstractNorm> norm) override
    {
      if( dynamic_cast<const HilbertSpaceNorm*>(norm.get()) == nullptr ) throw InvalidArgumentException("RealSpace::setNormImpl");

      this->norm_ = norm;
    }

    void setScalarProductImpl(std::shared_ptr<AbstractScalarProduct> sp) override
    {
      sp_ = sp;
    }

    std::shared_ptr<AbstractScalarProduct> getScalarProductImpl() const override
    {
      return sp_;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const override
    {
      return std::make_unique< VectorSpaceElement<Vector> >(*this);
    }

    std::shared_ptr<AbstractScalarProduct> sp_;
  };
}

#endif // ALGORITHM_FUNCTIONSPACES_VECTOR_SPACE_VECTOR_SPACE_HH
