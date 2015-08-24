#ifndef ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH

#include "VectorSpaces/productSpace.hh"
#include "Util/Base/vectorBase.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/Mixins/impl.hh"
#include "Util/cast.hh"

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

    template <class Description>
    class Vector :
        public VectorBase< Vector<Description> > ,
        public SupportedOperatorBase< Vector<Description> >
    {
      using VectorImpl = typename Description::template CoefficientVectorRepresentation<>::type;
      using Variable = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Variables,0>::type> >;
      using Space = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Spaces,Variable::spaceIndex>::type> >;

    public:
      Vector(const ::Algorithm::VectorSpace& space_)
        : VectorBase< Vector<Description> >(space_),
          spaces_(&cast_ref< VectorCreator<Description> >(space_.impl()).impl()),
          v_( Description::template CoefficientVectorRepresentation<>::init( spaces_ ))
      {}

      Vector(const ::Algorithm::VectorSpace& space_, const VectorImpl& v)
        : VectorBase< Vector<Description> >(space_),
          spaces_(&cast_ref< VectorCreator<Description> >(space_.impl()).impl()),
          v_(v)
      {}

      Vector& operator=(const VectorImpl& v)
      {
        v_ = v;
        return *this;
      }

      VectorImpl& impl()
      {
        return v_;
      }

      const VectorImpl& impl() const
      {
        return v_;
      }

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

    namespace Detail
    {
      template <class Description, int i>
      struct ExtractDescription
      {
        using Variables = typename Description::Variables;
        using Spaces = typename Description::Spaces;
        using Variable = std::decay_t<typename boost::fusion::result_of::value_at_c<Variables,i>::type>;
        using SpacePtr = std::remove_reference_t<typename boost::fusion::result_of::value_at_c<Spaces,Variable::spaceIndex>::type>;
        using SpaceShiftedVariable = ::Kaskade::Variable< ::Kaskade::VariableId<Variable::id > , ::Kaskade::SpaceIndex<0> , ::Kaskade::Components<Variable::m> >;
        using type = ::Kaskade::VariableSetDescription< boost::fusion::vector< SpacePtr > , boost::fusion::vector< SpaceShiftedVariable > >;
      };

      template <class Description, int i>
      using ExtractDescription_t = typename ExtractDescription<Description,i>::type;


      template <int i, int n>
      struct Copy
      {
        template <class Description>
        static void apply(const ProductSpace::Vector& x, ::Kaskade::VariableSet<Description>& y)
        {
          if( ( x.creator().isPrimalSubSpaceId(i) && x.isPrimalEnabled() ) ||
              ( x.creator().isDualSubSpaceId(i) && x.isDualEnabled() ) )
            boost::fusion::at_c<i>(y.data).coefficients() = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(x.variable(i)).impl().data);
          Copy<i+1,n>::apply(x,y);
        }

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const ProductSpace::Vector& x, CoeffVector& y)
        {
          if( ( x.creator().isPrimalSubSpaceId(i) && x.isPrimalEnabled() ) ||
              ( x.creator().isDualSubSpaceId(i) && x.isDualEnabled() ) )
            boost::fusion::at_c<i>(y.data) = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(x.variable(i)).impl().data);
          Copy<i+1,n>::template toCoefficientVector<Description>(x,y);
        }

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector& x, ProductSpace::Vector& y)
        {
          if( ( y.creator().isPrimalSubSpaceId(i) && y.isPrimalEnabled() ) ||
              ( y.creator().isDualSubSpaceId(i) && y.isDualEnabled() ) )
            boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,i> > >(y.variable(i)).impl().data) = boost::fusion::at_c<i>(x.data);
          Copy<i+1,n>::template fromCoefficientVector<Description>(x,y);
        }
      };

      template <int n>
      struct Copy<0,n>
      {
        template <class Description>
        static void apply(const ::Algorithm::Vector& x, ::Kaskade::VariableSet<Description>& y)
        {
          if( isAny< Vector< Description > >(x) )
          {
            boost::fusion::at_c<0>(y.data).coefficients() = boost::fusion::at_c<0>(cast_ref< Vector< Description > >(x).impl().data);
            return;
          }

          if( isAny<ProductSpace::Vector>(x))
          {
            const auto& x_ = cast_ref<ProductSpace::Vector>(x);
            if( ( x_.creator().isPrimalSubSpaceId(0) && x_.isPrimalEnabled() ) ||
                ( x_.creator().isDualSubSpaceId(0) && x_.isDualEnabled() ) )
            boost::fusion::at_c<0>(y.data).coefficients() = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,0> > >(x_.variable(0)).impl().data);
            Copy<1,n>::apply(x_,y);
            return;
          }

          assert(false);
        }

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const ::Algorithm::Vector& x, CoeffVector& y)
        {
          if( isAny< Vector< Description > >(x) )
          {
            boost::fusion::at_c<0>(y.data) = boost::fusion::at_c<0>(cast_ref< Vector< Description > >(x).impl().data);
            return;
          }

          if( isAny<ProductSpace::Vector>(x))
          {
            const auto& x_ = cast_ref<ProductSpace::Vector>(x);
            if( ( x_.creator().isPrimalSubSpaceId(0) && x_.isPrimalEnabled() ) ||
                ( x_.creator().isDualSubSpaceId(0) && x_.isDualEnabled() ) )
              boost::fusion::at_c<0>(y.data) = boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,0> > >(x_.variable(0)).impl().data);
            Copy<1,n>::template toCoefficientVector<Description>(x_,y);
            return;
          }

          assert(false);
        }

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector& x, ::Algorithm::Vector& y)
        {
          if( isAny< Vector< Description > >(y) )
          {
            boost::fusion::at_c<0>(cast_ref< Vector< Description > >(y).impl().data) = boost::fusion::at_c<0>(x.data);
            return;
          }

          if( isAny<ProductSpace::Vector>(y))
          {
            auto& y_ = cast_ref<ProductSpace::Vector>(y);
            if( ( y_.creator().isPrimalSubSpaceId(0) && y_.isPrimalEnabled() ) ||
                ( y_.creator().isDualSubSpaceId(0) && y_.isDualEnabled() ) )
              boost::fusion::at_c<0>(cast_ref< Vector< ExtractDescription_t<Description,0> > >(y_.variable(0)).impl().data) = boost::fusion::at_c<0>(x.data);
            Copy<1,n>::template fromCoefficientVector<Description>(x,y_);
            return;
          }

          assert(false);
        }
      };


      template <int n>
      struct Copy<n,n>
      {
        template <class Description>
        static void apply(const ProductSpace::Vector&, ::Kaskade::VariableSet<Description>&)
        {}

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const ProductSpace::Vector&, CoeffVector&)
        {}

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector&, ProductSpace::Vector&)
        {}
      };
    }

    template <class Description>
    void copy(const ::Algorithm::Vector& x, ::Kaskade::VariableSet<Description>& y)
    {
      Detail::Copy<0,Description::noOfVariables>::apply(x,y);
    }

    template <class Description>
    void copyToCoefficientVector(const ::Algorithm::Vector& x, typename Description::template CoefficientVectorRepresentation<>::type& y)
    {
      Detail::Copy<0,Description::noOfVariables>::template toCoefficientVector<Description>(x,y);
    }


    template <class Description>
    void copyFromCoefficientVector(const typename Description::template CoefficientVectorRepresentation<>::type& x,
                                   ::Algorithm::Vector& y)
    {
      Detail::Copy<0,Description::noOfVariables>::template fromCoefficientVector<Description>(x,y);
    }
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH
