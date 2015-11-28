#ifndef SPACY_VECTOR_HH
#define SPACY_VECTOR_HH

#include <type_traits>
#include <boost/type_erasure/any.hpp>

#include "Spacy/Util/Concepts/vectorConcept.hh"
#include "Spacy/Util/Mixins/impl.hh"
#include "Spacy/Util/cast.hh"
#include "Spacy/Util/typeErasedStorage.hh"

namespace Spacy
{
  /// @cond
  class Real;
  class VectorSpace;
  /// @endcond

  /** \addtogroup SpacyGroup
   * @{
   */

  /**
   * @anchor VectorAnchor
   * @brief Vector class.  Can store objects that satisfy the requirements of \ref VectorConceptAnchor "VectorConcept".
   */
  using AnyVector = boost::type_erasure::any< Concepts::VectorConcept >;

//  class Vector : public TypeErasedStorage
//  {
//  public:
//    Vector() = default;

//    /// Construct from vector implementation.
//    template <class Impl,
//              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,Vector>::value> >
//    Vector(Impl&& impl)
//      : TypeErasedStorage(std::forward<Impl>(impl)),
//        add_( [this](const Vector& y) -> Vector&
//        {
//          (*Spacy::target<Impl>(*this)) += (*Spacy::target<Impl>(y));
//          return *this;
//        } ),
//        subtract_( [this](const Vector& y) -> Vector&
//        {
//          (*Spacy::target<Impl>(*this)) -= (*Spacy::target<Impl>(y));
//          return *this;
//        } ),
//        multiply_( [this](double a) -> Vector&
//        {
//          (*Spacy::target<Impl>(*this)) *= a;
//          return *this;
//        } ),
//        negate_( [this]() -> Vector
//        {
//          return -(*Spacy::target<Impl>(*this));
//        } ),
//        compare_( [this](const Vector& y)
//        {
//          return (*Spacy::target<Impl>(*this)) == (*Spacy::target<Impl>(y));
//        } ),
//        asDual_( [this](const Vector& y) -> Vector { return (*Spacy::target<Impl>(*this))( *Spacy::target<Impl>(y) ); } ),
//        //asDual_( std::cref(*Spacy::target<Impl>(*this)) ),
//        space_( std::bind(&std::decay_t<Impl>::space, Spacy::target<Impl>(*this) ) )
//    {}

//    /// Assign from vector implementation.
//    template <class Impl,
//              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,Vector>::value> >
//    Vector& operator=(Impl&& impl)
//    {
//      TypeErasedStorage::operator =(std::forward<Impl>(impl));
//      add_ = [this](const Vector& y) -> Vector&
//      {
//        (*Spacy::target<Impl>(*this)) += (*Spacy::target<Impl>(y));
//        return *this;
//      };
//      subtract_ = [this](const Vector& y) -> Vector&
//      {
//        (*Spacy::target<Impl>(*this)) -= (*Spacy::target<Impl>(y));
//        return *this;
//      };
//      multiply_ = [this](double a) -> Vector&
//      {
//        (*Spacy::target<Impl>(*this)) *= a;
//        return *this;
//      };
//      negate_ = [this]() -> Vector
//      {
//        return -(*Spacy::target<Impl>(*this));
//      };
//      compare_ = [this](const Vector& y)
//      {
//        return (*Spacy::target<Impl>(*this)) == (*Spacy::target<Impl>(y));
//      };
//      asDual_ = [this](const Vector& y) -> Vector { return (*Spacy::target<Impl>(*this))( *Spacy::target<Impl>(y) ); };
////      asDual_ = std::cref(*Spacy::target<Impl>(*this));
//      space_ = std::bind(&std::decay_t<Impl>::space, Spacy::target<Impl>(*this) );
//      return *this;
//    }

//    /// In-place summation \f$ x += y \f$.
//    Vector& operator+=(const Vector& y);

//    /// In-place subraction \f$ x -= y \f$.
//    Vector& operator-=(const Vector& y);

//    /// In-place multiplication \f$ x *= a \f$.
//    Vector& operator*=(double a);

//    /// Negation \f$-x\f$
//    Vector operator-() const;

//    /// Equality comparison (with respect to accuracy specified in the underlying function space).
//    bool operator==(const Vector& y) const;

//    /// Apply as dual element \f$x(y)\f$.
//    Vector operator()(const Vector& y) const;

//    /// Access underlying vector space \f$X\f$.
//    const VectorSpace& space() const;

//    /// Check if an implementation has been assigned.
//    operator bool() const;

//  private:
//    std::function<Vector&(const Vector&)> add_;
//    std::function<Vector&(const Vector&)> subtract_;
//    std::function<Vector&(double)> multiply_;
//    std::function<Vector()> negate_;
//    std::function<bool(const Vector&)> compare_;
//    std::function<Vector(const Vector&)> asDual_;
//    std::function<const VectorSpace*()> space_;
//  };

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
//    return Spacy::target<ToType>(v) != nullptr;
    return is<ToType>(v.impl());
  }

  template <class ToType>
  const ToType& cast_ref(const Spacy::Vector& v)
  {
//    return *Spacy::target<ToType>(v);
    return cast_ref<ToType>(v.impl());
  }

  template <class ToType>
  ToType& cast_ref(Spacy::Vector& v)
  {
//    return *Spacy::target<ToType>(v);
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
