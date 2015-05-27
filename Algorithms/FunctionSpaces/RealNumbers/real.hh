#ifndef ALGORITHM_REAL_NUMBERS_REAL_HH
#define ALGORITHM_REAL_NUMBERS_REAL_HH

#include <memory>
#include <vector>

#include "../../Interface/abstractFunctionSpaceElement.hh"
#include "../../Interface/abstractHilbertSpace.hh"

namespace Algorithm
{
  /// Real number.
  class Real : public AbstractFunctionSpaceElement
  {
  public:
    /**
     * @brief Construct real number with value x.
     * @param x initial value
     * @param space associated function space (RealSpace)
     */
    Real(double x, const AbstractHilbertSpace& space);

    /**
     * @brief Construct real number with initial value 0.
     * @param space associated function space (RealSpace)
     */
    explicit Real(const AbstractHilbertSpace& space);

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

  private:
    friend class RealProduct;
    std::vector<double> x_;
  };
}

#endif // ALGORITHM_REAL_NUMBERS_REAL_HH
