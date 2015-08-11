//#ifndef ALGORITHM_ADAPTER_FENICS_LAGRANGEFUNCTIONAL_HH
//#define ALGORITHM_ADAPTER_FENICS_LAGRANGEFUNCTIONAL_HH

//#include <memory>

//#include "Interface/abstractFunctionSpaceElement.hh"
//#include "Interface/Functional/abstractC2Functional.hh"
//#include "Interface/Operator/abstractC1Operator.hh"
//#include "Interface/Operator/abstractC2Operator.hh"
//#include "Interface/constraintOperator.hh"
//#include "Util/mixins.hh"

//#include "pc2Functional.hh"
//#include "../../lagrangeFunctional.hh"

//namespace Algorithm
//{
//  namespace Fenics
//  {
////    class LagrangeFunctional
////        : public Interface::AbstractLagrangeFunctional, public Mixin::StateIndex, public Mixin::ControlIndex, public Mixin::AdjointIndex
////    {
////    public:
////      LagrangeFunctional(std::unique_ptr<Interface::AbstractC2Functional>&& f,
////                         std::unique_ptr<Interface::ConstraintOperator>&& constraint,
////                         std::shared_ptr<Interface::AbstractBanachSpace> domain);

////      double d0(const Interface::AbstractFunctionSpaceElement& x) const final override;

////      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement& x) const final override;

////      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const final override;

////      const Interface::AbstractC2Functional& costFunctional() const;

////      const Interface::ConstraintOperator& constraint() const;

////    private:
////      LagrangeFunctional* cloneImpl() const final override;

////      std::unique_ptr<Interface::Hessian> makeHessian(const Interface::AbstractFunctionSpaceElement& x) const override;

////      std::unique_ptr<Interface::AbstractC2Functional> f_;
////      std::unique_ptr<Interface::ConstraintOperator> constraint_;
////    };

//    class LagrangeFunctional :
//        public Interface::AbstractC2Functional, public Mixin::UniqueImpl<Interface::AbstractC2Functional>,
//        public Mixin::StateIndex, public Mixin::ControlIndex, public Mixin::AdjointIndex
//    {
//    public:
//      explicit LagrangeFunctional(const Interface::AbstractC2Functional& L);

//      explicit LagrangeFunctional(std::unique_ptr<Interface::AbstractC2Functional>&& L);

//      double d0(const Interface::AbstractFunctionSpaceElement& x) const final override;

//      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement& x) const final override;

//      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const final override;

//      void setOrigin(const Interface::AbstractFunctionSpaceElement& x) const;

//    private:
//      LagrangeFunctional* cloneImpl() const final override;

//      std::unique_ptr<Interface::Hessian> makeHessian(const Interface::AbstractFunctionSpaceElement& x) const;
//    };


//    template <class Functional, class... Args>
//    ::Algorithm::C2Functional makeLagrangeFunctional( Args&&... args )
//    {
//      return createFromUniqueImpl< ::Algorithm::C2Functional,Fenics::LagrangeFunctional>( std::make_unique<Functional>(std::forward<Args>(args)...) );
//    }

////    ::Algorithm::LagrangeFunctional makeLagrangeFunctional(std::unique_ptr<Interface::AbstractC2Functional>&& f,
////                                                           std::unique_ptr<Interface::AbstractC2Operator>&& state,
////                                                           std::unique_ptr<Interface::AbstractC1Operator>&& control,
////                                                           std::unique_ptr<Interface::AbstractC2Functional>&& pd2c);
//  }
//}

//#endif // ALGORITHM_ADAPTER_FENICS_LAGRANGEFUNCTIONAL_HH
