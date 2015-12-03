// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "terminationCriterion.hh"

#include "Spacy/Spaces/RealSpace/real.hh"

#include <cassert>

namespace Spacy
{
  namespace CG
  {
    bool TerminationCriterion::operator()() const
    {
      assert(base_);
      return base_->operator ()();
    }

    void TerminationCriterion::clear()
    {
      assert(base_);
      base_->clear();
    }

    void TerminationCriterion::update(Real alpha, Real qAq, Real qPq, Real rPINVr)
    {
      assert(base_);
      base_->update(toDouble(alpha),toDouble(qAq),toDouble(qPq),toDouble(rPINVr));
    }

    bool TerminationCriterion::vanishingStep() const
    {
      assert(base_);
      return base_->vanishingStep();
    }

    bool TerminationCriterion::minimalDecreaseAchieved() const
    {
      assert(base_);
      return base_->minimalDecreaseAchieved();
    }

    void TerminationCriterion::setEps(double eps)
    {
      assert(base_);
      base_->setEps(eps);
    }

    void TerminationCriterion::setAbsoluteAccuracy(double accuracy)
    {
      assert(base_);
      base_->setAbsoluteAccuracy(accuracy);
    }

    void TerminationCriterion::setMinimalAccuray(double accuracy)
    {
      assert(base_);
      base_->setMinimalAccuray(accuracy);
    }

    void TerminationCriterion::setRelativeAccuracy(double accuracy)
    {
      assert(base_);
      base_->setRelativeAccuracy(accuracy);
    }

    TerminationCriterion::operator bool() const
    {
      return base_;
    }
  }
}
