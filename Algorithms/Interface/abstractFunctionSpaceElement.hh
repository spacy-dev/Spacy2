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

    virtual void copyTo(AbstractFunctionSpaceElement&) const = 0;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> clone() const = 0;

    virtual void print(std::ostream&) const = 0;

    virtual AbstractFunctionSpaceElement& operator+=(const AbstractFunctionSpaceElement& y) = 0;

    virtual AbstractFunctionSpaceElement& operator-=(const AbstractFunctionSpaceElement& y) = 0;

    virtual AbstractFunctionSpaceElement& operator*=(double) = 0;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> operator- () const = 0;

    /**
     * @brief Size of coefficient vector.
     */
    virtual unsigned size() const = 0;

    /**
     * @brief Access entry in coefficient vector.
     */
    virtual double& coefficient(unsigned) = 0;

    /**
     * @brief Access entry in coefficient vector.
     */
    virtual const double& coefficient(unsigned) const = 0;

    unsigned spaceIndex() const;

    const AbstractBanachSpace& getSpace() const;


  protected:
    const AbstractBanachSpace& space_;
  };

  std::ostream& operator<<(std::ostream& os, const AbstractFunctionSpaceElement& element);
}

#endif // ALGORITHM_INTERFACE_ABSTRACTFUNCTIONSPACEELEMENT_HH
