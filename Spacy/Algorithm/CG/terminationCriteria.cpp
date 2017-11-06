#include "terminationCriteria.hh"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <numeric>

#include <iostream>

namespace Spacy
{
  namespace CG
  {
    namespace Termination
    {
      bool StrakosTichyEnergyError::operator()() const
      {
        auto tol = max( getRelativeAccuracy() , eps() );
        if( getVerbosityLevel() > 1 ) std::cout << "      termination criterion (relative error): " << sqrt(squaredRelativeError()) << "\n      tolerance: " << tol << std::endl;
        if ( scaledGamma2.size() > getMaxSteps() || ( scaledGamma2.size() > lookAhead_ && squaredRelativeError() < tol*tol ) )
          std::cout << "      termination criterion (relative error): " << sqrt(squaredRelativeError()) << "\n      tolerance: " << tol << std::endl;
        return scaledGamma2.size() > getMaxSteps() || ( scaledGamma2.size() > lookAhead_ && squaredRelativeError() < tol*tol );
      }


      void StrakosTichyEnergyError::update(double alpha, double qAq, double, double rPINVr)
      {
        scaledGamma2.push_back( alpha * rPINVr );
        energyNorm2 += alpha * rPINVr;
        stepLength2 = std::abs(qAq);
      }

      bool StrakosTichyEnergyError::vanishingStep() const noexcept
      {
        auto tol = getAbsoluteAccuracy()*getAbsoluteAccuracy();
        if( energyNorm2 > tol) tol = min(tol,eps()*eps()*energyNorm2);
        return stepLength2 < tol;
      }

      void StrakosTichyEnergyError::clear() noexcept
      {
        scaledGamma2.clear();
        energyNorm2 = 0;
      }

      void StrakosTichyEnergyError::setLookAhead(unsigned lookAhead) noexcept
      {
        lookAhead_ = lookAhead;
      }

      bool StrakosTichyEnergyError::minimalDecreaseAchieved() const noexcept
      {
        return squaredRelativeError() < getMinimalAccuracy()*getMinimalAccuracy();
      }

      double StrakosTichyEnergyError::squaredRelativeError() const noexcept
      {
        if( scaledGamma2.size() < lookAhead_ ) return std::numeric_limits<double>::max();
        return std::accumulate(scaledGamma2.end() - lookAhead_, scaledGamma2.end(), 0.) / energyNorm2;
      }

      bool AdaptiveRelativeEnergyError::errorEstimationTerminate() const
      {
        auto tol = std::max(getRelativeAccuracy(), eps());
        tau = 1.;
        unsigned lookAhead_old = lookAhead_;
        lookAhead_ = lookahead_min - 1;
        unsigned firstLookaheadLower = lookahead_min;

        //go from d_min to d_old and check, wether we can decrease the lookahead
        for (unsigned i = lookahead_min; i <= lookAhead_old && scaledGamma2.size() > 2 * lookAhead_; i++)
        {
          lookAhead_++;
          tau = std::accumulate(scaledGamma2.end() - lookAhead_, scaledGamma2.end(), 0.)
              / std::accumulate(scaledGamma2.end() - 2 * lookAhead_, scaledGamma2.end() - lookAhead_, 0.);

          if (tau > desiredtau)
          {
            firstLookaheadLower = i;
          }
        }

        //if this didnt result in a small enough tau, increase the lookahead
        if (firstLookaheadLower == lookAhead_old)
        {
          lookAhead_ = lookAhead_old;
          while (scaledGamma2.size() > 2 * lookAhead_)
          {
            tau = std::accumulate(scaledGamma2.end() - lookAhead_, scaledGamma2.end(), 0.)
                /
                std::accumulate(scaledGamma2.end() - 2 * lookAhead_, scaledGamma2.end() - lookAhead_, 0.);
            if (tau > desiredtau)
            { firstLookaheadLower = lookAhead_; }
            if (firstLookaheadLower < 0.9 * lookAhead_ && firstLookaheadLower + 5 < lookAhead_) break;
            lookAhead_++;
          }
          lookAhead_ = firstLookaheadLower;
        } else
        {
          lookAhead_ = firstLookaheadLower;
        }

        //recompute the tau for the computed lookahead
        tau = std::accumulate(scaledGamma2.end() - lookAhead_, scaledGamma2.end(), 0.)
            / std::accumulate(scaledGamma2.end() - 2 * lookAhead_, scaledGamma2.end() - lookAhead_, 0.);
        tau = tau + (3.365 / 2.23) * getStdDev(); //basically one sided confidence interval of 99,5% for n = 5

        if (scaledGamma2.size() > getMaxSteps() ||
            (scaledGamma2.size() > 2 * lookAhead_ && tau < 0.75 && squaredRelativeError() < tol * tol))
          std::cout << "      adaptive termination criterion (relative error): "
                    << sqrt(squaredRelativeError())
                    << "\n      tolerance: " << tol << std::endl;
        return scaledGamma2.size() > getMaxSteps() || (scaledGamma2.size() > 2 * lookAhead_ && tau < 0.75 &&
                                                       (squaredRelativeError() < tol * tol));
      }

