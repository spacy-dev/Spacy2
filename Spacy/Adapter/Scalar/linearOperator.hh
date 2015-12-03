// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_SCALAR_LINEAR_OPERATOR_HH
#define SPACY_ADAPTER_SCALAR_LINEAR_OPERATOR_HH

#include <functional>
#include "Spacy/Util/Base/addArithmeticOperators.hh"
#include "Spacy/Util/Base/operatorBase.hh"
#include "Spacy/Util/Base/vectorBase.hh"
#include "Spacy/Util/Mixins/get.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  /** @addtogroup ScalarGroup
   * @{
   */
  namespace Scalar
  {
    struct LinearOperator :
      VectorBase ,
      OperatorBase ,
      Mixin::Get<double> ,
      AddArithmeticOperators<LinearOperator>
    {
      LinearOperator(const VectorSpace& space, double value);

      ::Spacy::Vector operator()(const ::Spacy::Vector& dx) const;

      LinearOperator operator-() const;

      std::function< ::Spacy::Vector(const ::Spacy::Vector&) > solver() const;
    };
  }
  /** @} */
}

#endif // SPACY_ADAPTER_SCALAR_LINEAR_OPERATOR_HH
