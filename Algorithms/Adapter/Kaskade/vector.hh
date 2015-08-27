#ifndef ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH

#include "Util/Base/vectorBase.hh"
#include "Util/cast.hh"

#include "util.hh"

namespace Algorithm
{  
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace Kaskade
  {
    /// \cond
    template <class> class VectorCreator;
    /// \endcond

    /**
     * @ingroup KaskadeGroup
     * @brief Vector implementation for %Kaskade 7 (single space).
     */
    template <class Description>
    class Vector :
        public VectorBase< Vector<Description> > ,
        public SupportedOperatorBase< Vector<Description> >
    {
      using VectorImpl = typename Description::template CoefficientVectorRepresentation<>::type;
      using Variable = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Variables,0>::type> >;
      using Space = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Spaces,Variable::spaceIndex>::type> >;

    public:
      /// Construct vector from underlying vector space.
      Vector(const ::Algorithm::VectorSpace& space_)
        : VectorBase< Vector<Description> >(space_),
          spaces_(&cast_ref< VectorCreator<Description> >(space_.impl()).impl()),
          v_( Description::template CoefficientVectorRepresentation<>::init( spaces_ ))
      {}

      /// Assign from coefficient vector of %Kaskade 7.
      Vector& operator=(const VectorImpl& v)
      {
        v_ = v;
        return *this;
      }

      /// Access coefficient vector.
      VectorImpl& impl()
      {
        return v_;
      }

      /// Access coefficient vector.
      const VectorImpl& impl() const
      {
        return v_;
      }

      /// Apply as dual element.
      double operator()(const Vector& y) const
      {
        return impl() * y.impl();
      }

      //      Vector& axpy(double a, const AbstractVector& y)
      //      {
      //        v_.axpy(a,castTo< Vector<Description> >(y).v_);
      //        return *this;
      //      }

    private:
      typename Description::Spaces spaces_;
      VectorImpl v_;
    };
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH
