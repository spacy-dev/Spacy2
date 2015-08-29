#ifndef ALGORITHM_ADAPTER_FENICS_OPERATOR_HH
#define ALGORITHM_ADAPTER_FENICS_OPERATOR_HH

#include <memory>

#include <dolfin.h>

#include "Algorithms/linearizedOperator.hh"
#include "Algorithms/vectorSpace.hh"
#include "Algorithms/Util/Mixins/disableAssembly.hh"
#include "Algorithms/Util/Base/operatorBase.hh"

#include "luSolver.hh"
#include "util.hh"
#include "assignXIfPresent.hh"

namespace Algorithm
{
  namespace Fenics
  {
    /**
     * @ingroup FenicsGroup
     * @brief %Operator interface for FEniCS. Models a differentiable operator \f$A:X\rightarrow Y\f$.
     * @warning In the .ufl file you have to name the argument of \f$f\f$ by "x"!
     * @see @ref C1OperatorAnchor "C1Operator", @ref C1OperatorConceptAnchor "C1OperatorConcept"
     */
    template <class ResidualForm, class JacobianForm>
    class Operator :
        public OperatorBase ,
        public Mixin::DisableAssembly
    {
    public:
      /**
       * @brief Construct operator for FEnics.
       * @param F Residual form for the evaluation of \f$A\f$
       * @param J Jacobian form for the evaluation of \f$A'\f$
       * @param bcs Dirichlet boundary conditions
       * @param domain domain space \f$X\f$
       * @param range range space \f$Y\f$
       */
      Operator(const ResidualForm& F, const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
               VectorSpace& domain, VectorSpace& range)
        : OperatorBase( domain , range ),
          F_( F.function_space(0) ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs )
      {
        copyCoefficients(F,F_);
        copyCoefficients(J,J_);
      }

      /**
       * @brief Construct operator without boundary conditions for FEnics.
       * @param F Residual form for the evaluation of \f$A\f$
       * @param J Jacobian form for the evaluation of \f$A'\f$
       * @param domain domain space \f$X\f$
       * @param range range space \f$Y\f$
       */
      Operator(const ResidualForm& F, const JacobianForm& J, VectorSpace& domain, VectorSpace& range)
        : OperatorBase( domain , range ),
          F_( F.function_space(0) ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_()
      {
        copyCoefficients(F,F_);
        copyCoefficients(J,J_);
      }

      /**
       * @brief Move constructor.
       * @param other object to move from
       */
      Operator(Operator&& other)
        : OperatorBase( other ) ,
          Mixin::DisableAssembly(other.assemblyIsDisabled()) ,
          F_( other.F_.function_space(0) ) ,
          J_( other.J_.function_space(0) , other.J_.function_space(1) ) ,
          bcs_( std::move(other.bcs_) ) ,
          A_( std::move(other.A_) ) ,
          b_( std::move(other.b_) )
      {
        copyCoefficients(other.F_,F_);
        copyCoefficients(other.J_,J_);
      }

      /**
       * @brief Move constructor.
       * @param other object to copy from
       */
      Operator(const Operator& other)
        : OperatorBase( other ) ,
          Mixin::DisableAssembly(other.assemblyIsDisabled()) ,
          F_( other.F_.function_space(0) ) ,
          J_( other.J_.function_space(0) , other.J_.function_space(1) ) ,
          bcs_( other.bcs_ ) ,
          A_( (other.A_ != nullptr) ? other.A_->copy() : nullptr ) ,
          b_( (other.b_ != nullptr) ? other.b_->copy() : nullptr )
      {
        copyCoefficients(other.F_,F_);
        copyCoefficients(other.J_,J_);
      }

      /**
       * @brief Copy assignment.
       * @param other object to copy from
       */
      Operator& operator=(const Operator& other)
      {
        OperatorBase::operator=( other );
        disableAssembly(other.assemblyIsDisabled());
        F_ = other.F_;
        J_ = other.J_;
        bcs_ = other.bcs_;
        A_ = (other.A_ != nullptr) ? other.A_->copy() : nullptr;
        b_ = (other.b_ != nullptr) ? other.b_->copy() : nullptr;

        copyCoefficients(other.F_,F_);
        copyCoefficients(other.J_,J_);
      }

      /**
       * @brief Move assignment.
       * @param other object to move from
       */
      Operator& operator=(Operator&& other)
      {
        OperatorBase::operator=( other );
        disableAssembly(other.assemblyIsDisabled());
        F_ = other.F_;
        J_ = other.J_;
        bcs_ = std::move(other.bcs_);
        A_ = std::move(other.A_);
        b_ = std::move(other.b_);

        copyCoefficients(other.F_,F_);
        copyCoefficients(other.J_,J_);
      }

      /**
       * @brief Compute \f$A(x)\f$.
       */
      ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleOperator,std::ref(*this), std::placeholders::_1),x);

