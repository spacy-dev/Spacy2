#ifndef ALGORITHM_OPERATOR_BASE_HH
#define ALGORITHM_OPERATOR_BASE_HH

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  /**
   * @brief Base class for operators \f$A:\ X\rightarrow Y\f$, between function spaces \f$X\f$ and \f$Y\f$.
   */
  class OperatorBase
  {
  public:
    /**
     * @brief Constructor.
     * @param domain domain space \f$X\f$.
     * @param range range space \f$Y\f$.
     */
    OperatorBase(const VectorSpace& domain, const VectorSpace& range);

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;

  private:
    const VectorSpace &domain_, &range_;
  };
}

#endif // ALGORITHM_OPERATOR_BASE_HH
