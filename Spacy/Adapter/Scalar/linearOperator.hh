#ifndef SPACY_ADAPTER_SCALAR_LINEAR_OPERATOR_HH
#define SPACY_ADAPTER_SCALAR_LINEAR_OPERATOR_HH

#include <functional>
#include "Spacy/Util/Base/operatorBase.hh"
#include "Spacy/Util/Base/vectorBase.hh"
#include "Spacy/Util/Mixins/impl.hh"

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
      Mixin::Impl<double> ,
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
