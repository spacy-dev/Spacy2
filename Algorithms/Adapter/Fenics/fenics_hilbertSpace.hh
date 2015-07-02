#ifndef ALGORITHMS_ADAPTER_FENICS_HILBERTSPACE_HH
#define ALGORITHMS_ADAPTER_FENICS_HILBERTSPACE_HH

#include <memory>

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Interface/abstractHilbertSpace.hh"
#include "Interface/abstractScalarProduct.hh"

#include "fenics_functionSpaceElement.hh"
#include "fenics_l2scalarProduct.hh"

#include "hilbertSpace.hh"

namespace Algorithm
{
  template <class FenicsSpace>
  class Fenics_HilbertSpace : public AbstractHilbertSpace
  {
  public:
    Fenics_HilbertSpace(const FenicsSpace& space, const dolfin::Function& dummy)
      : AbstractHilbertSpace(std::make_shared<Fenics_DefaultScalarProduct>()),
        space_(space), dummy_(dummy)
    {
      *dummy_.vector() *= 0.;
    }

  private:
    std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const
    {
      return std::make_unique< Fenics_Vector >(*this,dummy_);
    }

    const FenicsSpace& space_;
    dolfin::Function dummy_;
  };

  template <class FenicsSpace>
  auto make_Fenics_HilbertSpace(const FenicsSpace& space, const dolfin::Function& dummy)
  {
    return makeHilbertSpace< Fenics_HilbertSpace<FenicsSpace> >(space,dummy);
  }
}

#endif // ALGORITHMS_ADAPTER_FENICS_HILBERTSPACE_HH
