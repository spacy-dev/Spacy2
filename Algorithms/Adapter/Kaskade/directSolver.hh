#ifndef ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH
#define ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH

#include "linalg/direct.hh"

#include "vector.hh"
#include "Util/cast.hh"
#include "Util/Base/operatorBase.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    /**
     * @ingroup KaskadeGroup
     * @brief Direct solver for interface for Kaskade 7.
     *
     * Performs lazy construction of the solver.
     */
    template <class KaskadeOperator, class AnsatzVariableDescription, class TestVariableDescription>
    class DirectSolver :
        public OperatorBase
    {
      using Spaces = typename AnsatzVariableDescription::Spaces;
      using Domain = typename AnsatzVariableDescription::template CoefficientVectorRepresentation<>::type;
      using Range = typename TestVariableDescription::template CoefficientVectorRepresentation<>::type;
    public:
      /**
       * @brief Constructor.
       * @param A Kaskade operator (i.e., AssembledGalerkinOperator or MatrixRepresentedOperator)
       * @param spaces boost fusion forward sequence of space pointers required to initialize temporary storage
       * @param domain domain space of the solver
       * @param range range space of the solver
       * @param directSolver solver type (DirectType::MUMPS (default), DirectType::UMFPACK, DirectType::UMFPACK3264 or DirectType::SUPERLU)
       * @param property matrix property (MatrixProperties::GENERAL (default) or MatrixProperties::SYMMETRIC)
       */
      DirectSolver(KaskadeOperator A, const Spaces& spaces,
                   const VectorSpace& domain , const VectorSpace& range,
                   DirectType directSolver = DirectType::MUMPS, MatrixProperties property = MatrixProperties::GENERAL )
        : OperatorBase(domain,range),
          A_(std::move(A)),
          spaces_(spaces),
          directSolver_(directSolver),
          property_(property)
      {}

      /// Compute \f$A^{-1}x\f$.
      ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const
      {
        if( solver_ == nullptr) solver_ = std::make_shared< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<Domain,Range> > >
            ( ::Kaskade::directInverseOperator(A_, directSolver_, property_) );
        Range y_(TestVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        Domain x_(AnsatzVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        copyToCoefficientVector<AnsatzVariableDescription>(x,x_);

        solver_->apply( x_ , y_ );

        auto y = range().element();
        copyFromCoefficientVector<TestVariableDescription>(y_,y);

        return y;
      }

    private:
      KaskadeOperator A_;
      Spaces spaces_;
      DirectType directSolver_ = DirectType::MUMPS;
      MatrixProperties property_ = MatrixProperties::GENERAL;
      mutable std::shared_ptr< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<Domain,Range> > > solver_ = nullptr;
    };
  }
}

#endif // ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH
