#ifndef SPACY_VECTOR_HH
#define SPACY_VECTOR_HH

#include <type_traits>
#include <boost/type_erasure/any.hpp>

#include "Spacy/Util/Concepts/vectorConcept.hh"
#include "Spacy/Util/Mixins/impl.hh"
#include "Spacy/Util/cast.hh"

namespace Spacy
{
  /// @cond
  class Real;
  /// @endcond

  /** \addtogroup SpacyGroup
   * @{
   */

  /**
   * @anchor VectorAnchor
   * @brief Vector class.  Can store objects that satisfy the requirements of \ref VectorConceptAnchor "VectorConcept".
   */
  using AnyVector = boost::type_erasure::any< Concepts::VectorConcept >;

  class Vector : public Mixin::CopyingUniqueImpl<AnyVector>
  {
  public:
    Vector()
      : Mixin::CopyingUniqueImpl<AnyVector>( std::make_unique<AnyVector>() )
    {}

    template <class VImpl,
              class = std::enable_if_t<!std::is_same<std::decay_t<VImpl>,Vector>::value>
              >
    Vector(VImpl&& v)
      : Mixin::CopyingUniqueImpl<AnyVector>(std::make_unique<AnyVector>(std::forward<VImpl>(v)))
    {}

//    Vector(AnyVector v);

 //   operator AnyVector() const;

    /**
     * @brief In-place summation \f$ x+=y\f$.
     * @param y vector to add to this vector
     * @return \f$ x+=y\f$.
     */
    Vector& operator+=(const Vector& y);

    /**
     * @brief In-place subtraction \f$ x-=y\f$.
     * @param y vector to subtract from this vector
     * @return \f$ x-=y\f$.
     */
    Vector& operator-=(const Vector& y);

    /**
     * @brief In-place multiplication \f$ x*=a\f$.
     * @param a scaling factor
     * @return \f$ x*=a\f$.
     */
    Vector& operator*=(double a);

    /**
     * @brief Negation \f$ -x\f$.
     * @return \f$ -x \f$.
     */
    Vector operator-() const;

    Vector operator()(const Vector& y) const;

    /**
     * @brief Comparison operator \f$ x==y\f$.
     * @param y vector to compare with this vector
     * @return \f$ x==y\f$.
     */
    bool operator==(const Vector& y) const;

    const VectorSpace& space() const;
  };

  template <class ToType>
  bool is(const Spacy::Vector& v)
  {
    return is<ToType>(v.impl());
  }

  template <class ToType>
  const ToType& cast_ref(const Spacy::Vector& v)
  {
    return cast_ref<ToType>(v.impl());
  }

  template <class ToType>
  ToType& cast_ref(Spacy::Vector& v)
  {
    return cast_ref<ToType>(v.impl());
  }

////  template <class> struct Scale;

  /**
   * @brief Multiplication with arithmetic types (double,float,int,...).
   * @return \f$z=a*x\f$.
   */
//  template <class Arithmetic,
//            class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
  Vector operator*(double a, Vector x);

  Vector operator*(Vector x, double a);

  /**
   * @brief Sum of vectors.
   * @return Compute \f$z=x+y\f$.
   */
  Vector operator+(Vector x, const Vector& y);

  /**
   * @brief Subtract vectors.
   * @return \f$z=x-y\f$.
   */
  Vector operator-(Vector x, const Vector& y);

  /**
   * @brief Compute scalar product.
   * @return \f$z=x*y=(x,y)\f$.
   */
  Real operator*(const Vector& x, const Vector& y);

  /**
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
  /** @} */
}

#endif // SPACY_VECTOR_HH
