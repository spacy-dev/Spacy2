#ifndef ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_ELEMENT_HH

#include <memory>
#include <vector>
#include <utility>

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/voider.hh"

namespace Algorithm
{  
  namespace VectorSpaceDetail
  {
//    template <class Vector>
//    using TryCall_Coefficients = decltype( boost::fusion::at_c<0>(std::declval<Vector>().data) );

//    template <class Vector, class = void> struct HasCall_Coefficients : std::false_type {};
//    template <class Vector> struct HasCall_Coefficients<Vector, void_t<TryCall_Coefficients<Vector> > > : std::true_type {};


    template <class Vector>
    using TryCall_Zeros = decltype( std::declval<Vector>().zeros() );

    template <class Vector, class = void> struct HasCall_Zeros : std::false_type {};
    template <class Vector> struct HasCall_Zeros<Vector, void_t<TryCall_Zeros<Vector> > > : std::true_type {};


    template <class Vector, bool>
    struct MakeZero
    {
      static void apply(Vector& v)
      {
        v *= 0.;
      }
    };

    template <class Vector>
    struct MakeZero<Vector,true>
    {
      static void apply(Vector& v)
      {
        v.zeros();
      }
    };

    template <class Vector>
    void makeZero(Vector &v)
    {
      MakeZero<Vector,HasCall_Zeros<Vector>::value>::apply(v);
    }

    template <class Vector, bool>
    struct AccessCoefficients
    {
      static double& apply(Vector& v, unsigned i) { return v[i]; }

      static const double& apply(const Vector& v, unsigned i) { return v[i]; }
    };

//    template <class Vector>
//    struct AccessCoefficients<Vector,true>
//    {
//      static double& apply(Vector& v, unsigned i) { return boost::fusion::at_c<0>(v.data)[i][0]; }

//      static const double& apply(const Vector& v, unsigned i) { return boost::fusion::at_c<0>(v.data)[i][0]; }
//    };

    template <class Vector>
    double& accessCoefficients(Vector& v, unsigned i)
    {
      return AccessCoefficients< Vector ,/* HasCall_Coefficients<Vector>::value*/false >::apply(v,i);
    }

    template <class Vector>
    const double& accessCoefficients(const Vector& v, unsigned i)
    {
      return AccessCoefficients< Vector , /*HasCall_Coefficients<Vector>::value */false>::apply(v,i);
    }


    template <class Vector>
    using TryCall_Size = decltype( std::declval<Vector>().size() );

    template <class Vector, class = void> struct HasCall_Size : std::false_type {};
    template <class Vector> struct HasCall_Size< Vector , void_t< TryCall_Size<Vector> > > : std::true_type{};

    template <bool>
    struct Size
    {
      template <class Vector>
      static unsigned apply(const Vector& v) { return v.size(); }
    };

    template <> struct Size<false>
    {
      template <class Vector>
      static unsigned apply(const Vector& v) { return v.dim(); }
    };

    template <class Vector>
    unsigned size(const Vector& v)
    {
      return Size< HasCall_Size<Vector>::value >::apply(v);
    }
  }

  template <class> bool isVectorSpaceElement(const AbstractFunctionSpaceElement&);

  template <class Vector>
  class VectorSpaceElement : public AbstractFunctionSpaceElement
  {
  public:
    VectorSpaceElement(const AbstractBanachSpace& space, const Vector& v)
      : AbstractFunctionSpaceElement(space), v_(v)
    {}

    explicit VectorSpaceElement(const AbstractBanachSpace& space)
      : AbstractFunctionSpaceElement(space)// todo: generalize init
    {
      VectorSpaceDetail::makeZero(v_);
    }

    void copyTo(AbstractFunctionSpaceElement& y) const override
    {
      if( !isVectorSpaceElement<Vector>(y) ) throw InvalidArgumentException("VectorSpaceElement::operator+=");

      dynamic_cast<VectorSpaceElement<Vector>&>(y).v_ = v_;
    }
    void print(std::ostream& os) const final override
    {
      os << v_; // todo generalize output
    }

    VectorSpaceElement& operator=(const Vector& v)
    {
      v_ = v;
      return *this;
    }

    VectorSpaceElement& operator=(const AbstractFunctionSpaceElement& y) final override
    {
      if( !isVectorSpaceElement<Vector>(y) ) throw InvalidArgumentException("VectorSpaceElement::operator=");
      v_ = dynamic_cast<const VectorSpaceElement&>(y).v_; // todo generalize
      return *this;
    }


    VectorSpaceElement& operator+=(const AbstractFunctionSpaceElement& y) final override
    {
      if( !isVectorSpaceElement<Vector>(y) ) throw InvalidArgumentException("VectorSpaceElement::operator+=");
      v_ += dynamic_cast<const VectorSpaceElement&>(y).v_; // todo generalize
      return *this;
    }

    VectorSpaceElement& operator-=(const AbstractFunctionSpaceElement& y) final override
    {
      if( !isVectorSpaceElement<Vector>(y) ) throw InvalidArgumentException("VectorSpaceElement::operator-=");
      v_ -= dynamic_cast<const VectorSpaceElement&>(y).v_; // todo generalize
      return *this;
    }

    VectorSpaceElement& operator*=(double a) final override
    {
      v_ *= a; // todo generalize
      return *this;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> operator- () const final override
    {
      auto v = std::make_unique<VectorSpaceElement>(this->getSpace(),v_);
      *v *= -1;
      return std::move(v);
    }

    double& coefficient(unsigned i) final override
    {
      return VectorSpaceDetail::accessCoefficients(v_,i); // todo generalize access
    }

    const double& coefficient(unsigned i) const final override
    {
      return VectorSpaceDetail::accessCoefficients(v_,i); // todo generalize access
    }

    unsigned size() const
    {
      return VectorSpaceDetail::size(v_); // todo generalize
    }

    Vector& impl()
    {
      return v_;
    }

    const Vector& impl() const
    {
      return v_;
    }

  private:    
    VectorSpaceElement* cloneImpl() const final override
    {
      return new VectorSpaceElement(getSpace(),v_);
    }

    double applyAsDualTo(const AbstractFunctionSpaceElement &y) const final override
    {
      const auto& y_ = dynamic_cast<const VectorSpaceElement&>(y);

      return dot( v_ , y_.v_ );
    }

    friend class L2Product;
    Vector v_;
  };

  template <class Vector>
  bool isVectorSpaceElement(const AbstractFunctionSpaceElement& y)
  {
    return dynamic_cast< const VectorSpaceElement<Vector>* >(&y) != nullptr;
  }


}

#endif // ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_ELEMENT_HH
