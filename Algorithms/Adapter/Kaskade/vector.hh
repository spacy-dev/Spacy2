#ifndef ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH

#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/Mixins/impl.hh"
#include "Util/castTo.hh"

namespace Algorithm
{  
  namespace Kaskade
  {
    template <class> class FunctionSpace;

    template <class Description>
    class Vector : public Interface::AbstractFunctionSpaceElement
    {
      using VectorImpl = typename Description::template CoefficientVectorRepresentation<>::type;
      using Variable = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Variables,0>::type> >;
      using Space = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Spaces,Variable::spaceIndex>::type> >;

    public:
      Vector(const Interface::AbstractFunctionSpace& space_)
        : Interface::AbstractFunctionSpaceElement(space_),
          spaces_(&castTo< FunctionSpace<Description> >(space()).impl()),
          v_( Description::template CoefficientVectorRepresentation<>::init( spaces_ ))
      {}

      Vector(const Interface::AbstractFunctionSpace& space_, const VectorImpl& v)
        : Interface::AbstractFunctionSpaceElement(space_),
          spaces_(&castTo< FunctionSpace<Description> >(space()).impl()),
          v_(v)
      {}

      void copyTo(Interface::AbstractFunctionSpaceElement& y) const override
      {
        castTo< Vector<Description> >(y).v_ = v_;
      }

      void print(std::ostream& os) const final override
      {
        //os << v_; // todo generalize output
      }

      Vector& operator=(const VectorImpl& v)
      {
        v_ = v;
        return *this;
      }

      Vector& operator=(const Interface::AbstractFunctionSpaceElement& y) final override
      {
        v_ = castTo< Vector<Description> >(y).v_;
        return *this;
      }

      Vector& operator+=(const Interface::AbstractFunctionSpaceElement& y) final override
      {
        v_ += castTo< Vector<Description> >(y).v_;
        return *this;
      }

      Vector& operator-=(const Interface::AbstractFunctionSpaceElement& y) final override
      {
        v_ -= castTo< Vector<Description> >(y).v_;
        return *this;
      }

      Vector& operator*=(double a) final override
      {
        v_ *= a;
        return *this;
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator- () const final override
      {
        auto v = std::make_unique<Vector<Description> >(space(),v_);
        *v *= -1;
        return std::move(v);
      }

      double& coefficient(unsigned i) final override
      {
        return boost::fusion::at_c<0>(v_.data)[i/Variable::m][i%Variable::m];
      }

      const double& coefficient(unsigned i) const final override
      {
        return boost::fusion::at_c<0>(v_.data)[i/Variable::m][i%Variable::m];
      }

      unsigned size() const
      {
        return v_.dim();
      }

      VectorImpl& impl()
      {
        return v_;
      }

      const VectorImpl& impl() const
      {
        return v_;
      }

    private:
      double applyAsDualTo(const Interface::AbstractFunctionSpaceElement& y) const final override
      {
        return castTo< Vector<Description> >(y).v_ * v_;
      }


      Vector* cloneImpl() const final override
      {
        return new Vector(*this);
      }

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
        static void apply(const ProductSpaceElement& x, ::Kaskade::VariableSet<Description>& y)
        {
          if( ( x.space().isPrimalSubSpaceId(i) && x.isPrimalEnabled() ) ||
              ( x.space().isDualSubSpaceId(i) && x.isDualEnabled() ) )
            boost::fusion::at_c<i>(y.data).coefficients() = boost::fusion::at_c<0>(castTo< Vector< ExtractDescription_t<Description,i> > >(x.variable(i)).impl().data);
          Copy<i+1,n>::apply(x,y);
        }

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const ProductSpaceElement& x, CoeffVector& y)
        {
          if( ( x.space().isPrimalSubSpaceId(i) && x.isPrimalEnabled() ) ||
              ( x.space().isDualSubSpaceId(i) && x.isDualEnabled() ) )
            boost::fusion::at_c<i>(y.data) = boost::fusion::at_c<0>(castTo< Vector< ExtractDescription_t<Description,i> > >(x.variable(i)).impl().data);
          Copy<i+1,n>::template toCoefficientVector<Description>(x,y);
        }

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector& x, ProductSpaceElement& y)
        {
          if( ( y.space().isPrimalSubSpaceId(i) && y.isPrimalEnabled() ) ||
              ( y.space().isDualSubSpaceId(i) && y.isDualEnabled() ) )
            boost::fusion::at_c<0>(castTo< Vector< ExtractDescription_t<Description,i> > >(y.variable(i)).impl().data) = boost::fusion::at_c<i>(x.data);
          Copy<i+1,n>::template fromCoefficientVector<Description>(x,y);
        }
      };

