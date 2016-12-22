#pragma once

#include <functional>
#include <memory>
#include <Eigen/Dense>

#include <Spacy/Util/Base/FunctionalBase.hh>


namespace Spacy
{
  /// @cond
  class LinearOperator;
  class Real;
  class Vector;
  /// @endcond

  /** @addtogroup ScalarGroup @{ */
namespace Rn
{
    /// @cond
    class LinearOperator;
    /// @endcond

enum class RoleOfFunctional{ NORMAL, TANGENTIAL };

template<RoleOfFunctional role>
class CompositeStepFunctional : public Spacy::FunctionalBase 
{
	public:
	CompositeStepFunctional(
	            std::function<double(const ::Eigen::VectorXd&)> f,
				std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > df,
                std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > ddf,
				std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > c,
                std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > dc,
                std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&, const ::Eigen::VectorXd&) > pddc,
                const Spacy::VectorSpace& domain);

	CompositeStepFunctional(
	            const CompositeStepFunctional<RoleOfFunctional::TANGENTIAL>& L_T,
	            std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > M);

   // Compute f(x).
  Spacy::Real operator()(const ::Spacy::Vector& x) const;
 
  // Compute f'(x) as element of X*.
  Spacy::Vector d1(const ::Spacy::Vector& x) const;
  
  // Compute f''(x)dx as element of X*.
  Spacy::Vector d2(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const;
  
  // Access f''(x) as mapping f''(x): X->X*.
  Spacy::Rn::LinearOperator hessian(const ::Spacy::Vector& x) const;
 
  friend class CompositeStepFunctional<RoleOfFunctional::NORMAL>;

private:
     
	 std::function<double(const ::Eigen::VectorXd&)> f_;
     std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > df_;
     std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > ddf_;
	 std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > c_;
     std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > dc_;
     std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&, const ::Eigen::VectorXd&) > pddc_;
     std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > M_;
     std::shared_ptr<VectorSpace> operatorSpace_ = nullptr;

};

using NormalStepFunctional = CompositeStepFunctional<RoleOfFunctional::NORMAL>;

using TangentialStepFunctional = CompositeStepFunctional<RoleOfFunctional::TANGENTIAL>;

}
  /** @} */
}
