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
  template <class Vector>
  class KaskadeVectorSpace : public AbstractHilbertSpace
  {
  public:
    KaskadeVectorSpace(const Vector& v)
      : AbstractHilbertSpace( std::make_shared< l2Product<Vector> >() ), v_(v)
    {}

  private:
    std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const override
    {
      return std::make_unique< VectorSpaceElement<Vector> >(v_,*this);
    }

    std::shared_ptr<AbstractDualPairing> sp_;
    Vector v_;
  };
}

#endif // ALGORITHM_FUNCTIONSPACES_VECTOR_SPACE_KASKADE_VECTOR_SPACE_HH
