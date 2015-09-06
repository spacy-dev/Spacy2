#ifndef SPACY_VECTOR_HH
#define SPACY_VECTOR_HH

#include <type_traits>
#include <boost/type_erasure/any.hpp>

#include "Spacy/Util/Concepts/vectorConcept.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  /**
   * @ingroup SpacyGroup
   * @anchor VectorAnchor
   * @brief Vector class.  Can store objects that satisfy the requirements of \ref VectorConceptAnchor "VectorConcept".
   */
  using Vector = boost::type_erasure::any< Concepts::VectorConcept >;

////  template <class> struct Scale;

  /**
   * @ingroup SpacyGroup
   * @brief Multiplication with arithmetic types (double,float,int,...).
   * @return \f$z=a*x\f$.
   */
//  template <class Arithmetic,
//            class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
  inline Vector operator*(double a, Vector x)
  {
    return x*=a;//Scale<Arithmetic>{a,x};
  }

  /**
   * @ingroup SpacyGroup
   * @brief Sum of vectors.
   * @return Compute \f$z=x+y\f$.
   */
  Vector operator+(Vector x, const Vector& y);

  /**
   * @ingroup SpacyGroup
   * @brief Subtract vectors.
   * @return \f$z=x-y\f$.
   */
  Vector operator-(Vector x, const Vector& y);

  /**
   * @ingroup SpacyGroup
   * @brief Compute scalar product.
   * @return \f$z=x*y=(x,y)\f$.
   */
  Real operator*(const Vector& x, const Vector& y);

  /**
   * @ingroup SpacyGroup
   * @brief Compute norm, where the norm associated with the underlying function space is used.
   * @return \f$ z = \|x\| \f$.
   */
  Real norm(const Vector& x);

  void checkDualPairing(const Vector& x, const Vector& y);
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

#endif // SPACY_VECTOR_HH
