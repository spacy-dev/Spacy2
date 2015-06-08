#ifndef ALGORITHM_FUNCTIONSPACES_VECTOR_SPACE_KASKADE_VECTOR_SPACE_HH
#define ALGORITHM_FUNCTIONSPACES_VECTOR_SPACE_KASKADE_VECTOR_SPACE_HH

#include <memory>

#include "Interface/abstractHilbertSpace.hh"
#include "hilbertSpaceNorm.hh"
#include "Util/invalidargumentexception.hh"

#include "l2Product.hh"
#include "vectorSpaceElement.hh"

namespace Algorithm
{
  class AbstractDualPairing;
  class AbstractFunctionSpaceElement;

  namespace Kaskade
  {
    template <class Vector>
    class VectorSpace : public AbstractHilbertSpace
    {
    public:
      VectorSpace(const Vector& v)
        : AbstractHilbertSpace( std::make_shared< ::Algorithm::Kaskade::l2Product<Vector> >() ), v_(v)
      {}

    private:
      std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const override
      {
        return std::make_unique< VectorSpaceElement<Vector> >(*this,v_);
      }

      std::shared_ptr<AbstractDualPairing> sp_;
      Vector v_;
    };
  }
}

#endif // ALGORITHM_FUNCTIONSPACES_VECTOR_SPACE_KASKADE_VECTOR_SPACE_HH
