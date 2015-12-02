#ifndef SPACY_ADAPTER_SCALAR_C1_OPERATOR_HH
#define SPACY_ADAPTER_SCALAR_C1_OPERATOR_HH

#include <functional>
#include <memory>

#include "Spacy/Util/Base/operatorBase.hh"

namespace Spacy
{
  /// @cond
  class LinearOperator;
  class Real;
  class Vector;
  /// @endcond

  /** @addtogroup ScalarGroup
   * @{
   */
  namespace Scalar
  {
    /// @cond
    class LinearOperator;
    /// @endcond

    class C1Operator : public Spacy::OperatorBase
    {
    public:
      C1Operator(std::function<double(double)> value,
                 std::function<double(double)> derivative);

     Spacy::Vector operator()(const ::Spacy::Vector& x) const;

     Spacy::Vector d1(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const;

     LinearOperator linearization(const ::Spacy::Vector& x) const;

    private:
      std::function<double(double)> value_ = [](double x) { return x*x-1;};
      std::function<double(double)> derivative_ = [](double x) { return 2*x; };
      std::shared_ptr<VectorSpace> operatorSpace_ = nullptr;
    };
  }
  /** @} */
}

#endif // SPACY_ADAPTER_SCALAR_C1_OPERATOR_HH
