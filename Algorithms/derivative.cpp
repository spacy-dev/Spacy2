#include "derivative.hh"

#include "functionSpaceElement.hh"
#include "c2Functional.hh"
#include "c2Operator.hh"

#include "Interface/abstractFunctionSpaceElement.hh"

#include "Interface/abstractC0Operator.hh"
#include "Interface/abstractC1Operator.hh"
#include "Interface/abstractC2Operator.hh"

#include "Interface/abstractC0Functional.hh"
#include "Interface/abstractC1Functional.hh"
#include "Interface/abstractC2Functional.hh"

namespace Algorithm
{
  namespace
  {
    class C1OperatorDerivative : public AbstractC0Operator
    {
    public:
      C1OperatorDerivative(const AbstractC1Operator& A, const AbstractFunctionSpaceElement& x)
        : A_(dynamic_cast<AbstractC1Operator*>(A.clone().release())),
          x_(x.clone())
      {
        (*A_)(*x_);
      }

      std::unique_ptr<AbstractC0Operator> clone() const override
      {
        return std::make_unique<C1OperatorDerivative>(*A_,*x_);
      }

      std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& dx) const override
      {
        return A_->d1(dx);
      }

      const AbstractBanachSpace& getDomain() const override
      {
        return A_->getDomain();
      }

      const AbstractBanachSpace& getRange() const override
      {
        return A_->getRange();
      }

    private:
      std::unique_ptr<AbstractC1Operator> A_;
      std::unique_ptr<AbstractFunctionSpaceElement> x_;
    };

    class C2OperatorDerivative : public AbstractC1Operator
    {
    public:
      C2OperatorDerivative(const AbstractC2Operator& A, const AbstractFunctionSpaceElement& x)
        : A_(dynamic_cast<AbstractC2Operator*>(A.clone().release())),
          x_(x.clone())
      {
        (*A_)(*x_);
      }

      std::unique_ptr<AbstractC0Operator> clone() const override
      {
        return std::make_unique<C2OperatorDerivative>(*A_,*x_);
      }

      std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& dx) const override
      {
        dx_ = dx.clone();
        return A_->d1(*dx_);
      }

      std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& dy) const override
      {
        return A_->d2(*dx_,dy);
      }

      const AbstractBanachSpace& getDomain() const override
      {
        return A_->getDomain();
      }

      const AbstractBanachSpace& getRange() const override
      {
        return A_->getRange();
      }

    private:
      std::unique_ptr<AbstractC2Operator> A_;
      std::unique_ptr<AbstractFunctionSpaceElement> x_;
      mutable std::unique_ptr<AbstractFunctionSpaceElement> dx_;
    };


    class C1FunctionalDerivative : public AbstractC0Functional
    {
    public:
      C1FunctionalDerivative(const AbstractC1Functional& A, const AbstractFunctionSpaceElement& x)
        : AbstractC0Functional(A.getDomain()),
          A_(dynamic_cast<AbstractC1Functional*>(A.clone().release())),
          x_(x.clone())
      {
        (*A_)(*x_);
      }

      std::unique_ptr<AbstractC0Functional> clone() const override
      {
        return std::make_unique<C1FunctionalDerivative>(*A_,*x_);
      }

      double operator()(const AbstractFunctionSpaceElement& dx) const override
      {
        return A_->d1(dx);
      }

    private:
      std::unique_ptr<AbstractC1Functional> A_;
      std::unique_ptr<AbstractFunctionSpaceElement> x_;
    };

    class C2FunctionalDerivative : public AbstractC1Functional
    {
    public:
      C2FunctionalDerivative(const AbstractC2Functional& A, const AbstractFunctionSpaceElement& x)
        : AbstractC1Functional(A.getDomain()),
          A_(dynamic_cast<AbstractC2Functional*>(A.clone().release())),
          x_(x.clone())
      {
        (*A_)(*x_);
      }

      std::unique_ptr<AbstractC0Functional> clone() const override
      {
        return std::make_unique<C2FunctionalDerivative>(*A_,*x_);
      }

      double operator()(const AbstractFunctionSpaceElement& dx) const override
      {
        dx_ = dx.clone();
        return A_->d1(*dx_);
      }

      double d1(const AbstractFunctionSpaceElement& dy) const override
      {
        return A_->d2(*dx_,dy);
      }

    private:
      std::unique_ptr<AbstractC2Functional> A_;
      std::unique_ptr<AbstractFunctionSpaceElement> x_;
      mutable std::unique_ptr<AbstractFunctionSpaceElement> dx_;
    };
  }



  C0Operator derivative(const C1Operator& A, const FunctionSpaceElement& x)
  {
    return C0Operator(std::make_shared<C1OperatorDerivative>(A.impl(),x.impl()) );
  }

  C1Operator derivative(const C2Operator& A, const FunctionSpaceElement& x)
  {
    return C1Operator(std::make_shared<C2OperatorDerivative>(A.impl(),x.impl()) );
  }

  C0Functional derivative(const C1Functional& A, const FunctionSpaceElement& x)
  {
    return C0Functional(std::make_shared<C1FunctionalDerivative>(A.impl(),x.impl()) );
  }

  C1Functional derivative(const C2Functional& A, const FunctionSpaceElement& x)
  {
    return C1Functional(std::make_shared<C2FunctionalDerivative>(A.impl(),x.impl()) );
  }
}
