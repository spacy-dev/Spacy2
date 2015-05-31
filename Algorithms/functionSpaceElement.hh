#ifndef ALGORITHM_FUNCTIONSPACEELEMENT_HH
#define ALGORITHM_FUNCTIONSPACEELEMENT_HH

#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

#include "Interface/abstractHilbertSpace.hh"

namespace Algorithm
{
  class AbstractBanachSpace;
  class AbstractFunctionSpaceElement;
  class FunctionSpace;

  class FunctionSpaceElement;

  struct Scale
  {
    double a;
    const FunctionSpaceElement& x;
  };

  /**
   * @brief Representation of a function space element.
   *
   * This class is the base class for all variables. Their specific form is specified in an Implementation derived from AbstractFunctionSpaceElement.
   */
  class FunctionSpaceElement
  {
  public:
    FunctionSpaceElement();

    /**
     * @brief Constructor
     */
    FunctionSpaceElement(std::unique_ptr<AbstractFunctionSpaceElement>&& implementation);

    FunctionSpaceElement(const AbstractFunctionSpaceElement& implementation);

    FunctionSpaceElement(const FunctionSpaceElement&);

    FunctionSpaceElement& operator=(const FunctionSpaceElement&);

    FunctionSpaceElement& operator=(const AbstractFunctionSpaceElement& implementation);

    /**
     * @brief print information on this function space element
     */
    void print(std::ostream&) const;

    /**
     * @brief Access to the underlying implementation.
     */
    AbstractFunctionSpaceElement& impl();

    /**
     * @brief Access to the underlying implementation.
     */
    const AbstractFunctionSpaceElement& impl() const;

    /**
     * @brief Access index of the underlying function space.
     */
    unsigned spaceIndex() const;


    /**
     * \brief In-place multiplication with arithmetic types.
     */
    template <class Arithmetic,
              class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
    FunctionSpaceElement& operator*=(const Arithmetic& a)
    {
      (*impl_) *= a;
      return *this;
    }

    /**
     * @brief In-place summation.
     */
    FunctionSpaceElement& operator+=(const FunctionSpaceElement& y);

    /**
     * @brief Axpy for \f$ x += a*y \f$.
     */
    FunctionSpaceElement& operator+=(Scale&& y);

    /**
     * @brief In-place subtraction.
     */
    FunctionSpaceElement& operator-=(const FunctionSpaceElement& y);

    /**
     * @brief Get \f$-x\f$.
     */
    FunctionSpaceElement operator-() const;

    /**
     * @brief Equality check.
     */
    bool operator==(const FunctionSpaceElement& y) const;

    /**
     * \brief Access to entry of coefficient vector.
     */
    double& coefficient(unsigned);

    /**
     * \brief Access to entry of coefficient vector.
     */
    const double& coefficient(unsigned) const;

    /**
     * @brief Size of coefficient vector.
     */
    unsigned size() const;

  private:
    std::unique_ptr<AbstractFunctionSpaceElement> impl_ = nullptr;
  };

  /**
   * \brief Construct function space element from arguments for its implementation.
   */
  template <class Implementation, class... Args>
  FunctionSpaceElement makeElement(Args&&... args)
  {
    return FunctionSpaceElement( std::make_unique<Implementation>(args...) );
  }


  /**
   * @brief Compute \f$z=a*x\f$.
   */
  template <class Arithmetic,
            class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
  auto operator*(const Arithmetic& a, FunctionSpaceElement x)
  {
    return x *= a;
  }


  /**
   * @brief Compute \f$z=a*x\f$.
   */
  template <class Arithmetic,
            class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
  auto operator*(const FunctionSpaceElement& x, const Arithmetic& a)
  {
    return a*x;
  }

  /**
   * @brief Compute \f$z=x+y\f$.
   */
  FunctionSpaceElement operator+(FunctionSpaceElement x, const FunctionSpaceElement& y);

  FunctionSpaceElement operator+(double a, FunctionSpaceElement x);

  FunctionSpaceElement operator+(const FunctionSpaceElement& x, double a);

  auto operator*(const FunctionSpaceElement& x, const FunctionSpaceElement& y) -> decltype(x.impl()*y.impl());


  /**
   * @brief Print function space element to os.
   */
  std::ostream& operator<<(std::ostream& os, const FunctionSpaceElement& x);

  /**
   * \brief Create function space element of type Implementation from args... .
   */
  template <class Implementation, class... Args>
  FunctionSpaceElement createFunctionSpaceElement(Args&&... args)
  {
    return FunctionSpaceElement(std::make_unique<Implementation>(args...));
  }
}

#endif // ALGORITHM_FUNCTIONSPACEELEMENT_HH
