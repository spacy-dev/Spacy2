#pragma once

#include <Eigen/Dense>

namespace Spacy
{
  /// @cond
  class Vector;
  class VectorSpace;
  /// @endcond

  namespace Rn
  {
    /**
     * @ingroup EigenGroup
     * @brief Linear solver for %Eigen matrices.
     */
    class LinearSolver
    {
    public:
      LinearSolver(::Eigen::MatrixXd A,const VectorSpace& domain);

      Spacy::Vector operator()(const ::Spacy::Vector& y) const;

    private:
      ::Eigen::MatrixXd A_;
      const VectorSpace& domain_;
    };
  }
}
