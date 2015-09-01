#ifndef ALGORITHM_REAL_SPACE_SOLVER_HH
#define ALGORITHM_REAL_SPACE_SOLVER_HH

#include "VSA/vector.hh"

namespace VSA
{
  namespace Real
  {
    /**
     * @ingroup RealGroup
     * @brief A solver for scalar problems.
     */
    class Solver
    {
    public:
      /**
       * @brief Constructor.
       * @param z to be inverted
       */
      Solver(double z);

      /**
       * @brief Apply solver.
       * @param y right hand side
       * @return \f$z^{-1}y\f$.
       */
      ::VSA::Vector operator()(const ::VSA::Vector& y) const;

    private:
      double z_;
    };
  }
}

#endif // ALGORITHM_REAL_SPACE_SOLVER_HH
