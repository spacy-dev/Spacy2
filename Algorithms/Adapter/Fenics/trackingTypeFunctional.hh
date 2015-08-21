//#ifndef ALGORITHM_ADAPTER_FENICS_TRACKINGTYPEFUNCTIONAL_HH
//#define ALGORITHM_ADAPTER_FENICS_TRACKINGTYPEFUNCTIONAL_HH

//#include <memory>
//#include <utility>

//#include "Interface/abstractFunctional.hh"
//#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
//#include "Util/Mixins/controlIndex.hh"
//#include "Util/Mixins/stateIndex.hh"
//#include "Util/castTo.hh"

//namespace Algorithm
//{
//  namespace Fenics
//  {
//    class TrackingTypeCostFunctional :
//        public Interface::AbstractFunctional , public Mixin::ControlIndex , public Mixin::StateIndex
//    {
//    public:
//      TrackingTypeCostFunctional(std::unique_ptr<Interface::AbstractFunctional>&& YNormFunctional,
//                                 std::unique_ptr<Interface::AbstractFunctional>&& UNormFunctional,
//                                 std::shared_ptr<Interface::AbstractVectorSpace> domain)
//        : Interface::AbstractFunctional(domain),
//          YNormFunctional_(std::move(YNormFunctional)),
//          UNormFunctional_(std::move(UNormFunctional))
//      {}

//      std::unique_ptr<Interface::AbstractVector> d1(const Interface::AbstractVector& x) const final override
//      {
//        const auto& x_  = castTo<ProductSpaceElement>(x);
//        auto result_ = clone(x_);
//        auto& result = castTo<ProductSpaceElement>(*result_);

//        result.variable(stateIndex()) = *YNormFunctional_->d1( x_.variable(stateIndex()));
//        result.variable(controlIndex()) = *UNormFunctional_->d1( x_.variable(controlIndex()));
//        return std::move(result_);
//      }

//      std::unique_ptr<Interface::AbstractVector> d2(const Interface::AbstractVector& x, const Interface::AbstractVector& dx) const final override
//      {
//        const auto& x_  = castTo<ProductSpaceElement>(x);
//        const auto& dx_ = castTo<ProductSpaceElement>(dx);

//        auto result_ = clone(x_);
//        auto& result = castTo<ProductSpaceElement>(*result_);

//        result.variable(stateIndex()) = *YNormFunctional_->d2( x_.variable(stateIndex()) , dx_.variable(stateIndex())   );
//        result.variable(controlIndex()) = *UNormFunctional_->d2( x_.variable(controlIndex()) , dx_.variable(controlIndex()) );
//        return std::move(result_);
//      }


//    private:
//      TrackingTypeCostFunctional* cloneImpl() const final override
//      {
//        auto r = new TrackingTypeCostFunctional(clone(YNormFunctional_),clone(UNormFunctional_),sharedDomain());
//        return r;
//      }

//      std::unique_ptr<Interface::Hessian> makeHessian(const Interface::AbstractVector& x) const override
//      {
//        return UNormFunctional_->hessian(castTo<ProductSpaceElement>(x).variable(controlIndex()));
//      }

//      double d0(const Interface::AbstractVector& x) const final override
//      {
//        const auto& x_ = castTo<ProductSpaceElement>(x);
//        return (*YNormFunctional_)( x_.variable(stateIndex()) )
//             + (*UNormFunctional_)( x_.variable(controlIndex()) );
//      }


//      std::unique_ptr<Interface::AbstractFunctional> YNormFunctional_, UNormFunctional_;
//    };
//  }
//}

//#endif // ALGORITHM_ADAPTER_FENICS_TRACKINGTYPEFUNCTIONAL_HH

