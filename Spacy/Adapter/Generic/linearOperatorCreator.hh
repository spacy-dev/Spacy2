// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_GENERIC_LINEAR_OPERATOR_CREATOR_HH
#define SPACY_ADAPTER_GENERIC_LINEAR_OPERATOR_CREATOR_HH

#include <functional>
#include "Spacy/vectorSpace.hh"
#include "Spacy/Util/Base/operatorBase.hh"

namespace Spacy
{
  namespace Generic
  {
    template <class LinearOperator>
    class LinearOperatorCreator : public OperatorBase
    {
    public:
      LinearOperatorCreator(std::function<LinearOperator(const VectorSpace*)> creator,
                            const VectorSpace& domain, const VectorSpace& range)
        : OperatorBase(domain,range),
          creator_(std::move(creator))
      {}

      LinearOperator operator()(const VectorSpace* space) const
      {
        return creator_(space);
      }

    private:
      std::function<LinearOperator(const VectorSpace*)> creator_;
    };
  }
}

#endif // SPACY_ADAPTER_GENERIC_LINEAR_OPERATOR_CREATOR_HH