        auto y = range().vector();
        copy(*b_,y);
        return std::move(y);
      }

      /**
       * @brief Compute \f$A'(x)dx\f$.
       * @param x iterate
       * @param dx correction
       * @return \f$A'(x)dx\f$
       */
      ::Algorithm::Vector d1(const ::Algorithm::Vector &x, const ::Algorithm::Vector &dx) const
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);

        auto dx_ = dolfin::Function( J_.function_space(0) );
        copy(dx,dx_);
        auto y_ = dx_.vector()->copy();
        A_->mult(*dx_.vector(), *y_);

        auto y = range().vector();
        copy(*y_,y);

        return std::move(y);
      }

      /**
       * @brief Access \f$A'(x)\f$ as linear operator \f$X\rightarrow Y\f$
       * @param x current iterate
       * @return LinearizedOperator
       * @see LinearizedOperator, LinearOperator, LinearOperatorConcept
       */
      auto linearization(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);
        assert(A_ != nullptr);
        Operator newOp = Operator(*this);
        newOp.disableAssembly();
        return LinearizedOperator( std::move(newOp) , x , LUSolver( *A_ , *F_.function_space(0) , range() , domain() ) );
      }

    private:
      /// Assemble discrete representation of \f$A(x)\f$.
      void assembleOperator(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( b_ != nullptr && (oldX_F == x) ) return;

        auto x_ = dolfin::Function( J_.function_space(0) );
        copy(x,x_);
        assign_x_if_present(F_,x_);
        b_ = x_.vector()->factory().create_vector();

        dolfin::Assembler assembler;
        assembler.assemble(*b_, F_);

        for(const auto& bc : bcs_)
          bc->apply( *b_ , *x_.vector() );

        oldX_F = x;
      }

      /// Assemble discrete representation of \f$A'(x)\f$.
      void assembleGradient(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( A_ != nullptr && ( oldX_J == x ) ) return;

        auto x_ = dolfin::Function( J_.function_space(0) );
        copy(x,x_);
        assign_x_if_present(J_,x_);
        A_ = x_.vector()->factory().create_matrix();

        dolfin::Assembler assembler;
        assembler.assemble(*A_, J_);

        for(const auto& bc : bcs_)
          bc->apply( *A_ , *x_.vector() , *x_.vector() );

        oldX_J = x;
      }

      mutable ResidualForm F_;
      mutable JacobianForm J_;
      std::vector<const dolfin::DirichletBC*> bcs_ = {};
      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
      mutable std::shared_ptr<dolfin::GenericVector> b_ = nullptr;
      mutable ::Algorithm::Vector oldX_F = {}, oldX_J = {};
    };


    /**
     * @ingroup FenicsGroup
     * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow Y\f$ as used in Fenics.
     * @return @ref Operator "::Algorithm::Fenics::Operator<ResidualForm,JacobianForm>( F , J , bcs , domain , range )"
     */
    template <class ResidualForm, class JacobianForm>
    auto makeOperator(ResidualForm& F, JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
                      VectorSpace& domain, VectorSpace& range)
    {
      return Operator<ResidualForm,JacobianForm>{ F , J , bcs , domain , range };
    }

    /**
     * @ingroup FenicsGroup
     * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow Y\f$ from FEniCS(dolfin).
     * @return @ref Operator "::Algorithm::Fenics::Operator<ResidualForm,JacobianForm>( F , J , domain , range )"
     */
    template <class ResidualForm, class JacobianForm>
    auto makeOperator(ResidualForm& F, JacobianForm& J, VectorSpace& domain, VectorSpace& range)
    {
      return Operator<ResidualForm,JacobianForm>{ F , J , domain , range };
    }

    /**
     * @ingroup FenicsGroup
     * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow X\f$ from FEniCS(dolfin).
     * @return @ref Operator "::Algorithm::Fenics::Operator<ResidualForm,JacobianForm>( F , J , bcs , space , space )"
     */
    template <class ResidualForm, class JacobianForm>
    auto makeOperator(ResidualForm& F, JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, VectorSpace& space)
    {
      return Operator<ResidualForm,JacobianForm>{ F , J , bcs , space , space };
    }

    /**
     * @ingroup FenicsGroup
     * @brief Convenient generation of a differentiable operator \f$A: X\rightarrow X\f$ from FEniCS(dolfin).
     * @return @ref Operator "Fenics::Operator<ResidualForm,JacobianForm>( F , J , space , space )"
     */
    template <class ResidualForm, class JacobianForm>
    auto makeOperator(ResidualForm& F, JacobianForm& J, VectorSpace& space)
    {
      return Operator<ResidualForm,JacobianForm>{ F , J , space , space };
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_OPERATOR_HH
