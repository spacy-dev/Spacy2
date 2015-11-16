#include "Test/AlgorithmQuality/EquationSolver/simpleTests.hh"
#include "Test/AlgorithmQuality/EquationSolver/Newton/newtonTestSetup.hh"

#include "Spacy/Spaces/realSpace.hh"

using namespace Spacy;

namespace LocalNewton
{
  inline auto createSolver(const C1Operator&F, double relativeAccuracy, double eps, unsigned maxSteps)
  {
    return createLocalNewton(F,relativeAccuracy,eps,maxSteps);
  }


  GENERATE_SCALAR_TEST(LocalNewton,Linear,2)
  GENERATE_SCALAR_TEST(LocalNewton,Quadratic,7)
}

namespace CovariantNewton
{
  inline auto createSolver(const C1Operator&F, double relativeAccuracy, double eps, unsigned maxSteps)
  {
    return createCovariantNewton(F,relativeAccuracy,eps,maxSteps);
  }


  GENERATE_SCALAR_TEST(CovariantNewton,Linear,2)
  GENERATE_SCALAR_TEST(CovariantNewton,Quadratic,7)
}

namespace ContravariantNewton
{
  inline auto createSolver(const C1Operator&F, double relativeAccuracy, double eps, unsigned maxSteps)
  {
    return createContravariantNewton(F,relativeAccuracy,eps,maxSteps);
  }


  GENERATE_SCALAR_TEST(ContravariantNewton,Linear,1)
  GENERATE_SCALAR_TEST(ContravariantNewton,Quadratic,6)
}

#include "Test/AlgorithmQuality/EquationSolver/undefSimpleTests.hh"
