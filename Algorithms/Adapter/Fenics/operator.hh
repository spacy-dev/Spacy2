#ifndef ALGORITHM_ADAPTER_FENICS_OPERATOR_HH
#define ALGORITHM_ADAPTER_FENICS_OPERATOR_HH

#include <memory>

#include <dolfin.h>

#include "linearSolver.hh"
#include "../../vectorSpace.hh"
#include "Util/Mixins/disableAssembly.hh"
#include "Util/Base/operatorBase.hh"
#include "linearizedOperator.hh"

#include "luSolver.hh"
#include "util.hh"
#include "assignXIfPresent.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class ResidualForm, class JacobianForm>
    class Operator : public OperatorBase , public Mixin::DisableAssembly
    {
    public:
      Operator(const ResidualForm& F, const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
               ::Algorithm::VectorSpace* domain, ::Algorithm::VectorSpace* range)
        : OperatorBase( domain , range ),
          F_( F.function_space(0) ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs ),
          dummy_( J.function_space(0) )
      {
        copyCoefficients(F,F_);
        copyCoefficients(J,J_);
      }

      Operator(const ResidualForm& F, const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
               std::shared_ptr<dolfin::GenericMatrix> A,
               ::Algorithm::VectorSpace* domain, ::Algorithm::VectorSpace* range)
        : OperatorBase( domain , range ),
          Mixin::DisableAssembly(true),
          F_( F.function_space(0) ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs ),
          A_(A->copy()),
          dummy_( J.function_space(0) )
      {
        copyCoefficients(F,F_);
        copyCoefficients(J,J_);
      }

      Operator(Operator&& other)
        : OperatorBase( other.domain_ptr() , other.range_ptr() ) ,
          Mixin::DisableAssembly(other.assemblyIsDisabled()) ,
          F_( other.F_.function_space(0) ) ,
          J_( other.J_.function_space(0) , other.J_.function_space(1) ) ,
          bcs_( other.bcs_ ) ,
          A_( std::move(other.A_) ) ,
          b_( std::move(other.b_) ) ,
          dummy_( other.J_.function_space(0) )
      {
        copyCoefficients(other.F_,F_);
        copyCoefficients(other.J_,J_);
      }

      Operator(const Operator& other)
        : OperatorBase( other.domain_ptr() , other.range_ptr() ) ,
          Mixin::DisableAssembly(other.assemblyIsDisabled()) ,
          F_( other.F_.function_space(0) ) ,
          J_( other.J_.function_space(0) , other.J_.function_space(1) ) ,
          bcs_( other.bcs_ ) ,
          A_( (other.A_ != nullptr) ? other.A_->copy() : nullptr ) ,
          b_( (other.b_ != nullptr) ? other.b_->copy() : nullptr ) ,
          dummy_( other.J_.function_space(0) )
      {
        copyCoefficients(other.F_,F_);
        copyCoefficients(other.J_,J_);
      }

      Operator(const ResidualForm& F, const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
               ::Algorithm::VectorSpace& domain, ::Algorithm::VectorSpace& range)
        : Operator(F,J,bcs,&domain,&range)
      {}

      ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleOperator,std::ref(*this), std::placeholders::_1),x);

        auto y = range().element();
        copy(*b_,y);
        return y;
      }

      ::Algorithm::Vector d1(const ::Algorithm::Vector &x, const ::Algorithm::Vector &dx) const
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);

        auto y_ = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(dx,*y_);
        auto Ax = y_->copy();
        A_->mult(*y_, *Ax);

        auto result = range().element();
        copy(*Ax,result);

        return result;
      }

      LinearOperator linearization(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);
        assert(A_ != nullptr);
        return LinearizedOperator(Operator(F_,J_,bcs_,A_,domain_ptr(),range_ptr()),x);
      }

      LinearSolver solver() const
      {
        assert (A_ != nullptr);
        return LUSolver( A_ , *F_.function_space(0) , range_ptr() , domain_ptr() );
      }

    private:
      void assembleOperator(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( b_ != nullptr && (oldX_F == x) ) return;

        dummy_.vector() = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*dummy_.vector());
        Assign_X_If_Present<decltype(F_)>::apply(F_,dummy_);
        b_ = dummy_.vector()->factory().create_vector();

        dolfin::Assembler assembler;
        assembler.assemble(*b_, F_);

        for(const auto& bc : bcs_)
          bc->apply( *b_ , *dummy_.vector() );

        oldX_F = x;
      }

      void assembleGradient(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( A_ != nullptr && ( oldX_J == x ) ) return;

        dummy_.vector() = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*dummy_.vector());
        Assign_X_If_Present<decltype(J_)>::apply(J_,dummy_);
        A_ = dummy_.vector()->factory().create_matrix();

        dolfin::Assembler assembler;
        assembler.assemble(*A_, J_);

        auto tmp = dummy_.vector()->copy();
        for(const auto& bc : bcs_)
          bc->apply( *A_ , *tmp , *dummy_.vector() );

        oldX_J = x;
      }


      mutable ResidualForm F_;
      mutable JacobianForm J_;
      std::vector<const dolfin::DirichletBC*> bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
      mutable std::shared_ptr<dolfin::GenericVector> b_ = nullptr;
      mutable ::Algorithm::Vector oldX_F, oldX_J;
      mutable dolfin::Function dummy_;
    };


    /**
     * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow Y\f$ as used in Fenics.
     * @return ::Algorithm::Fenics::Operator<ResidualForm,JacobianForm>( F , J , bcs , domain , range )
     */
    template <class ResidualForm, class JacobianForm>
    auto makeOperator(ResidualForm& F, JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
                      ::Algorithm::VectorSpace& domain, ::Algorithm::VectorSpace& range)
    {
      return ::Algorithm::Fenics::Operator<ResidualForm,JacobianForm>( F , J , bcs , domain , range );
    }

    /**
     * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow X\f$ as used in Fenics.
     * @return ::Algorithm::Fenics::Operator<ResidualForm,JacobianForm>( F , J , bcs , space , space )
     */
    template <class ResidualForm, class JacobianForm>
    auto makeOperator(ResidualForm& F, JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, ::Algorithm::VectorSpace& space)
    {
      return ::Algorithm::Fenics::Operator<ResidualForm,JacobianForm>( F , J , bcs , space , space );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_OPERATOR_HH
