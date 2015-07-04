#ifndef ALGORITHM_REAL_NUMBERS_REAL_HH
#define ALGORITHM_REAL_NUMBERS_REAL_HH

#include <memory>

#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  namespace Interface { class AbstractBanachSpace; }

  /// Real number.
  class Real : public Interface::AbstractFunctionSpaceElement
  {
  public:
    /**
     * @brief Construct real number with value x.
     * @param x initial value
     * @param space associated function space (RealSpace)
     */
    Real(double x, const Interface::AbstractBanachSpace& space);

    /**
     * @brief Construct real number with initial value 0.
     * @param space associated function space (RealSpace)
     */
    explicit Real(const Interface::AbstractBanachSpace& space);

    /**
     * @brief copy data to y.
     */
    void copyTo(Interface::AbstractFunctionSpaceElement& y) const final override;

    /// Print to os.
    void print(std::ostream& os) const final override;

    /// Assignment.
    Real& operator=(const Interface::AbstractFunctionSpaceElement& y) final override;

    /// In-place summation.
    Real& operator+=(const Interface::AbstractFunctionSpaceElement& y) final override;

    /// In-place subtraction.
    Real& operator-=(const Interface::AbstractFunctionSpaceElement& y) final override;

    /// In-place multiplication.
    Real& operator*=(double) final override;

    /// Get -x.
    std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator- () const final override;

    /// Access value.
    double& coefficient(unsigned) final override;

    /// Access value.
    const double& coefficient(unsigned) const final override;

    /// Number of entries in coefficient vector (=1).
    unsigned size() const final override;

    operator double() const
    {
      return x_;
    }

  private:
    /**
     * @brief Get a copy of this real number.
     */
    Real* cloneImpl() const final override;

    /// Apply as dual element.
    double applyAsDualTo(const Interface::AbstractFunctionSpaceElement& y) const final override;

    friend class RealProduct;
    double x_;
  };

  bool isRealElement(const Interface::AbstractFunctionSpaceElement& x);
}

#endif // ALGORITHM_REAL_NUMBERS_REAL_HH
