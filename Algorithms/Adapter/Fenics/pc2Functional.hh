//#ifndef ALGORITHM_ADAPTER_FENICS_PFunctional_HH
//#define ALGORITHM_ADAPTER_FENICS_PFunctional_HH

//#include <memory>
//#include <vector>

//#include <dolfin.h>

//#include "Interface/abstractFunctional.hh"
//#include "Interface/Operator/linearizedOperator.hh"

//#include "luSolver.hh"
//#include "util.hh"
//#include "vector.hh"

//#include "vectorSpace.hh"
//#include "functional.hh"

//#include "Util/Exceptions/callOfUndefinedFunctionException.hh"
//#include "Util/Mixins/adjointIndex.hh"
//#include "Util/Mixins/controlIndex.hh"
//#include "Util/Mixins/stateIndex.hh"
//#include "Util/cast.hh"
//#include "Util/create.hh"

//#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

//namespace Algorithm
//{
//  namespace Fenics
//  {
//    template <class JacobianForm>
//    class PFunctional : public Interface::AbstractFunctional, public Mixin::StateIndex, public Mixin::ControlIndex , public Mixin::AdjointIndex
//    {
//    public:
//      PFunctional(const JacobianForm& pd2c, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractVectorSpace> space)
//        : Interface::AbstractFunctional( space ),
//          pd2c_( pd2c.function_space(0) , pd2c.function_space(1) ),
//          bcs_( bcs )
//      {
//        copyCoefficients(pd2c,pd2c_);
//      }

//      PFunctional(const JacobianForm& pd2c, const std::vector<const dolfin::DirichletBC*>& bcs, const ::Algorithm::VectorSpace& space)
//        : PFunctional(pd2c,bcs,space.sharedImpl())
//      {}

//      double d0(const Interface::AbstractVector& x) const final override
//      {
//        throw CallOfUndefinedFunctionException("Fenics::C2Operator::operator()");
//      }

//      double d1(const Interface::AbstractVector &x, const Interface::AbstractVector &dx) const final override
//      {
//        throw CallOfUndefinedFunctionException("Fenics::C2Operator::d1()");
//      }

//      std::unique_ptr<Interface::AbstractVector> d2(const Interface::AbstractVector& x,
//                                                                  const Interface::AbstractVector& dx) const final override
//      {
//        const auto& x_ = castTo<ProductSpaceElement>(x);
//        const auto& dx_ = castTo<ProductSpaceElement>(dx);

//        const auto& dy = castTo<Vector>(dx_.variable(stateIndex()));

//        assemble(x_);
//        auto y = clone(x_);
//        toVector( y->variable( controlIndex()) ) *= 0;
//        toVector( y->dualVariable( 0 ) ) *= 0;
//        A_->mult(*dy.impl().vector(), *toVector(y->variable(stateIndex())).impl().vector());

//        return std::unique_ptr<Interface::AbstractVector>( y.release() );
//      }


//    private:
//      void assemble(const ProductSpaceElement& x) const
//      {
//        if( oldX_ != nullptr && oldX_->equals(x) ) return;
//        oldX_ = clone(x);

//        pd2c_.u = castTo<Vector>(x.variable(stateIndex())).impl();
//        pd2c_.p = castTo<Vector>(x.variable(adjointIndex())).impl();
//        A_ = castTo<Vector>(x.variable(stateIndex())).impl().vector()->factory().create_matrix();

//        // Assemble right-hand side
//        dolfin::Assembler assembler;
//        assembler.assemble(*A_, pd2c_);

//        // Apply boundary conditions
//        for(const auto& bc : bcs_)
//          bc->apply( *A_ );
//      }

//      PFunctional* cloneImpl() const
//      {
//        return new PFunctional(pd2c_,bcs_,sharedDomain());
//      }

//      mutable JacobianForm pd2c_;
//      std::vector<const dolfin::DirichletBC*> bcs_;
//      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
//      mutable std::unique_ptr<ProductSpaceElement> oldX_ = nullptr;
//    };


//    template <class JacobianForm>
//    auto makePFunctional(JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const VectorSpace& space)
//    {
//      return createFromUniqueImpl< ::Algorithm::Functional , ::Algorithm::Fenics::PFunctional<JacobianForm> >( J , bcs , space );
//    }
//  }
//}

//#endif // ALGORITHM_ADAPTER_FENICS_PFunctional_HH
