#pragma once

#include <Eigen/Dense>

#include <Spacy/Util/Mixins/Get.hh>
#include <Spacy/Util/Base/AddArithmeticOperators.hh>
#include <Spacy/Util/Base/OperatorBase.hh>
#include <Spacy/Util/Base/VectorBase.hh>

namespace Spacy
{
  namespace Rn
  {
    /// @cond
    class LinearSolver;
    /// @endcond

    /**
     * @ingroup EigenGroup
     * @brief A linear operator \f$A:X\rightarrow Y\f$ for finite-dimensional problems, based on the %Eigen library.
     */
    class LinearOperator :
      public Mixin::Get< ::Eigen::MatrixXd >,
      public VectorBase,
      public OperatorBase ,
      public AddArithmeticOperators<LinearOperator>
    {
    public:
      /**
       * @brief LinearOperator
       * @param A matrix specifying the linear operator
       * @param space space of linear operators \f$X\rightarrow Y\f$
       * @param domain \f$X\f$
       * @param range \f$Y\f$
       */
      LinearOperator(::Eigen::MatrixXd A, const VectorSpace& space, const VectorSpace& domain, const VectorSpace& range);

      /// Compute \f$A(x)\f$
      ::Spacy::Vector operator()(const ::Spacy::Vector& dx) const;

      /// Apply as dual element, currently not implemented.
      Real operator()(const LinearOperator&) const;

      /// Access solver representing \f$A^{-1}\f$
      LinearSolver solver() const;
    };
  }
}
