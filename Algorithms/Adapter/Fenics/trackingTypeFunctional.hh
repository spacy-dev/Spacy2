#ifndef ALGORITHM_ADAPTER_FENICS_TRACKINGTYPEFUNCTIONAL_HH
#define ALGORITHM_ADAPTER_FENICS_TRACKINGTYPEFUNCTIONAL_HH

#include <memory>
#include <utility>

#include "Interface/Functional/abstractC2Functional.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "Util/makeLinearSolver.hh"
#include "Util/Mixins/controlIndex.hh"
#include "Util/Mixins/stateIndex.hh"
#include "Util/castTo.hh"

namespace Algorithm
{
  namespace Fenics
  {
    class TrackingTypeCostFunctional :
        public Interface::AbstractC2Functional , public Mixin::ControlIndex , public Mixin::StateIndex
    {
    public:
      TrackingTypeCostFunctional(std::unique_ptr<Interface::AbstractC2Functional>&& YNormFunctional,
                                 std::unique_ptr<Interface::AbstractC2Functional>&& UNormFunctional,
                                 std::shared_ptr<Interface::AbstractBanachSpace> domain)
        : Interface::AbstractC2Functional(domain),
          YNormFunctional_(std::move(YNormFunctional)),
          UNormFunctional_(std::move(UNormFunctional))
      {}

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        const auto& x_  = castTo<ProductSpaceElement>(x);
        auto result_ = clone(x_);
        auto& result = castTo<ProductSpaceElement>(*result_);

        result.variable(stateIndex()) = *YNormFunctional_->d1( x_.variable(stateIndex()));
        result.variable(controlIndex()) = *UNormFunctional_->d1( x_.variable(controlIndex()));
        return std::move(result_);
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const final override
      {
        const auto& x_  = castTo<ProductSpaceElement>(x);
        const auto& dx_ = castTo<ProductSpaceElement>(dx);

        auto result_ = clone(x_);
        auto& result = castTo<ProductSpaceElement>(*result_);

        result.variable(stateIndex()) = *YNormFunctional_->d2( x_.variable(stateIndex()) , dx_.variable(stateIndex())   );
        result.variable(controlIndex()) = *UNormFunctional_->d2( x_.variable(controlIndex()) , dx_.variable(controlIndex()) );
        return std::move(result_);
      }


    private:
      TrackingTypeCostFunctional* cloneImpl() const final override
      {
        auto r = new TrackingTypeCostFunctional(clone(YNormFunctional_),clone(UNormFunctional_),sharedDomain());
        return r;
      }

      std::unique_ptr<Interface::Hessian> makeHessian(const Interface::AbstractFunctionSpaceElement& x) const override
      {
        return UNormFunctional_->hessian(castTo<ProductSpaceElement>(x).variable(controlIndex()));
      }

      double d0(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        const auto& x_ = castTo<ProductSpaceElement>(x);
        return (*YNormFunctional_)( x_.variable(stateIndex()) )
             + (*UNormFunctional_)( x_.variable(controlIndex()) );
      }


      std::unique_ptr<Interface::AbstractC2Functional> YNormFunctional_, UNormFunctional_;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_TRACKINGTYPEFUNCTIONAL_HH

