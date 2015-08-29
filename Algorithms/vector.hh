#ifndef ALGORITHM_VECTOR_HH
#define ALGORITHM_VECTOR_HH

#include <type_traits>
#include <boost/type_erasure/any.hpp>

#include "Algorithms/vectorSpace.hh"
#include "Algorithms/Util/Concepts/vectorConcept.hh"
#include "Algorithms/Util/Exceptions/incompatibleSpaceException.hh"

namespace Algorithm
{
  /**
   * @ingroup VHatGroup
   * @anchor VectorAnchor
   * @brief Vector class.  Can store objects that satisfy the requirements of \ref VectorConceptAnchor "VectorConcept".
   */
  using Vector = boost::type_erasure::any< Concepts::VectorConcept >;

////  template <class> struct Scale;

  /**
   * @ingroup VHatGroup
   * @brief Multiplication with arithmetic types (double,float,int,...).
   * @return \f$z=a*x\f$.
   */
  template <class Arithmetic,
            class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
  Vector operator*(Arithmetic a, Vector x)
  {
    return x*=a;//Scale<Arithmetic>{a,x};
  }

  /**
   * @ingroup VHatGroup
   * @brief Sum of vectors.
   * @return Compute \f$z=x+y\f$.
   */
  Vector operator+(Vector x, const Vector& y);

  /**
   * @ingroup VHatGroup
   * @brief Subtract vectors.
   * @return \f$z=x-y\f$.
   */
  Vector operator-(Vector x, const Vector& y);

  /**
   * @ingroup VHatGroup
   * @brief Compute scalar product.
   * @return \f$z=x*y=(x,y)\f$.
   */
  double operator*(const Vector& x, const Vector& y);

  /**
   * @ingroup VHatGroup
   * @brief Compute norm, where the norm associated with the underlying function space is used.
   * @return \f$ z = \|x\| \f$.
   */
  double norm(const Vector& x);


  template <class V>
  void checkSameSpaces(const V& x, const V& y)
  {
    if( x.space()->index() != y.space()->index() )
      throw IncompatibleSpaceException("checkSameSpaces",x.space()->index(),y.space()->index());
  }
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
