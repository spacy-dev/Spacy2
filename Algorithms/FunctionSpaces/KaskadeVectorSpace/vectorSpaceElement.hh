#ifndef ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH

#include <memory>
#include <vector>
#include <utility>

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/invalidargumentexception.hh"
#include "Util/voider.hh"

namespace Algorithm
{  
  namespace Kaskade
  {
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
      {}

      void copyTo(AbstractFunctionSpaceElement& y) const override
      {
        if( !::Algorithm::Kaskade::isVectorSpaceElement<Vector>(y) ) throw InvalidArgumentException("VectorSpaceElement::operator+=");

        dynamic_cast<VectorSpaceElement<Vector>&>(y).v_ = v_;
      }

      void print(std::ostream& os) const final override
      {
        //      os << v_; // todo generalize output
      }

      VectorSpaceElement& operator=(const Vector& v)
      {
        v_ = v;
        return *this;
      }

      VectorSpaceElement& operator=(const AbstractFunctionSpaceElement& y) final override
      {
        if( !::Algorithm::Kaskade::isVectorSpaceElement<Vector>(y) ) throw InvalidArgumentException("VectorSpaceElement::operator=");
        v_ = dynamic_cast<const VectorSpaceElement&>(y).v_; // todo generalize
        return *this;
      }

      VectorSpaceElement& operator+=(const AbstractFunctionSpaceElement& y) final override
      {
        if( !::Algorithm::Kaskade::isVectorSpaceElement<Vector>(y) ) throw InvalidArgumentException("VectorSpaceElement::operator+=");
        v_ += dynamic_cast<const VectorSpaceElement&>(y).v_; // todo generalize
        return *this;
      }

      VectorSpaceElement& operator-=(const AbstractFunctionSpaceElement& y) final override
      {
        if( !::Algorithm::Kaskade::isVectorSpaceElement<Vector>(y) ) throw InvalidArgumentException("VectorSpaceElement::operator-=");
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
        auto v = std::make_unique<VectorSpaceElement>(getSpace(),v_);
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
        return v_.dim(); // todo generalize
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

      friend class L2Product;
      Vector v_;
    };

    template <class Vector>
    bool isVectorSpaceElement(const AbstractFunctionSpaceElement& y)
    {
      return dynamic_cast< const ::Algorithm::Kaskade::VectorSpaceElement<Vector>* >(&y) != nullptr;
    }
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH
