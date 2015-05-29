#ifndef ALGORITHM_ALGORITHM_PARAMETERS_HH
#define ALGORITHM_ALGORITHM_PARAMETERS_HH

namespace Algorithm
{
  struct Parameter
  {
    double absoluteAccuracy = 1e-12;
    double relativeAccuracy = 1e-6;
    unsigned maxSteps = 100;
  };
}

#endif // ALGORITHM_ALGORITHM_PARAMETERS_HH
