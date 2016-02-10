#ifndef SPACY_ADAPTER_SCALAR_C2_FUNCTIONAL_HH
#define SPACY_ADAPTER_SCALAR_C2_FUNCTIONAL_HH

#include <functional>
#include <memory>

#include "Spacy/Util/Base/functionalBase.hh"
#include "Spacy/Adapter/Scalar/linearOperator.hh"

namespace Spacy
{
  /// @cond
  class LinearOperator;
  class Real;
  class Vector;
  /// @endcond

  /** @addtogroup ScalarGroup @{ */
namespace Scalar
{
    /// @cond
    class LinearOperator;
    /// @endcond

class C2Functional : public Spacy::FunctionalBase 
{
public:
  C2Functional(std::function<double(double)> value,
               std::function<double(double)> derivative, 
               std::function<double(double)> secDerivative) :
               Spacy::FunctionalBase(Space::R) ,
               value_(value), 
               derivative_(derivative), 
               secDerivative_(secDerivative) {};
  // Compute f(x).
  Spacy::Real operator()(const ::Spacy::Vector& x) const
 {
   return value_(cast_ref<Spacy::Real>(x));
 }
 
  // Compute f'(x) as element of X*.
  Spacy::Vector d1(const ::Spacy::Vector& x) const
  {
   return Spacy::Real(derivative_(cast_ref<Spacy::Real>(x)));
  }
  
      // Compute f''(x)dx as element of X*.
  Spacy::Vector d2(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const
  {
	  return Spacy::Real( secDerivative_( cast_ref<Spacy::Real>(x))  * cast_ref<Spacy::Real>(dx) );
  }
  // Access f''(x) as mapping f''(x): X->X*.
  Spacy::Scalar::LinearOperator hessian(const ::Spacy::Vector& x) const
  {
   return Spacy::Scalar::LinearOperator( *operatorSpace_, secDerivative_( cast_ref<Spacy::Real>(x) ));
  }
 
private:
  std::shared_ptr<VectorSpace> operatorSpace_ = nullptr;
  std::function<double(const double&)> value_ ;
  std::function<double(const double&)> derivative_;
  std::function<double(const double&)> secDerivative_;
  
  
};


}
  /** @} */
}

#endif // SPACY_ADAPTER_SCALAR_C2_FUNCTIONAL_HH
