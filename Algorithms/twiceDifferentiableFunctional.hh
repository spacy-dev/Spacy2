#ifndef ALGORITHM_TWICE_DIFFERENTIABLEFUNCTIONAL_HH
#define ALGORITHM_TWICE_DIFFERENTIABLFUNCTIONAL_HH

#include "differentiableFunctional.hh"

namespace Algorithm
{
  class AbstractTwiceDifferentiableFunctional;

  class TwiceDifferentiableFunctional : public DifferentiableFunctional
  {
  public:
    TwiceDifferentiableFunctional(std::shared_ptr<AbstractTwiceDifferentiableFunctional> impl);

//    FunctionSpaceElement d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy);

    double d2(const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;

    AbstractTwiceDifferentiableFunctional& impl();

    const AbstractTwiceDifferentiableFunctional& impl() const;

  private:
    std::shared_ptr<AbstractTwiceDifferentiableFunctional> impl_;
  };
}
#endif // ALGORITHM_TWICE_DIFFERENTIABLFUNCTIONAL_HH
