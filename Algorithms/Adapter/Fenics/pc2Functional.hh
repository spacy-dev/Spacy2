#ifndef ALGORITHM_ADAPTER_FENICS_PC2FUNCTIONAL_HH
#define ALGORITHM_ADAPTER_FENICS_PC2FUNCTIONAL_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/Functional/abstractC2Functional.hh"
#include "Interface/Operator/linearizedOperator.hh"

#include "luSolver.hh"
#include "util.hh"
#include "vector.hh"

#include "banachSpace.hh"
#include "../../c2Functional.hh"

#include "Util/Exceptions/callOfUndefinedFunctionException.hh"
#include "Util/Mixins/adjointIndex.hh"
#include "Util/Mixins/controlIndex.hh"
#include "Util/Mixins/stateIndex.hh"
#include "Util/create.hh"

#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class JacobianForm>
    class PC2Functional : public Interface::AbstractC2Functional, public Mixin::StateIndex, public Mixin::ControlIndex , public Mixin::AdjointIndex
    {
    public:
      PC2Functional(const JacobianForm& pd2c, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractBanachSpace> space)
        : Interface::AbstractC2Functional( space ),
          pd2c_( pd2c.function_space(0) , pd2c.function_space(1) ),
          bcs_( bcs )
      {
        copyCoefficients(pd2c,pd2c_);
      }

      PC2Functional(const JacobianForm& pd2c, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
        : PC2Functional(pd2c,bcs,space.sharedImpl())
      {}

      double d0(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        throw CallOfUndefinedFunctionException("Fenics::C2Operator::operator()");
      }

      double d1(const Interface::AbstractFunctionSpaceElement &x, const Interface::AbstractFunctionSpaceElement &dx) const final override
      {
        throw CallOfUndefinedFunctionException("Fenics::C2Operator::d1()");
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement& x,
                                                                  const Interface::AbstractFunctionSpaceElement& dx) const final override
      {
        const auto& x_ = toProductSpaceElement(x);
        const auto& dx_ = toProductSpaceElement(dx);

        const auto& dy = toVector(dx_.variable(stateIndex()));

        assemble(x_);
        auto y = clone(x_);
        toVector( y->variable( controlIndex()) ) *= 0;
        toVector( y->dualVariable( 0 ) ) *= 0;
        A_->mult(*dy.impl().vector(), *toVector(y->variable(stateIndex())).impl().vector());

        return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( y.release() );
      }


    private:
      void assemble(const ProductSpaceElement& x) const
      {
        if( oldX_ != nullptr && oldX_->equals(x) ) return;
        oldX_ = clone(x);

        pd2c_.u = toVector(x.variable(stateIndex())).impl();
        pd2c_.p = toVector(x.variable(adjointIndex())).impl();
        A_ = toVector(x.variable(stateIndex())).impl().vector()->factory().create_matrix();

        // Assemble right-hand side
        dolfin::Assembler assembler;
        assembler.assemble(*A_, pd2c_);

        // Apply boundary conditions
        for(const auto& bc : bcs_)
          bc->apply( *A_ );
      }

      PC2Functional* cloneImpl() const
      {
        return new PC2Functional(pd2c_,bcs_,sharedDomain());
      }

      mutable JacobianForm pd2c_;
      std::vector<const dolfin::DirichletBC*> bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
      mutable std::unique_ptr<ProductSpaceElement> oldX_ = nullptr;
    };


    template <class JacobianForm>
    auto makePC2Functional(JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
    {
      return create_uniqueImpl< ::Algorithm::C2Functional , ::Algorithm::Fenics::PC2Functional<JacobianForm> >( J , bcs , space );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_PC2FUNCTIONAL_HH
