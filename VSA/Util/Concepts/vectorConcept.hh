#ifndef ALGORITHM_VECTOR_CONCEPT_HH
#define ALGORITHM_VECTOR_CONCEPT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/type_erasure/operators.hpp>
#include "conceptBase.hh"
#include "singleConcepts.hh"

namespace VSA
{
  /// @cond
  class VectorSpace;
  /// @endcond

  namespace Concepts
  {
    /**
     * @ingroup ConceptGroup
     * @anchor VectorConceptAnchor
     * @brief Concept for vector implementations.
     *
     * The minimal signature of a vector is:
     * @code
     * // My vector.
     * class MyVector
     * {
     * public:
     *   // Copy constructor.
     *   MyVector(const MyVector&);
     *
     *   // Move constructor.
     *   MyVector(MyVector&&);
     *
     *   // Copy assignable x=y.
     *   MyVector& operator=(const MyVector& y);
     *
     *   // In-place summation x+=y.
     *   MyVector& operator+=(const MyVector& y);
     *
     *   // In-place subtraction x-=y.
     *   MyVector& operator-=(const MyVector& y);
     *
     *   // In-place multiplication x*=a.
     *   MyVector& operator*=(double a);
     *
     *   // Negation -y.
     *   MyVector operator-(const MyVector& y);
     *
     *   // Equality comparison x==y.
     *   bool operator==(const MyVector& y);
     *
     *   // Apply as dual element x(y).
     *   double operator()(const MyVector& y);
     *
     *   // Access pointer to underlying function space.
     *   const VectorSpace* space() const;
     * };
     * @endcode
     *
     * See @ref VectorAnchor "::VSA::Vector".
     */
    using VectorConcept =
    boost::mpl::vector<
      ConceptBase ,
      boost::type_erasure::assignable<> ,
      boost::type_erasure::multiply_assignable< boost::type_erasure::_self , double > ,
      boost::type_erasure::add_assignable<> ,
      boost::type_erasure::subtract_assignable<> ,
      boost::type_erasure::negatable<> ,
      boost::type_erasure::equality_comparable<> ,
      boost::type_erasure::callable<double(const boost::type_erasure::_self&),
                                    const boost::type_erasure::_self> ,
      has_space<const VectorSpace*(),
                const boost::type_erasure::_self>
    >;
  }
}

#endif // ALGORITHM_VECTOR_CONCEPT_HH
