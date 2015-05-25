#ifndef ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_ELEMENT_HH

#include <memory>
#include <vector>

#include "../../Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
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
      v_.zeros();
    }

    std::unique_ptr<AbstractFunctionSpaceElement> clone() const final override
    {
      return std::make_unique<VectorSpaceElement>(v_,this->getSpace());
    }

    void print(std::ostream& os) const final override
    {
      os << v_; // todo generalize output
    }


    VectorSpaceElement& operator+=(const AbstractFunctionSpaceElement& y) final override
    {
      v_ += dynamic_cast<const VectorSpaceElement&>(y).v_; // todo generalize
      return *this;
    }

    VectorSpaceElement& operator-=(const AbstractFunctionSpaceElement& y) final override
    {
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
      return v_[i]; // todo generalize access
    }

    const double& coefficient(unsigned i) const final override
    {
      return v_[i]; // todo generalize access
    }

    unsigned size() const
    {
      return v_.size(); // todo generalize
    }

  private:
    friend class L2Product;
    Vector v_;
  };
}

#endif // ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_ELEMENT_HH
