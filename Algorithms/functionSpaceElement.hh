#ifndef ALGORITHM_FUNCTIONSPACEELEMENT_HH
#define ALGORITHM_FUNCTIONSPACEELEMENT_HH

#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Interface/abstractHilbertSpace.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class AbstractBanachSpace;
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
  class FunctionSpaceElement : public Mixin::UniqueImpl<AbstractFunctionSpaceElement>
  {
  public:
    /// Default constructor.
    FunctionSpaceElement();

    /**
     * @brief Construct FunctionSpaceElement from implementation.
     */
    FunctionSpaceElement(std::unique_ptr<AbstractFunctionSpaceElement>&& implementation);

    /**
     * @brief Construct FunctionSpaceElement from implementation.
     */
    FunctionSpaceElement(const AbstractFunctionSpaceElement& implementation);

    /**
     * @brief Copy constructor.
     */
    FunctionSpaceElement(const FunctionSpaceElement&);

    /**
     * @brief Copy assignment.
     */
    FunctionSpaceElement& operator=(const FunctionSpaceElement&);

    /**
     * @brief Assign from implementation.
     */
    FunctionSpaceElement& operator=(const AbstractFunctionSpaceElement& implementation);

    /**
     * @brief print information on this function space element
     */
    void print(std::ostream&) const;

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
      impl() *= a;
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
     * \brief Apply as dual element.
     */
    double operator()(const FunctionSpaceElement& y) const;

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
  };

  /**
   * @brief Convenient generation of function space element from implementation arguments.
   * @return FunctionSpaceElement( std::make_shared<Implementation>(std::forward<Args>(args)...) )
   */
  template <class Implementation, class... Args>
  FunctionSpaceElement makeElement(Args&&... args)
  {
    return FunctionSpaceElement( std::make_unique<Implementation>(std::forward<Args>(args)...) );
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

  /**
   * @brief Compute \f$z=x-y\f$.
   */
  FunctionSpaceElement operator-(FunctionSpaceElement x, const FunctionSpaceElement& y);

  /**
   * @brief Compute \f$z=a+x\f$.
   * @param a
   * @param x function space element with implementation of type Real.
   */
  FunctionSpaceElement operator+(double a, FunctionSpaceElement x);

  /**
   * @brief Compute \f$z=a+x\f$.
   * @param a
   * @param x function space element with implementation of type Real.
   */
  FunctionSpaceElement operator+(const FunctionSpaceElement& x, double a);

  /**
   * @brief Compute \f$z=x*y=(x,y)\f$.
   */
  auto operator*(const FunctionSpaceElement& x, const FunctionSpaceElement& y) -> decltype(x.impl()*y.impl());


  /**
   * @brief Print function space element to os.
   */
  std::ostream& operator<<(std::ostream& os, const FunctionSpaceElement& x);
}

#endif // ALGORITHM_FUNCTIONSPACEELEMENT_HH
