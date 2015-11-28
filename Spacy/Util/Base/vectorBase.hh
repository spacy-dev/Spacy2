#ifndef SPACY_VECTOR_BASE_HH
#define SPACY_VECTOR_BASE_HH

namespace Spacy
{
  /// @cond
  class VectorSpace;
  void checkSpaceCompatibility(const VectorSpace* x, const VectorSpace* y);
  /// @endcond


  /**
   * @brief Base class for vector implementations.
   * @tparam Derived derived vector implementation, provided for CRTP
   *
   * Provides access to the underlying vector space and related operations.
   * @see Kaskade::Vector, Fenics::Vector
   */
  class VectorBase
  {
  public:
    /**
     * @brief Constructor.
     * @param space underlying vector space
     */
    VectorBase( const VectorSpace& space );

    /**
     * @brief Copy constructor.
     * @param y object to copy from
     */
    VectorBase(const VectorBase& y);

    /**
     * @brief Move constructor.
     * @param y object to move from
     */
    VectorBase(VectorBase&& y) noexcept;

    /**
     * @brief Copy assignment.
     * @param y object to copy from
     */
    VectorBase& operator=(const VectorBase& y);

    /**
     * @brief Move assignment.
     * @param y object to move from
     */
    VectorBase& operator=(VectorBase&& y) noexcept;

    /**
     * @brief Access underlying vector space.
     * @return underlying vector space
     */
    const VectorSpace* space() const;

  private:
    const VectorSpace& space_;
  };
}

#endif // SPACY_VECTOR_BASE_HH
