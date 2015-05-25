#ifndef ALGORITHM_INTERFACE_ABSTRACTFUNCTIONSPACEELEMENT_HH
#define ALGORITHM_INTERFACE_ABSTRACTFUNCTIONSPACEELEMENT_HH

#include <iostream>
#include <memory>
#include <vector>

namespace Algorithm
{
  class AbstractBanachSpace;

  class AbstractFunctionSpaceElement
  {
  public:
    explicit AbstractFunctionSpaceElement(const AbstractBanachSpace& space);

    virtual ~AbstractFunctionSpaceElement() = default;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> clone() const = 0;

    ///
    virtual std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement&) const = 0;

    virtual void print(std::ostream&) const = 0;

    virtual AbstractFunctionSpaceElement& operator+=(const AbstractFunctionSpaceElement& y) = 0;

    virtual AbstractFunctionSpaceElement& operator-=(const AbstractFunctionSpaceElement& y) = 0;

    virtual AbstractFunctionSpaceElement& operator*=(double) = 0;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> operator- () const = 0;

    virtual std::vector<double>& coefficients() = 0;

    virtual const std::vector<double>& coefficients() const = 0;

    virtual double& coefficient(unsigned) = 0;

    virtual const double& coefficient(unsigned) const = 0;

    unsigned spaceIndex() const;

    const AbstractBanachSpace& getSpace() const;

  protected:
    const AbstractBanachSpace& space_;
  };

//  inline std::unique_ptr<AbstractFunctionSpaceElement> operator+(AbstractFunctionSpaceElement x, const AbstractFunctionSpaceElement& y);
//  Real   operator* (const Real&, double);
//  Real   operator* (double, const Real&);
//  Real   operator+ (const Real&, const Real&);

  std::ostream& operator<<(std::ostream& os, const AbstractFunctionSpaceElement& element);
}

#endif // ALGORITHM_INTERFACE_ABSTRACTFUNCTIONSPACEELEMENT_HH
