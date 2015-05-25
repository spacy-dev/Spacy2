#ifndef ALGORITHM_CG_HH
#define ALGORITHM_CG_HH

#include "cgImplementation.hh"

namespace Algorithm
{
  /**
   * Standard preconditioned conjugate gradient method.
   */
  using CG = CGBase<CGImplementationType::STANDARD>;

  /**
   * Truncated preconditioned conjugate gradient method for nonconvex problems. Stops iteration if a direction of negative curvature is encountered.
   */
  using TCG = CGBase<CGImplementationType::TRUNCATED>;

  /**
   * Regularized preconditioned conjugate gradient method for nonconvex problems. Denote the used operator by \f$A\f$ and the preconditioner by \f$P\f$.
   * Then if a direction of negative curvature is encountered \f$A\f$ is implicitly replaced by the regularization $\f$A+\thetaP\f$. Then the CG method is
   * restarted for the regularized problem. The necessary quantities are available during the standard cg implementation, thus the costs for computing the
   * regularization are neglishible.
   */
  using RCG = CGBase<CGImplementationType::REGULARIZED>;

  /**
   * Hybrid preconditioned conjugate gradient method for nonconvex problems, mixing the truncated with the regularized conjugate gradient method. If a direction
   * of negative curvature is encounted and the termination criterion indicates sufficient decrease in the used norm the iteration is stopped. Else, denoting
   * the used operator by \f$A\f$ and the preconditioner by \f$P\f, \f$A\f$ is implicitly replaced by the regularization $\f$A+\thetaP\f$. Then the CG method is
   * restarted for the regularized problem. The necessary quantities are available during the standard cg implementation, thus the costs for computing the
   * regularization are neglishible.
   */
  using HCG = CGBase<CGImplementationType::HYBRID>;
}

#endif // ALGORITHM_CG_HH
