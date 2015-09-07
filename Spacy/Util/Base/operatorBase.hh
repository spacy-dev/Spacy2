#ifndef SPACY_OPERATOR_BASE_HH
#define SPACY_OPERATOR_BASE_HH

namespace Spacy
{
  /// \cond
  class VectorSpace;
  void checkSpaceCompatibility(const VectorSpace&,const VectorSpace&);
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

    /// Move constructor.
    OperatorBase(OperatorBase&&) = default;

    /// Copy constructor.
    OperatorBase(const OperatorBase&) = default;

    /// Move assignment.
    OperatorBase& operator=(OperatorBase&& B)
    {
      checkSpaceCompatibility(range(),B.range());
      checkSpaceCompatibility(domain(),B.domain());
      return *this;
    }

    /// Move assignment.
    OperatorBase& operator=(const OperatorBase& B)
    {
      checkSpaceCompatibility(range(),B.range());
      checkSpaceCompatibility(domain(),B.domain());
      return *this;
    }

    /**
     * @brief Access domain space \f$X\f$.
     * @return domain space \f$X\f$.
     */
    const VectorSpace& domain() const;

    /**
     * @brief Access range space \f$Y\f$.
     * @return range space \f$Y\f$.
     */
    const VectorSpace& range() const;

  private:
    const VectorSpace &domain_, &range_;
  };
}

#endif // SPACY_OPERATOR_BASE_HH