      template <int n>
      struct Copy<0,n>
      {
        template <class Description>
        static void apply(const Interface::AbstractFunctionSpaceElement& x, ::Kaskade::VariableSet<Description>& y)
        {
          if( is< Vector< Description > >(x) )
          {
            boost::fusion::at_c<0>(y.data).coefficients() = boost::fusion::at_c<0>(castTo< Vector< Description > >(x).impl().data);
            return;
          }

          if( is<ProductSpaceElement>(x))
          {
            const auto& x_ = castTo<ProductSpaceElement>(x);
            if( ( x_.space().isPrimalSubSpaceId(0) && x_.isPrimalEnabled() ) ||
                ( x_.space().isDualSubSpaceId(0) && x_.isDualEnabled() ) )
            boost::fusion::at_c<0>(y.data).coefficients() = boost::fusion::at_c<0>(castTo< Vector< ExtractDescription_t<Description,0> > >(x_.variable(0)).impl().data);
            Copy<1,n>::apply(x_,y);
            return;
          }

          assert(false);
        }

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const Interface::AbstractFunctionSpaceElement& x, CoeffVector& y)
        {
          if( is< Vector< Description > >(x) )
          {
            boost::fusion::at_c<0>(y.data) = boost::fusion::at_c<0>(castTo< Vector< Description > >(x).impl().data);
            return;
          }

          if( is<ProductSpaceElement>(x))
          {
            const auto& x_ = castTo<ProductSpaceElement>(x);
            if( ( x_.space().isPrimalSubSpaceId(0) && x_.isPrimalEnabled() ) ||
                ( x_.space().isDualSubSpaceId(0) && x_.isDualEnabled() ) )
              boost::fusion::at_c<0>(y.data) = boost::fusion::at_c<0>(castTo< Vector< ExtractDescription_t<Description,0> > >(x_.variable(0)).impl().data);
            Copy<1,n>::template toCoefficientVector<Description>(x_,y);
            return;
          }

          assert(false);
        }

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector& x, Interface::AbstractFunctionSpaceElement& y)
        {
          if( is< Vector< Description > >(y) )
          {
            boost::fusion::at_c<0>(castTo< Vector< Description > >(y).impl().data) = boost::fusion::at_c<0>(x.data);
            return;
          }

          if( is<ProductSpaceElement>(y))
          {
            auto& y_ = castTo<ProductSpaceElement>(y);
            if( ( y_.space().isPrimalSubSpaceId(0) && y_.isPrimalEnabled() ) ||
                ( y_.space().isDualSubSpaceId(0) && y_.isDualEnabled() ) )
              boost::fusion::at_c<0>(castTo< Vector< ExtractDescription_t<Description,0> > >(y_.variable(0)).impl().data) = boost::fusion::at_c<0>(x.data);
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
        static void apply(const Interface::AbstractFunctionSpaceElement&, ::Kaskade::VariableSet<Description>&)
        {}

        template <class Description, class CoeffVector>
        static void toCoefficientVector(const Interface::AbstractFunctionSpaceElement&, CoeffVector&)
        {}

        template <class Description, class CoeffVector>
        static void fromCoefficientVector(const CoeffVector&, Interface::AbstractFunctionSpaceElement&)
        {}
      };
    }

    template <class Description>
    void copy(const Interface::AbstractFunctionSpaceElement& x, ::Kaskade::VariableSet<Description>& y)
    {
      Detail::Copy<0,Description::noOfVariables>::apply(x,y);
    }

    template <class Description>
    void copyToCoefficientVector(const Interface::AbstractFunctionSpaceElement& x, typename Description::template CoefficientVectorRepresentation<>::type& y)
    {
      Detail::Copy<0,Description::noOfVariables>::template toCoefficientVector<Description>(x,y);
    }


    template <class Description>
    void copyFromCoefficientVector(const typename Description::template CoefficientVectorRepresentation<>::type& x,
                                 Interface::AbstractFunctionSpaceElement& y)
    {
      Detail::Copy<0,Description::noOfVariables>::template fromCoefficientVector<Description>(x,y);
    }
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH
