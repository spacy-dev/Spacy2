#ifndef ALGORITHM_ADAPTER_KASKADE_HILBERT_SPACE_HH
#define ALGORITHM_ADAPTER_KASKADE_HILBERT_SPACE_HH

#include <memory>

#include "Interface/abstractHilbertSpace.hh"
#include "Util/Mixins/impl.hh"
#include "Util/create.hh"

#include "l2Product.hh"
#include "vector.hh"

#include "../../hilbertSpace.hh"

namespace Algorithm
{
  /// \cond
  namespace Interface { class AbstractFunctionSpaceElement; }
  /// \endcond

  namespace Kaskade
  {
    template <class Space, class Variable>
    class HilbertSpace : public Interface::AbstractHilbertSpace , public Mixin::Impl<Space>
    {
      using Description = ::Kaskade::VariableSetDescription< boost::fusion::vector< const Space* > , boost::fusion::vector< Variable > >;
      using VectorImpl = typename Description::template CoefficientVectorRepresentation<>::type;
    public:
      HilbertSpace(const Space& space)
        : Interface::AbstractHilbertSpace( std::make_shared< l2Product<Description> >() ),
          Mixin::Impl<Space>(space)
      {}

    private:
      std::unique_ptr<Interface::AbstractFunctionSpaceElement> elementImpl() const override
      {
        return std::make_unique< Vector<Description> >(*this);
      }
    };

    template <class Variable, class Space>
    auto makeHilbertSpace(const Space& space)
    {
      return createFromSharedImpl< ::Algorithm::HilbertSpace , HilbertSpace<Space,Variable> >( space );
    }
  }
}

#endif // ALGORITHM_ADAPTER_KASKADE_HILBERT_SPACE_HH
