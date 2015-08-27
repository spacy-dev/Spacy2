#ifndef ALGORITHM_KASKADE_LAGRANGE_CG_SOLVER_HH
#define ALGORITHM_KASKADE_LAGRANGE_CG_SOLVER_HH

#include <string>

#include "linearSolver.hh"
#include "Algorithm/ConjugateGradients/cgSolver.hh"
#include "Algorithm/ConjugateGradients/triangularStateConstraintPreconditioner.hh"
#include "Util/mixins.hh"

#include "linearOperator.hh"
#include "util.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class> class Functional;

    namespace Lagrange
    {
      template <class FunctionalDefinition, int stateId=0, int controlId=1, int adjointId=2>
      class CGSolver :
          public Mixin::AbsoluteAccuracy ,
          public Mixin::RelativeAccuracy ,
          public Mixin::Eps ,
          public Mixin::IterativeRefinements ,
          public Mixin::MaxSteps ,
          public Mixin::Verbosity
      {
      public:
        explicit CGSolver(std::string solver = "CG")
         : solver_(solver)
        {}

        LinearSolver operator()(const Functional<FunctionalDefinition>& f)
        {
          using KaskadeOperator = typename Functional<FunctionalDefinition>::KaskadeOperator;
          using Matrix = typename  Functional<FunctionalDefinition>::Matrix;
          using VariableSetDescription = typename Functional<FunctionalDefinition>::VariableSetDescription;
          auto matA = f.assembler().template get<Matrix>(f.onlyLowerTriangle(),adjointId,adjointId+1,stateId,stateId+1);
          auto matB = f.assembler().template get<Matrix>(f.onlyLowerTriangle(),adjointId,adjointId+1,controlId,controlId+1);
          auto matM = f.assembler().template get<Matrix>(f.onlyLowerTriangle(),controlId,controlId+1,controlId,controlId+1);
          auto matAt = matA; matAt.transpose();
          auto matBt = matB; matBt.transpose();


          using VYSetDescription = Detail::ExtractDescription_t<VariableSetDescription,stateId>;
          using VUSetDescription = Detail::ExtractDescription_t<VariableSetDescription,controlId>;
          using VPSetDescription = Detail::ExtractDescription_t<VariableSetDescription,adjointId>;
          using Domain0 = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
          using KaskadeOperator2 = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain0,Domain0>;
          auto stateSolver = DirectSolver<KaskadeOperator2,VPSetDescription,VYSetDescription>( KaskadeOperator2(matA), f.spaces(),
                                                                              cast_ref<ProductSpace::VectorCreator>(f.domain().impl()).subSpace(adjointId),
                                                                              cast_ref<ProductSpace::VectorCreator>(f.domain().impl()).subSpace(stateId).dualSpace() );
          auto controlSolver = DirectSolver<KaskadeOperator2,VUSetDescription,VUSetDescription>( KaskadeOperator2(matM), f.spaces(),
                                                                                cast_ref<ProductSpace::VectorCreator>(f.domain().impl()).subSpace(controlId),
                                                                                cast_ref<ProductSpace::VectorCreator>(f.domain().impl()).subSpace(controlId).dualSpace() );
          auto adjointSolver = DirectSolver<KaskadeOperator2,VYSetDescription,VPSetDescription>( KaskadeOperator2(matAt), f.spaces(),
                                                                                cast_ref<ProductSpace::VectorCreator>(f.domain().impl()).subSpace(stateId),
                                                                                cast_ref<ProductSpace::VectorCreator>(f.domain().impl()).subSpace(adjointId).dualSpace() );

          auto B = Kaskade::LinearOperator<KaskadeOperator2,VUSetDescription,VPSetDescription>
              ( KaskadeOperator2(matB) ,
                cast_ref<ProductSpace::VectorCreator>(f.domain().impl()).subSpace(controlId) ,
                cast_ref<ProductSpace::VectorCreator>(f.domain().impl()).subSpace(adjointId).dualSpace() );
          auto Bt = Kaskade::LinearOperator<KaskadeOperator2,VPSetDescription,VUSetDescription>
              ( KaskadeOperator2(matBt) ,
                cast_ref<ProductSpace::VectorCreator>(f.domain().impl()).subSpace(adjointId) ,
                cast_ref<ProductSpace::VectorCreator>(f.domain().impl()).subSpace(controlId).dualSpace() );

          auto P = TriangularStateConstraintPreconditioner
              ( std::move(stateSolver) ,
                std::move(controlSolver) ,
                std::move(adjointSolver) ,
                std::move(B),
                std::move(Bt),
                f.domain().dualSpace() , f.domain() );

          P.setStateIndex(stateId);
          P.setControlIndex(controlId);
          P.setAdjointIndex(adjointId);

          auto A = Kaskade::LinearOperator<KaskadeOperator,VariableSetDescription,VariableSetDescription>( f.A() , f.domain() , f.domain().dualSpace() );

          auto solver = ::Algorithm::CGSolver(A, P, solver_);
          solver.setAbsoluteAccuracy(absoluteAccuracy());
          solver.setRelativeAccuracy(relativeAccuracy());
          solver.setEps(eps());
          solver.setVerbosity(verbose());
          solver.setDetailedVerbosity(verbose_detailed());
          solver.setIterativeRefinements(iterativeRefinements());
          solver.setMaxSteps(maxSteps());

          return std::move(solver);
        }

      private:
        std::string solver_ = "CG";
      };
    }
  }
}

#endif // ALGORITHM_KASKADE_LAGRANGE_CG_SOLVER_HH
