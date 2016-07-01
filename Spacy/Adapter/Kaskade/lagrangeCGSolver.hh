#ifndef SPACY_KASKADE_LAGRANGE_CG_SOLVER_HH
#define SPACY_KASKADE_LAGRANGE_CG_SOLVER_HH

#include <string>

#include "Spacy/linearSolver.hh"
#include "Spacy/Algorithm/CG/linearSolver.hh"
#include "Spacy/Algorithm/CG/triangularStateConstraintPreconditioner.hh"
#include "Spacy/Util/mixins.hh"

#include "linearOperator.hh"
#include "util.hh"

namespace Spacy
{
  namespace Kaskade
  {
    template <class> class C2Functional;

    namespace Lagrange
    {
      /**
       * @ingroup KaskadeGroup
       * @brief Creator for conjugate gradient solver for optimal control problems with a CG::TriangularStateConstraintPreconditioner.
       *
       * Solve a system of the form
       * \f[ A = \left( \begin{array}{ccc} L_{yy} & L_{yu}  & A^T \\ L_{uy} & L_{uu} & B^T \\ A & B &  \end{array} \right)\f],
       * with preconditioner
       * \f[ P = \left( \begin{array}{ccc}  &  & A^{-T} \\  & L_{uu}^{-1} & -B^T \\ A^{-1} & -B &  \end{array} \right)\f],
       * where \f$A\f$ is the state operator, \f$B\f$ the control operator and \f$L_{uu}\f$ is the second derivative of the Lagrange functional
       * with respect to the control variable \f$u\f$. The state variable is denoted by \f$y\f$ and the adjoint variable by\f$p\f$.
       *
       * @tparam FunctionalDefinition definition of the Lagrange functional from Kaskade
       * @tparam stateId index of the state variable
       * @tparam controlId index of the control variable
       * @tparam adjointId index of the adjoint variable
       * @see CG::Solver
       */
      template <class FunctionalDefinition, int stateId=0, int controlId=1, int adjointId=2>
      class CGCreator :
          public Mixin::AbsoluteAccuracy ,
          public Mixin::RelativeAccuracy ,
          public Mixin::Eps ,
          public Mixin::IterativeRefinements ,
          public Mixin::MaxSteps ,
          public Mixin::Verbosity
      {
        using KaskadeOperator = typename C2Functional<FunctionalDefinition>::KaskadeOperator;
        using Matrix = typename  C2Functional<FunctionalDefinition>::Matrix;
        using VariableSetDescription = typename C2Functional<FunctionalDefinition>::VariableSetDescription;
        template <class X, class Y>
        using M = ::Kaskade::MatrixRepresentedOperator<Matrix,X,Y>;
      public:
        /**
         * @brief Constructor.
         * @param solver solver type ("CG", "RCG", "TCG", "RTCG").
         * @see CG::Solver
         */
        explicit CGCreator(std::string solver = "CG")
         : solver_(solver)
        {}

        /**
         * @brief Create conjugate gradient solver with triangular state constraint preconditioner for \f$L''\f$.
         * @param L Lagrange functional
         * @return solver representing \f$L''(x)^{-1}\f$, where \f$x\f$ is the current iterate
         */
        LinearSolver operator()(const C2Functional<FunctionalDefinition>& L) const
        {
          auto matA = L.assembler().template get<Matrix>(L.onlyLowerTriangle(),adjointId,adjointId+1,stateId,stateId+1);
          auto matB = L.assembler().template get<Matrix>(L.onlyLowerTriangle(),adjointId,adjointId+1,controlId,controlId+1);
          auto matM = L.assembler().template get<Matrix>(L.onlyLowerTriangle(),controlId,controlId+1,controlId,controlId+1);
          auto matAt = matA; matAt.transpose();
          auto matBt = matB; matBt.transpose();

          using VYSetDescription = Detail::ExtractDescription_t<VariableSetDescription,stateId>;
          using VUSetDescription = Detail::ExtractDescription_t<VariableSetDescription,controlId>;
          using VPSetDescription = Detail::ExtractDescription_t<VariableSetDescription,adjointId>;
          using DomainY = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
          using DomainU = typename VUSetDescription::template CoefficientVectorRepresentation<>::type;
          using DomainP = typename VPSetDescription::template CoefficientVectorRepresentation<>::type;

          auto stateSolver   = DirectSolver<M<DomainY,DomainP>,VPSetDescription,VYSetDescription>
              ( M<DomainY,DomainP>(matA), L.spaces(),
                cast_ref<ProductSpace::VectorCreator>(L.domain()).subSpace(adjointId),
                cast_ref<ProductSpace::VectorCreator>(L.domain()).subSpace(stateId).dualSpace() );
          auto controlSolver = DirectSolver<M<DomainU,DomainU>,VUSetDescription,VUSetDescription>
              ( M<DomainU,DomainU>(matM), L.spaces(),
                cast_ref<ProductSpace::VectorCreator>(L.domain()).subSpace(controlId),
                cast_ref<ProductSpace::VectorCreator>(L.domain()).subSpace(controlId).dualSpace() );
          auto adjointSolver = DirectSolver<M<DomainP,DomainY>,VYSetDescription,VPSetDescription>
              ( M<DomainP,DomainY>(matAt), L.spaces(),
                cast_ref<ProductSpace::VectorCreator>(L.domain()).subSpace(stateId),
                cast_ref<ProductSpace::VectorCreator>(L.domain()).subSpace(adjointId).dualSpace() );

          auto B = Kaskade::LinearOperator<VUSetDescription,VPSetDescription>
              ( M<DomainU,DomainP>(matB) ,
                cast_ref<ProductSpace::VectorCreator>(L.domain()).subSpace(controlId) ,
                cast_ref<ProductSpace::VectorCreator>(L.domain()).subSpace(adjointId).dualSpace() );
          auto Bt = Kaskade::LinearOperator<VPSetDescription,VUSetDescription>
              ( M<DomainP,DomainU>(matBt) ,
                cast_ref<ProductSpace::VectorCreator>(L.domain()).subSpace(adjointId) ,
                cast_ref<ProductSpace::VectorCreator>(L.domain()).subSpace(controlId).dualSpace() );

          auto P = CG::TriangularStateConstraintPreconditioner
              ( std::move(stateSolver) ,
                std::move(controlSolver) ,
                std::move(adjointSolver) ,
                std::move(B),
                std::move(Bt),
                L.domain().dualSpace() , L.domain() );

          P.setStateIndex(stateId);
          P.setControlIndex(controlId);
          P.setAdjointIndex(adjointId);

          auto A = Kaskade::LinearOperator<VariableSetDescription,VariableSetDescription>( L.A() , L.domain() , L.domain().dualSpace() );

          auto solver = ::Spacy::CG::LinearSolver(A, P, solver_);
          //~ solver.setAbsoluteAccuracy(absoluteAccuracy());
          //~ solver.setRelativeAccuracy(relativeAccuracy());
          //~ solver.setEps(eps());
          //~ solver.setVerbosityLevel(verbosityLevel());
          //~ solver.setIterativeRefinements(iterativeRefinements());
          //~ solver.setMaxSteps(maxSteps());

          return std::move(solver);
        }

      private:
        std::string solver_ = "CG";
      };
    }
  }
}

#endif // SPACY_KASKADE_LAGRANGE_CG_SOLVER_HH
