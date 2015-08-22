#ifndef ALGORITHM_VECTOR_HH
#define ALGORITHM_VECTOR_HH

#include <memory>

//#include "Interface/abstractVector.hh"
#include "Util/Mixins/impl.hh"

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/operators.hpp>

#include "Util/conceptBase.hh"

BOOST_TYPE_ERASURE_MEMBER( (has_space) , space , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_spaceIndex) , spaceIndex , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_size) , size , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_isAdmissible) , isAdmissible , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_norm) , norm , 1 )

BOOST_TYPE_ERASURE_MEMBER( (has_scalarProduct) , scalarProduct , 2 )

namespace Algorithm
{
  /// \cond
  //class VectorSpace;
  /// \endcond

  namespace te = boost::type_erasure;

  using VectorConcept =
  boost::mpl::vector<
    te::assignable<> ,
    te::multiply_assignable< te::_self , double > ,
    te::add_assignable<> ,
    te::subtract_assignable<> ,
    te::negatable<> ,
    te::equality_comparable<> ,
    te::callable<double(const te::_self&), const te::_self> ,
    has_spaceIndex<unsigned(), const te::_self> ,
//    has_space<const VectorSpace&(), const te::_self> ,
    has_isAdmissible<bool(), const te::_self> ,
    has_size<unsigned(), const te::_self> ,
    has_norm<double(const te::_self&), const te::_self> ,
    has_scalarProduct<double(const te::_self&,const te::_self&), const te::_self>
  >;

  using Vector =
  te::any<
    boost::mpl::vector<
      ConceptBase ,
      VectorConcept
    >
  >;



////  template <class> struct Scale;

//  /**
//   * @brief Representation of a function space element.
//   *
//   * This class is the base class for all variables. Their specific form is specified in an Implementation derived from AbstractVector.
//   */
//  class Vector : public Mixin::UniqueImpl<Interface::AbstractVector>
//  {
//  public:
//    /// Default constructor.
//    Vector();

//    /**
//     * @brief Construct Vector from implementation.
//     */
//    Vector(std::unique_ptr<Interface::AbstractVector>&& implementation);

//    /**
//     * @brief Construct Vector from implementation.
//     */
//    Vector(const Interface::AbstractVector& implementation);

//    /**
//     * @brief Copy constructor.
//     */
//    Vector(const Vector&) = default;

//    /**
//     * @brief Copy assignment.
//     */
//    Vector& operator=(const Vector&);

//    /**
//     * @brief Copy assignment.
//     */
//    template <class T>
//    Vector& operator=(const Scale<T>& s)
//    {
//      Vector y(s.x);
//      y *= s.a;

//      if( !implIsNullPtr() )
//        y.impl().copyTo(impl());
//      else
//        setImpl( clone( y.impl() ) );
//      return *this;
//    }

//    /**
//     * @brief Assign from implementation.
//     */
//    Vector& operator=(const Interface::AbstractVector& implementation);

//    /**
//     * @brief print information on this function space element
//     */
//    void print(std::ostream&) const;

//    /**
//     * @brief Access index of the underlying function space.
//     */
//    unsigned spaceIndex() const;


//    /**
//     * \brief In-place multiplication with arithmetic types.
//     */
//    template <class Arithmetic,
//              class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
//    Vector& operator*=(const Arithmetic& a)
//    {
//      impl() *= a;
//      return *this;
//    }

//    /**
//     * @brief In-place summation.
//     */
//    Vector& operator+=(const Vector& y);

//    /**
//     * @brief Axpy for \f$ x += a*y \f$.
//     */
//    template <class T>
//    Vector& operator+=(Scale<T>&& s)
//    {
//      impl().axpy(s.a,s.x.impl());
//      return *this;
//    }
//    /**
//     * @brief In-place subtraction.
//     */
//    Vector& operator-=(const Vector& y);

//    /**
//     * @brief Axpy for \f$ x -= a*y \f$.
//     */
//    template <class T>
//    Vector& operator-=(Scale<T>&& s)
//    {
//      impl().axpy(-s.a,s.x.impl());
//      return *this;
//    }

//    /**
//     * @brief Get \f$-x\f$.
//     */
//    Vector operator-() const;

//    /**
//     * \brief Apply as dual element.
//     */
//    double operator()(const Vector& y) const;

//    /**
//     * @brief Equality check.
//     */
//    bool operator==(const Vector& y) const;

//    /**
//     * \brief Access to entry of coefficient vector.
//     */
//    double& coefficient(unsigned);

//    /**
//     * \brief Access to entry of coefficient vector.
//     */
//    const double& coefficient(unsigned) const;

//    /**
//     * @brief Size of coefficient vector.
//     */
//    unsigned size() const;

//    /**
//     * @brief Checks if a function space is admissible in the case that the search space is a subset of a function space which is not a subspace.
//     */
//    bool isAdmissible() const;

//    /**
//     * @brief Compute norm, where the norm associated with the underlying function space is used.
//     */
//    double norm() const;
//  };


  /**
   * @brief Compute \f$z=a*x\f$.
   */
  template <class Arithmetic,
            class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
  Vector operator*(Arithmetic a, Vector x)
  {
    return x*=a;//Scale<Arithmetic>{a,x};
  }


  /**
   * @brief Compute \f$z=a*x\f$.
   */
  template <class Arithmetic,
            class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
  Vector operator*(const Vector& x, Arithmetic a)
  {
    return a*x;
  }

  /**
   * @brief Compute \f$z=x+y\f$.
   */
  Vector operator+(Vector x, const Vector& y);

  /**
   * @brief Compute \f$z=x-y\f$.
   */
  Vector operator-(Vector x, const Vector& y);

  /**
   * @brief Compute \f$z=a+x\f$.
   * @param a
   * @param x function space element with implementation of type Real.
   */
  Vector operator+(double a, Vector x);

  /**
   * @brief Compute \f$z=a+x\f$.
   * @param a
   * @param x function space element with implementation of type Real.
   */
  Vector operator+(const Vector& x, double a);

  /**
   * @brief Compute \f$z=x*y=(x,y)\f$.
   */
  double operator*(const Vector& x, const Vector& y);


  /**
   * @brief Compute norm, where the norm associated with the underlying function space is used.
   */
  double norm(const Vector& x);


  /**
   * @brief Print function space element to os.
   */
//  std::ostream& operator<<(std::ostream& os, const Vector& x);

//  template <class T>
//  struct Scale
//  {
//    operator Vector() const
//    {
//      auto y = x;
//      return y *= a;
//    }

//    T a;
//    const Vector& x;
//  };
}

#endif // ALGORITHM_VECTOR_HH
