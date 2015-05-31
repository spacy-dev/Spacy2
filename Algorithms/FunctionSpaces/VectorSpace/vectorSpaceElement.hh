#ifndef ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_ELEMENT_HH

#include <memory>
#include <vector>
#include <utility>

#include "../../Interface/abstractFunctionSpaceElement.hh"
#include "../../Util/invalidargumentexception.hh"
#include "../../Util/voider.hh"

namespace Algorithm
{  
  namespace VectorSpaceDetail
  {
    template <class Vector>
    using TryCall_Coefficients = decltype( std::declval<Vector>().coefficients() );

    template <class Vector, class = void> struct HasCall_Coefficients : std::false_type {};
    template <class Vector> struct HasCall_Coefficients<Vector, void_t<TryCall_Coefficients<Vector> > > : std::true_type {};

    template <class Vector>
    using TryCall_Zeros = decltype( std::declval<Vector>().zeros() );

    template <class Vector, class = void> struct HasCall_Zeros : std::false_type {};
    template <class Vector> struct HasCall_Zeros<Vector, void_t<TryCall_Zeros<Vector> > > : std::true_type {};


    template <class Vector, bool>
    struct MakeZero
    {
      static void apply(Vector& v)
      {
        v = 0.;
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

    template <class Vector>
    struct AccessCoefficients<Vector,true>
    {
      static double& apply(Vector& v, unsigned i) { return v.coefficients()[i]; }

      static const double& apply(const Vector& v, unsigned i) { return v.coefficients()[i]; }
    };

    template <class Vector>
    double& accessCoefficients(Vector& v, unsigned i)
    {
      return AccessCoefficients< Vector , HasCall_Coefficients<Vector>::value >::apply(v,i);
    }

    template <class Vector>
    const double& accessCoefficients(const Vector& v, unsigned i)
    {
      return AccessCoefficients< Vector , HasCall_Coefficients<Vector>::value >::apply(v,i);
    }
  }

  template <class> bool isVectorSpaceElement(const AbstractFunctionSpaceElement&);

  template <class Vector>
  class VectorSpaceElement : public AbstractFunctionSpaceElement
  {
  public:
    VectorSpaceElement(const Vector& v, const AbstractBanachSpace& space)
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

    std::unique_ptr<AbstractFunctionSpaceElement> clone() const final override
    {
      return std::make_unique<VectorSpaceElement>(v_,getSpace());
    }

    void print(std::ostream& os) const final override
    {
      os << v_; // todo generalize output
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
      return std::make_unique<VectorSpaceElement>(-v_,this->getSpace());
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
      return v_.size(); // todo generalize
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
