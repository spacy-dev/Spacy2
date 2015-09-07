#ifndef SPACY_ADAPTER_FENICS_OPERATOR_SPACE_HH
#define SPACY_ADAPTER_FENICS_OPERATOR_SPACE_HH

#include "vector.hh"

namespace Spacy
{
  /// @cond
  class VectorSpace;
  /// @endcond

  namespace FEniCS
  {
    /// @cond
    class LinearOperator;
    /// @endcond

    class LinearOperatorCreator
    {
    public:
      LinearOperatorCreator(const VectorSpace& domain, const VectorSpace& range, std::shared_ptr<const dolfin::FunctionSpace> dolfinSpace);

      LinearOperator operator()(const VectorSpace* space) const;

      const VectorSpace& domain() const;

      const VectorSpace& range() const;

    private:
      const VectorSpace &domain_, &range_;
      std::shared_ptr<const dolfin::FunctionSpace> dolfinSpace_;
    };
  }
}

#endif // SPACY_ADAPTER_FENICS_OPERATOR_SPACE_HH

