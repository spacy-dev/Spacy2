#ifndef SPACY_VECTOR_BASE_HH
#define SPACY_VECTOR_BASE_HH

#include <string>

namespace Spacy
{
  /// @cond
  class VectorSpace;
  void checkSpaceCompatibility(const VectorSpace& x, const VectorSpace& y);
  /// @endcond


  /**
   * @brief Base class for vector implementations.
   * @tparam Derived derived vector implementation, provided for CRTP
   *
   * Provides access to the underlying vector space and related operations.
   * @see Kaskade::Vector, Fenics::Vector, Rn::Vector
   */
  class VectorBase
  {
  public:
    /**
     * @brief Constructor.
     * @param space underlying vector space
     */
    VectorBase( const VectorSpace& space );

    /// Copy constructor.
    VectorBase(const VectorBase& y);

    /// Move constructor.
    VectorBase(VectorBase&& y) noexcept;

    /// Copy assignment.
    VectorBase& operator=(const VectorBase& y);

    /// Move assignment.
    VectorBase& operator=(VectorBase&& y) noexcept;

    /// Access underlying vector space.
    const VectorSpace& space() const;

  private:
    const VectorSpace& space_;
  };
}

#endif // SPACY_VECTOR_BASE_HH
