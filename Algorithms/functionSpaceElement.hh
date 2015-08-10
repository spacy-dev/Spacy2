#ifndef ALGORITHM_FUNCTIONSPACEELEMENT_HH
#define ALGORITHM_FUNCTIONSPACEELEMENT_HH

#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

#include "Interface/abstractFunctionSpace.hh"
#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  template <class> struct Scale;

  /**
   * @brief Representation of a function space element.
   *
   * This class is the base class for all variables. Their specific form is specified in an Implementation derived from AbstractFunctionSpaceElement.
   */
  class FunctionSpaceElement : public Mixin::UniqueImpl<Interface::AbstractFunctionSpaceElement>
  {
  public:
    /// Default constructor.
    FunctionSpaceElement();

    /**
     * @brief Construct FunctionSpaceElement from implementation.
     */
    FunctionSpaceElement(std::unique_ptr<Interface::AbstractFunctionSpaceElement>&& implementation);

    /**
     * @brief Construct FunctionSpaceElement from implementation.
     */
    FunctionSpaceElement(const Interface::AbstractFunctionSpaceElement& implementation);

    /**
     * @brief Copy constructor.
     */
    FunctionSpaceElement(const FunctionSpaceElement&) = default;

    /**
     * @brief Copy assignment.
     */
    FunctionSpaceElement& operator=(const FunctionSpaceElement&);

    /**
     * @brief Copy assignment.
     */
    template <class T>
    FunctionSpaceElement& operator=(const Scale<T>& s)
    {
      FunctionSpaceElement y(s.x);
      y *= s.a;

      if( !implIsNullPtr() )
        y.impl().copyTo(impl());
      else
        setImpl( clone( y.impl() ) );
      return *this;
    }

    /**
     * @brief Assign from implementation.
     */
    FunctionSpaceElement& operator=(const Interface::AbstractFunctionSpaceElement& implementation);

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
    template <class T>
    FunctionSpaceElement& operator+=(Scale<T>&& s)
    {
      impl().axpy(s.a,s.x.impl());
      return *this;
    }
    /**
     * @brief In-place subtraction.
     */
    FunctionSpaceElement& operator-=(const FunctionSpaceElement& y);

    /**
     * @brief Axpy for \f$ x -= a*y \f$.
     */
    template <class T>
    FunctionSpaceElement& operator-=(Scale<T>&& s)
    {
      impl().axpy(-s.a,s.x.impl());
      return *this;
    }

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

    /**
     * @brief Checks if a function space is admissible in the case that the search space is a subset of a function space which is not a subspace.
     */
    bool isAdmissible() const;

    /**
     * @brief Compute norm, where the norm associated with the underlying function space is used.
     */
    double norm() const;
  };


  /**
   * @brief Compute \f$z=a*x\f$.
   */
  template <class Arithmetic,
            class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
  auto operator*(const Arithmetic& a, FunctionSpaceElement x)
  {
    return Scale<Arithmetic>{a,x};
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
   * @brief Compute norm, where the norm associated with the underlying function space is used.
   */
  double norm(const FunctionSpaceElement& x);


  /**
   * @brief Print function space element to os.
   */
  std::ostream& operator<<(std::ostream& os, const FunctionSpaceElement& x);

  template <class T>
  struct Scale
  {
    operator FunctionSpaceElement() const
    {
      auto y = x;
      return y *= a;
    }

    T a;
    const FunctionSpaceElement& x;
  };
}

#endif // ALGORITHM_FUNCTIONSPACEELEMENT_HH
