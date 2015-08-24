#ifndef ALGORITHM_REAL_SPACE_SOLVER_HH
#define ALGORITHM_REAL_SPACE_SOLVER_HH

#include "../../vector.hh"

namespace Algorithm
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

      /// Compute \f$z^{-1}y\f$.
      ::Algorithm::Vector operator()(const ::Algorithm::Vector& y) const;

    private:
      double z_;
    };
  }
}

#endif // ALGORITHM_REAL_SPACE_SOLVER_HH
