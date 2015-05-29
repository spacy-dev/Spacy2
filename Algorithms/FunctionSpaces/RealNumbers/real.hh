#ifndef ALGORITHM_REAL_NUMBERS_REAL_HH
#define ALGORITHM_REAL_NUMBERS_REAL_HH

#include <memory>
#include <vector>

#include "../../Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
   class AbstractBanachSpace;

  /// Real number.
  class Real : public AbstractFunctionSpaceElement
  {
  public:
    /**
     * @brief Construct real number with value x.
     * @param x initial value
     * @param space associated function space (RealSpace)
     */
    Real(double x, const AbstractBanachSpace& space);

    /**
     * @brief Construct real number with initial value 0.
     * @param space associated function space (RealSpace)
     */
    explicit Real(const AbstractBanachSpace& space);

    /**
     * @brief copy data to y.
     */
    void copyTo(AbstractFunctionSpaceElement& y) const final override;

    /**
     * @brief Get a copy of this real number.
     */
    std::unique_ptr<AbstractFunctionSpaceElement> clone() const final override;

    /// Print to os.
    void print(std::ostream& os) const final override;

    /// In-place summation.
    Real& operator+=(const AbstractFunctionSpaceElement& y) final override;

    /// In-place subtraction.
    Real& operator-=(const AbstractFunctionSpaceElement& y) final override;

    /// In-place multiplication.
    Real& operator*=(double) final override;

    /// Get -x.
    std::unique_ptr<AbstractFunctionSpaceElement> operator- () const final override;

    /// Access value.
    double& coefficient(unsigned) final override;

    /// Access value.
    const double& coefficient(unsigned) const final override;

    /// Number of entries in coefficient vector (=1).
    unsigned size() const final override;

    operator double() const
    {
      return x_[0];
    }

  private:
    friend class RealProduct;
    std::vector<double> x_;
  };

  bool isRealElement(const AbstractFunctionSpaceElement& x);
}

#endif // ALGORITHM_REAL_NUMBERS_REAL_HH
