#ifndef SPACY_ADAPTER_KASKADE_DIRECT_SOLVER_HH
#define SPACY_ADAPTER_KASKADE_DIRECT_SOLVER_HH

#include "linalg/direct.hh"

#include "Spacy/vector.hh"
#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Base/operatorBase.hh"
#include "util.hh"
#include "vectorSpace.hh"

namespace Spacy
{
  namespace Kaskade
  {
    /**
     * @ingroup KaskadeGroup
     * @brief Direct solver interface for %Kaskade 7.
     */
    template <class KaskadeOperator, class AnsatzVariableDescription, class TestVariableDescription>
    class DirectSolver :
        public OperatorBase
    {
      using Spaces = typename AnsatzVariableDescription::Spaces;
      using Domain = typename AnsatzVariableDescription::template CoefficientVectorRepresentation<>::type;
      using Range = typename TestVariableDescription::template CoefficientVectorRepresentation<>::type;
    public:
      DirectSolver() = delete;
      /**
       * @brief Constructor.
       * @param A %Kaskade operator (i.e., %AssembledGalerkinOperator or %MatrixRepresentedOperator)
       * @param spaces boost fusion forward sequence of space pointers required to initialize temporary storage
       * @param domain domain space of the solver
       * @param range range space of the solver
       * @param directSolver solver type (DirectType::MUMPS (default), DirectType::UMFPACK, DirectType::UMFPACK3264 or DirectType::SUPERLU)
       * @param property matrix property (MatrixProperties::GENERAL (default) or MatrixProperties::SYMMETRIC)
       */
      DirectSolver(KaskadeOperator A, const VectorSpace& domain, const VectorSpace& range,
                   DirectType directSolver = DirectType::UMFPACK, MatrixProperties property = MatrixProperties::GENERAL )
        : OperatorBase(domain,range),
          A_(std::move(A)),
          spaces_( extractSpaces<AnsatzVariableDescription>(domain) ),
          directSolver_(/*directSolver*/DirectType::UMFPACK),
          property_(property)
      {}

      /// Compute \f$A^{-1}x\f$.
      ::Spacy::Vector operator()(const ::Spacy::Vector& x) const
      {
        if( solver_ == nullptr) solver_ = std::make_shared< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<Domain,Range> > >
            ( ::Kaskade::directInverseOperator(A_, directSolver_, property_) );

        Range y_(TestVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        Domain x_(AnsatzVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        copyToCoefficientVector<AnsatzVariableDescription>(x,x_);

        solver_->apply( x_ , y_ );

        auto y = range().zeroVector();
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

    /**
     * @brief Convenient generation of direct solver for %Kaskade 7.
     * @param A %Kaskade operator (i.e., AssembledGalerkinOperator or MatrixRepresentedOperator)
     * @param spaces boost fusion forward sequence of space pointers required to initialize temporary storage
     * @param domain domain space of the solver
     * @param range range space of the solver
     * @param directSolver solver type (DirectType::MUMPS (default), DirectType::UMFPACK, DirectType::UMFPACK3264 or DirectType::SUPERLU)
     * @param property matrix property (MatrixProperties::GENERAL (default) or MatrixProperties::SYMMETRIC)
     * @return DirectSolver<KaskadeOperator,AnsatzVariableSetDescription,TestVariableSetDescription>( A , spaces , domain , range , directSolver , property )
     */
    template <class AnsatzVariableSetDescription, class TestVariableSetDescription, class KaskadeOperator>
    auto makeDirectSolver(KaskadeOperator A, const VectorSpace& domain, const VectorSpace& range,
                          DirectType directSolver = DirectType::MUMPS, MatrixProperties property = MatrixProperties::GENERAL )
    {
      return DirectSolver<KaskadeOperator,AnsatzVariableSetDescription,TestVariableSetDescription>
          ( std::move(A) , domain , range , directSolver , property );
    }

  }
}

#endif // SPACY_ADAPTER_KASKADE_DIRECT_SOLVER_HH
