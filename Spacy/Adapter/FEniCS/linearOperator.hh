#ifndef SPACY_OPERATORS_FENICS_LINEAR_OPERATOR_HH
#define SPACY_OPERATORS_FENICS_LINEAR_OPERATOR_HH

#include <functional>
#include <memory>
#include <utility>

#include "Spacy/linearSolver.hh"
#include "Spacy/Util/Base/addArithmeticOperators.hh"
#include "Spacy/Util/Base/operatorBase.hh"
#include "Spacy/Util/Base/vectorBase.hh"

#include "vectorSpace.hh"
#include "luSolver.hh"


namespace Spacy
{
  /// @cond
  class Vector;
  class VectorSpace;
  /// @endcond

  namespace FEniCS
  {
    /**
     * @ingroup FenicsGroup
     * @brief Linear operator interface for operators in %FEniCS.
     * @see LinearOperatorAnchor "LinearOperator", @ref LinearOperatorConceptAnchor "LinearOperatorConcept"
     */
    class LinearOperator :
        public OperatorBase ,
        public VectorBase ,
        public AddArithmeticOperators< LinearOperator >
    {
    public:
      /**
       * @brief Construct linear operator for %FEniCS.
       * @param A shared matrix from %FEniCS
       * @param space operator space (holds information on domain and range)
       */
      LinearOperator(std::shared_ptr<dolfin::GenericMatrix> A, const VectorSpace& space, std::shared_ptr<const dolfin::FunctionSpace> dolfinSpace);

      /**
       * @brief Construct linear operator for %FEniCS.
       * @param A shared matrix from %FEniCS
       * @param space operator space (holds information on domain and range)
       * @param solverCreator function/functor implementing the creation of a linear solver
       */
      LinearOperator(std::shared_ptr<dolfin::GenericMatrix> A, const VectorSpace& space, std::shared_ptr<const dolfin::FunctionSpace> dolfinSpace,
                     std::function<LinearSolver(const LinearOperator&)> solverCreator);

      /**
       * @brief Compute \f$A(x)\f$.
       * @param x operator argument
       */
      ::Spacy::Vector operator()(const ::Spacy::Vector& x) const;

      /**
       * @brief Access solver.
       * @return linear solver representing \f$A^{-1}\f$
       */
      LinearSolver solver() const;

      dolfin::GenericMatrix& impl();

      const dolfin::GenericMatrix& impl() const;

      std::shared_ptr<const dolfin::FunctionSpace> dolfinSpace() const;

      bool symmetric() const;

    private:
      ::Spacy::Vector applyAsOperator(const ::Spacy::Vector& x) const;

      ::Spacy::Vector applyAsDualElement(const ::Spacy::Vector& x) const;

      std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
      bool symmetric_ = false;
      std::function<LinearSolver(const LinearOperator&)> solverCreator_ = [](const LinearOperator& M)
        {
            return LUSolver( M.impl().copy() , M.dolfinSpace() , M.range() , M.domain() , M.symmetric() );
        };
      std::shared_ptr<const dolfin::FunctionSpace> space_;
    };
  }
}

#endif // SPACY_OPERATORS_FENICS_LINEAR_OPERATOR_HH