      void AdaptiveRelativeEnergyError::update(double alpha, double qAq, double, double rPINVr, const Vector& cg_iterate_)
      {
        scaledGamma2.push_back(alpha * rPINVr);
        energyNorm2 += alpha * rPINVr;
        stepLength2 = std::abs(qAq);
        cg_iterate = cg_iterate;
      }

      void AdaptiveRelativeEnergyError::update(double alpha, double qAq, double qPq, double rPINVr)
      {
        scaledGamma2.push_back(alpha * rPINVr);
        energyNorm2 += alpha * rPINVr;
        stepLength2 = std::abs(qAq);
      }



      bool AdaptiveRelativeEnergyError::vanishingStep() const noexcept
      {
        return false;
      }

      void AdaptiveRelativeEnergyError::clear() noexcept
      {
        scaledGamma2.clear();
        energyNorm2 = 0;
      }

      void AdaptiveRelativeEnergyError::setLookAhead(unsigned lookAhead) noexcept
      {
        std::cout
            << "We do not allow setting the lookahead in this implementation, lookahead will be chosen adaptively"
            << std::endl;
      }

      bool AdaptiveRelativeEnergyError::minimalDecreaseAchieved() const noexcept
      {
        return squaredRelativeError() < getMinimalAccuracy() * getMinimalAccuracy();
      }

      double AdaptiveRelativeEnergyError::squaredRelativeError() const noexcept
      {
        if (tau < 1) return getSquaredErrorEstimator() / getSquaredSolutionEstimator();
        else return 1.;
      }

      double AdaptiveRelativeEnergyError::getSquaredErrorEstimator() const noexcept
      {
        if (scaledGamma2.size() < lookAhead_ || tau >= 1) return std::numeric_limits<double>::max();
        return (tau / (1. - tau)) * std::accumulate(scaledGamma2.end() - lookAhead_, scaledGamma2.end(), 0.);
      }

      double AdaptiveRelativeEnergyError::getSquaredSolutionEstimator() const noexcept
      {
        return energyNorm2 + getSquaredErrorEstimator();
      }

      double AdaptiveRelativeEnergyError::getStdDev() const noexcept
      {
        double var = 0;
        unsigned no_taus = 5;
        if (scaledGamma2.size() > (2 * lookAhead_ + no_taus))
        {
          double mean = 0;
          std::vector<double> tauvec(no_taus);
          for (unsigned i = 0; i < no_taus; i++)
          {
            tauvec[i] = std::accumulate(scaledGamma2.end() - lookAhead_ - i, scaledGamma2.end() - i, 0.)
                / std::accumulate(scaledGamma2.end() - 2 * lookAhead_ - i,
                                  scaledGamma2.end() - lookAhead_ - i, 0.);
            mean += tauvec[i];
          }
          mean /= (no_taus);
          for (unsigned i = 0; i < no_taus; i++)
          {
            var += ((tauvec[i] - mean) * (tauvec[i] - mean));
          }
          var /= (no_taus - 1); // -1 if sample, else no_taus
        } else var = 1; // this assures, that we do not allow a termination here

        return get(sqrt(var));
      }

      AdaptiveRelativeEnergyError::AdaptiveRelativeEnergyError(StepTermination st)
      {
        st_ = st;
      }

      AdaptiveRelativeEnergyError::AdaptiveRelativeEnergyError() {}

      bool AdaptiveRelativeEnergyError::operator()() const
      {
        bool a = this->errorEstimationTerminate();
        bool b = st_.terminate(cg_iterate, getSquaredErrorEstimator(), getSquaredSolutionEstimator());
        if(a) std::cout<<" terminating due to relative accuracy"<<std::endl;
        if(b) std::cout<<" terminating due to alternative step termination criterion"<<std::endl;
        return a || b;
      }
    }
  }
}
