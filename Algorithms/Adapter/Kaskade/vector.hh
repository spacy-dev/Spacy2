#ifndef ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/Mixins/impl.hh"
#include "Util/castTo.hh"

namespace Algorithm
{  
  namespace Kaskade
  {
    template <class,class> class HilbertSpace;

    template <class Description>
    class Vector : public Interface::AbstractFunctionSpaceElement
    {
      using VectorImpl = typename Description::template CoefficientVectorRepresentation<>::type;
      using Space = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Spaces,0>::type> >;
      using Variable = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Variables,0>::type> >;

    public:
      Vector(const Interface::AbstractBanachSpace& space_)
        : Interface::AbstractFunctionSpaceElement(space_),
          spaces_(&castTo< HilbertSpace<Space,Variable> >(space()).impl()),
          v_( Description::template CoefficientVectorRepresentation<>::init( spaces_ ))
      {}

      Vector(const Interface::AbstractBanachSpace& space_, const VectorImpl& v)
        : Interface::AbstractFunctionSpaceElement(space_),
          spaces_(&castTo< HilbertSpace<Space,Variable> >(space()).impl()),
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
        return boost::fusion::at_c<0>(v_.data)[i][0];
      }

      const double& coefficient(unsigned i) const final override
      {
        return boost::fusion::at_c<0>(v_.data)[i][0];
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

    void copy(const AbstractFunctionSpaceElement& x, )
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH
