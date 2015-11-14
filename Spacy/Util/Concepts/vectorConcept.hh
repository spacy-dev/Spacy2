#ifndef SPACY_VECTOR_CONCEPT_HH
#define SPACY_VECTOR_CONCEPT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/type_erasure/operators.hpp>
#include "conceptBase.hh"
#include "singleConcepts.hh"

namespace Spacy
{
  /// @cond
  class Vector;
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
     *   MyVector operator-(const MyVector& y) const;
     *
     *   // Equality comparison x==y.
     *   bool operator==(const MyVector& y) const;
     *
     *   // Apply as dual element x(y).
     *   Vector operator()(const MyVector& y) const;
     *
     *   // Access pointer to underlying function space.
     *   const VectorSpace* space() const;
     * };
     * @endcode
     *
     * See @ref VectorAnchor "::Spacy::Vector".
     */
    using VectorConcept =
    boost::mpl::vector<
      ConceptBase ,
      boost::type_erasure::assignable<> ,
      boost::type_erasure::multiply_assignable< boost::type_erasure::_self , double > ,
      boost::type_erasure::add_assignable<> ,
      boost::type_erasure::subtract_assignable<> ,
      boost::type_erasure::negatable< const boost::type_erasure::_self > ,
      boost::type_erasure::equality_comparable< > ,
      boost::type_erasure::callable<Vector(const boost::type_erasure::_self&),
                                    const boost::type_erasure::_self> ,
      has_space<const VectorSpace*(),
                const boost::type_erasure::_self>
    >;
  }
}

#endif // SPACY_VECTOR_CONCEPT_HH
