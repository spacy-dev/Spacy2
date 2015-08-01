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

    template <class VectorImpl>
    class Vector : public Interface::AbstractFunctionSpaceElement , public Mixin::Impl<VectorImpl>
    {
    public:
      template <class Space, class Variable>
      Vector(const HilbertSpace<Space,Variable>& space)
        : Interface::AbstractFunctionSpaceElement(space),
          Mixin::Impl<VectorImpl>( VectorImpl( space.impl() ) )
      {}

      void copyTo(Interface::AbstractFunctionSpaceElement& y) const override
      {

        castTo< Vector<VectorImpl> >(y).impl() = impl();
      }

      void print(std::ostream& os) const final override
      {
        //os << impl(); // todo generalize output
      }

      Vector& operator=(const VectorImpl& v)
      {
        impl() = v;
        return *this;
      }

      Vector& operator=(const Interface::AbstractFunctionSpaceElement& y) final override
      {
        impl() = castTo< Vector<VectorImpl> >(y).impl();
        return *this;
      }

      Vector& operator+=(const Interface::AbstractFunctionSpaceElement& y) final override
      {
        impl() += castTo< Vector<VectorImpl> >(y).impl();
        return *this;
      }

      Vector& operator-=(const Interface::AbstractFunctionSpaceElement& y) final override
      {
        impl() -= castTo< Vector<VectorImpl> >(y).impl();
        return *this;
      }

      Vector& Interface::operator*=(double a) final override
      {
        impl() *= a;
        return *this;
      }

      std::unique_ptr<AbstractFunctionSpaceElement> Interface::operator- () const final override
      {
        auto v = std::make_unique<Vector<VectorImpl>(getSpace(),impl());
        *v *= -1;
        return std::move(v);
      }

      double& coefficient(unsigned i) final override
      {
        return boost::fusion::at_c<0>(impl().data)[i][0];
      }

      const double& coefficient(unsigned i) const final override
      {
        return boost::fusion::at_c<0>(impl().data)[i][0];
      }

      unsigned size() const
      {
        return impl().dim();
      }

    private:
      Vector* cloneImpl() const final override
      {
        return new Vector(*this);
      }
    };
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_KASKADE_VECTOR_SPACE_ELEMENT_HH
