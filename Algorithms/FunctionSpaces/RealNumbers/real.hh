#ifndef ALGORITHM_REAL_NUMBERS_REAL_HH
#define ALGORITHM_REAL_NUMBERS_REAL_HH

#include <memory>
#include <vector>

#include "../../Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  class Real : public AbstractFunctionSpaceElement
  {
  public:
    Real(double x, const AbstractBanachSpace& space);

    explicit Real(const AbstractBanachSpace& space);

    std::unique_ptr<AbstractFunctionSpaceElement> clone() const final override;

    void print(std::ostream&) const final override;


    Real& operator+=(const AbstractFunctionSpaceElement& y) final override;

    Real& operator-=(const AbstractFunctionSpaceElement& y) final override;

    Real& operator*=(double) final override;

    std::unique_ptr<AbstractFunctionSpaceElement> operator- () const final override;

    double& coefficient(unsigned) final override;

    const double& coefficient(unsigned) const final override;

    unsigned size() const final override;

  private:
    friend class RealProduct;
    std::vector<double> x_;
  };
}

#endif // ALGORITHM_REAL_NUMBERS_REAL_HH
