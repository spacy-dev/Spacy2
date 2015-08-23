#ifndef ALGORITHM_VECTOR_HH
#define ALGORITHM_VECTOR_HH

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

BOOST_TYPE_ERASURE_MEMBER( (has_norm) , norm , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_scalarProduct) , scalarProduct , 1 )

namespace Algorithm
{
  namespace te = boost::type_erasure;

  using VectorConcept =
  boost::mpl::vector<
    ConceptBase ,
    te::assignable<> ,
    te::multiply_assignable< te::_self , double > ,
    te::add_assignable<> ,
    te::subtract_assignable<> ,
    te::negatable<> ,
    te::equality_comparable<> ,
    te::callable<double(const te::_self&), const te::_self> ,
    has_spaceIndex<unsigned(), const te::_self> ,
    has_isAdmissible<bool(), const te::_self> ,
    has_norm<double(), const te::_self> ,
    has_scalarProduct<double(const te::_self&), const te::_self>
  >;

  using Vector = te::any<VectorConcept >;

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
   * @brief Compute \f$z=x*y=(x,y)\f$.
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
