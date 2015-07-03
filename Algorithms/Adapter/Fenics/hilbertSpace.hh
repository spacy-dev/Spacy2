#ifndef ALGORITHMS_ADAPTER_FENICS_HILBERTSPACE_HH
#define ALGORITHMS_ADAPTER_FENICS_HILBERTSPACE_HH

#include <memory>

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Interface/abstractHilbertSpace.hh"
#include "Interface/abstractScalarProduct.hh"

#include "scalarProducts.hh"
#include "vector.hh"

#include "../../hilbertSpace.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class FenicsSpace>
    class HilbertSpace : public AbstractHilbertSpace
    {
    public:
      HilbertSpace(const FenicsSpace& space, const dolfin::Function& dummy)
        : AbstractHilbertSpace(std::make_shared<l2ScalarProduct>()),
          space_(space), dummy_(dummy)
      {
        *dummy_.vector() *= 0.;
      }

    private:
      std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const
      {
        return std::make_unique< Vector >(*this,dummy_);
      }

      const FenicsSpace& space_;
      dolfin::Function dummy_;
    };

    template <class FenicsSpace>
    auto makeHilbertSpace(const FenicsSpace& space, const dolfin::Function& dummy)
    {
      return Algorithm::makeHilbertSpace< HilbertSpace<FenicsSpace> >(space,dummy);
    }
  }
}

#endif // ALGORITHMS_ADAPTER_FENICS_HILBERTSPACE_HH
