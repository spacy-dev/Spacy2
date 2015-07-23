#include "cgImplementation.hh"

#include <algorithm>
#include <iostream>


namespace Algorithm
{
  namespace CG_Detail
  {
    // class NoRegularization
    NoRegularization::NoRegularization(double eps, bool verbose) noexcept
      : Mixin::Eps(eps),
        Mixin::Verbosity(verbose)
    {}

    void NoRegularization::initializeRegularization() const noexcept {}
    void NoRegularization::regularize(double&, double) const noexcept {}
    void NoRegularization::updateRegularization(double,double) const noexcept {}


    // class Regularization
    Regularization::Regularization(double eps, bool verbose) noexcept
      : Mixin::Eps(eps), Mixin::Verbosity(verbose)
    {}

    void Regularization::initializeRegularization() const noexcept
    {
      theta = 0;
    }

    void Regularization::regularize(double& qAq, double qPq) const noexcept
    {
      qAq += theta*qPq;
    }

    void Regularization::updateRegularization(double qAq, double qPq) const
    {
      double oldTheta = theta > 0 ? theta : eps();
      theta += (1-qAq)/std::abs(qPq);
      if( verbose() ) std::cout << "Computed regularization parameter: " << theta << std::endl;
      theta = std::min(std::max(minIncrease*oldTheta,theta),maxIncrease*oldTheta);
      if( verbose() ) std::cout << "Updating regularization parameter from " << oldTheta << " to " << theta << std::endl;
    }
  }
}
