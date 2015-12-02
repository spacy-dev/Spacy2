#ifndef SPACY_OPERATOR_BASE_HH
#define SPACY_OPERATOR_BASE_HH

namespace Spacy
{
  /// \cond
  class VectorSpace;
  /// \endcond

  /// Base class for operators \f$A:\ X\rightarrow Y\f$, between function spaces \f$X\f$ and \f$Y\f$.
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
    OperatorBase& operator=(OperatorBase&& B);

    /// Move assignment.
    OperatorBase& operator=(const OperatorBase& B);

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;

  private:
    const VectorSpace &domain_, &range_;
  };
}

#endif // SPACY_OPERATOR_BASE_HH
