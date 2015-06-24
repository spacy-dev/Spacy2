#ifndef ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH

#include "abstractC1Functional.hh"
#include "hessian.hh"

namespace Algorithm
{
  class AbstractLinearOperator;

  class AbstractC2Functional : public AbstractC1Functional
  {
  public:
    AbstractC2Functional(const AbstractBanachSpace& domain);

//    double d1(const AbstractFunctionSpaceElement &dx) const override;
    Hessian getHessian(const AbstractFunctionSpaceElement& x) const;

  protected:
    virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx) const = 0;

    virtual Hessian makeHessian(const AbstractFunctionSpaceElement& x) const;
    //    virtual double d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const = 0;

//    virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement &dx) const = 0;

//    std::shared_ptr<AbstractC1Operator> getDerivative() const;

//    void setDerivative(std::shared_ptr<AbstractC1Operator> derivative);

//    using AbstractC1Functional::d1;
//  private:
//    std::shared_ptr<AbstractC1Operator> derivative_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH
