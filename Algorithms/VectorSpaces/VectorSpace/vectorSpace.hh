#ifndef ALGORITHM_FUNCTIONSPACES_VECTOR_SPACE_VECTOR_SPACE_HH
#define ALGORITHM_FUNCTIONSPACES_VECTOR_SPACE_VECTOR_SPACE_HH

#include <memory>

#include "Interface/abstractHilbertSpace.hh"
#include "Interface/hilbertSpaceNorm.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

#include "l2Product.hh"
#include "vectorSpaceElement.hh"

namespace Algorithm
{
  template <class Vector>
  class VectorSpace : public Interface::AbstractHilbertSpace
  {
  public:
    VectorSpace()
      : Interface::AbstractHilbertSpace( std::make_shared< l2Product<Vector> >() )
    {}

  private:
    std::unique_ptr<Interface::AbstractFunctionSpaceElement> elementImpl() const override
    {
      return std::make_unique< VectorSpaceElement<Vector> >(*this);
    }

    std::shared_ptr<Interface::AbstractScalarProduct> sp_;
  };
}

#endif // ALGORITHM_FUNCTIONSPACES_VECTOR_SPACE_VECTOR_SPACE_HH
