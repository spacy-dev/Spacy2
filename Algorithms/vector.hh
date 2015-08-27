#ifndef ALGORITHM_VECTOR_HH
#define ALGORITHM_VECTOR_HH

#include <type_traits>
#include <boost/type_erasure/any.hpp>

#include "Util/Concepts/vectorConcept.hh"
#include "vectorSpace.hh"

namespace Algorithm
{
  /**
   * @brief Vector class.  Can store objects that satisfy the requirements of VectorConcept.
   * @see Concepts::VectorConcept
   */
  using Vector = boost::type_erasure::any<Concepts::VectorConcept >;

////  template <class> struct Scale;

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
   * @brief Compute \f$z=x+y\f$.
   */
  Vector operator+(Vector x, const Vector& y);

  /**
   * @brief Compute \f$z=x-y\f$.
   */
  Vector operator-(Vector x, const Vector& y);

  /**
   * @brief Compute scalar product \f$z=x*y=(x,y)\f$.
   */
  double operator*(const Vector& x, const Vector& y);

  /**
   * @brief Compute norm, where the norm associated with the underlying function space is used.
   */
  double norm(const Vector& x);

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
