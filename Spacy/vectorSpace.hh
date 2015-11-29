// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_VECTOR_SPACE_HH
#define SPACY_VECTOR_SPACE_HH

#include <functional>
#include <vector>
#include <boost/type_erasure/any.hpp>

#include "Spacy/norm.hh"
#include "Spacy/scalarProduct.hh"

#include "Spacy/Util/Mixins/eps.hh"
#include "Spacy/Util/Mixins/target.hh"
#include "Spacy/vector.hh"

namespace Spacy
{
  /// @cond
  /// Each space gets a unique index, except if defaultIndex is set to true.
  /// In this case it gets the default index 0.
  namespace Detail { static unsigned spaceIndex = 1; }
  /// @endcond

  /** @addtogroup SpacyGroup
   * @{
   */

  /**
   * @brief Vector creator for feeding into VectorSpace.
   *
   * The minimal signature (besides copy and/or move constructor/assignment) of a vector creator is:
   * @code
   * // My vector creator.
   * class MyVectorCreator
   * {
   * public:
   *   // Compute ||x||.
   *   Vector operator()(const VectorSpace* space) const;
   * };
   * @endcode
   */
  using VectorCreator = std::function< Vector(const VectorSpace*) >;


  template <class T>
  T& creator(VectorSpace& space);

  template <class T>
  const T& creator(const VectorSpace& space);

  /**
   * @brief Function space \f$(X,\|\cdot\|)\f$.
   *
   * To construct a function space you need to provide a VectorCreatorAnchor "VectorCreator". It allows to customize the type of vector that is created by the vector space.
   * Possibly, it provides additional information.
   *
   * @see @ref VectorCreatorAnchor "VectorCreator".
   */
  class VectorSpace : public Mixin::Eps
  {
  public:
    VectorSpace() = default;
    /**
     * @brief Construct function space from @ref VectorCreatorAnchor "VectorCreator" and @ref NormAnchor "Norm".
     * @param creator object satisfying the @ref VectorCreatorConceptAnchor "VectorCreatorConcept"
     * @param norm type-erased norm
     * @param defaultIndex if false, then this space gets a unique index, else it gets the default index 0
     *
     * The default index can be used to use different locally defined function spaces together.
     */
    VectorSpace(VectorCreator creator, Norm norm, bool defaultIndex = false);

    /**
     * @brief Move constructor.
     * @param V vector space to move from
     */
    VectorSpace(VectorSpace&& V);

    /**
     * @brief Move assignment.
     * @param V vector space to move from
     */
    VectorSpace& operator=(VectorSpace&& V);

    /// Vector spaces can not copied.
    VectorSpace(const VectorSpace&) = delete;

    /// Vector spaces can not copy-assigned.
    VectorSpace& operator=(const VectorSpace&) = delete;

    /**
     * @brief Change norm of space.
     */
    void setNorm(Norm norm);

    /**
     * @brief Access norm.
     */
    const Norm& norm() const;

    /**
     * @brief Create new vector \f$v=0\f$.
     */
    Vector zeroVector() const;

    /**
     * @brief Access unique index of the function space.
     */
    unsigned index() const;

    /**
     * @brief Change scalar product.
     */
    void setScalarProduct(ScalarProduct sp);

    /**
     * @brief Access scalar product.
     */
    const ScalarProduct& scalarProduct() const;

    /**
     * @brief Access dual space \f$Y=X^*\f$.
     */
    const VectorSpace& dualSpace() const;

    /**
     * @brief Set dual space \f$Y=X^*\f$.
     */
    void setDualSpace(const VectorSpace* Y);

    /**
     * @brief Add space \f$Y\f$ for which this space \f$X\f$ acts as primal space.
     *
     * This is necessary to allow evaluation of \f$x(y)\f$ for \f$ x\in X \f$ and \f$ y\in Y\f$.
     */
    void addDualSpace(const VectorSpace& Y);

    /**
     * @brief Checks whether \f$Y\f$ has been assigned as dual space with respect to this space \f$X\f$.
     */
    bool isPrimalWRT(const VectorSpace& Y) const;

    /**
     * @brief Checks whether this space is a hilbert space.
     */
    bool isHilbertSpace() const;

    /**
     * @brief Restrict vector space.
     * @param f function object that checks if a vector is admissible.
     *
     * With this function constraints such as \f$\det(\nabla\varphi)>0\f$ in nonlinear elasticity can be implemented.
     */
    void setRestriction(std::function<bool(const Vector&)> f);

    /**
     * @brief Checks if vector is admissible. Default implementation always returns true;
     * @return true if x is admissible, else false
     */
    bool isAdmissible(const Vector& x) const;

    VectorCreator& creator();

    const VectorCreator& creator() const;

  private:
    VectorCreator creator_ = VectorCreator{};
    Norm norm_ = {};
    ScalarProduct sp_ = {};
    unsigned index_ = Detail::spaceIndex++;
    std::vector<unsigned> primalSpaces_ = {}, dualSpaces_ = {}; ///< primal and dual spaces with respect to this space
    const VectorSpace* dualSpace_ = nullptr;
    std::function<bool(const Vector&)> restriction_ = [](const Vector&){ return true; };
  };

  template <class T>
  T& creator(VectorSpace& space)
  {
    return *space.creator().template target<T>();
  }

  template <class T>
  const T& creator(const VectorSpace& space)
  {
    return *space.creator().template target<T>();
  }

  /**
   * @brief Construct Banach space.
   * @param creator object satisfying \ref VectorCreatorConceptAnchor "VectorCreatorConcept"
   * @param norm norm satisfying \ref NormConceptAnchor "NormConcept"
   * @return VectorSpace(impl,norm)
   */
  VectorSpace makeBanachSpace(VectorCreator creator, Norm norm);

  /**
   * @brief Construct Hilbert space.
   * @param creator object satisfying \ref VectorCreatorConceptAnchor "VectorCreatorConcept"
   * @param scalarProduct scalar product satisfying \ref ScalarProductConceptAnchor "ScalarProductConcept"
   * @return hilbert space
   */
  VectorSpace makeHilbertSpace(VectorCreator creator, ScalarProduct scalarProduct, bool defaultIndex = false);

  /**
   * @brief Relate function spaces.
   * @param X primal space
   * @param Y dual space
   *
   * Makes \f$X\f$ the primal space of \f$Y\f$ and
   * makes \f$Y\f$ the dual space of \f$X\f$.
   * This admits the evaluation of \f$y(x)\f$ for \f$x\in X\f$ and \f$y\in Y\f$.
   */
  void connect(VectorSpace& X, VectorSpace& Y);

  void connectAsPrimalDualPair(VectorSpace& X, VectorSpace& Y);

  void checkSpaceCompatibility(const VectorSpace& V, const VectorSpace& W);

  void checkSpaceCompatibility(const VectorSpace* V, const VectorSpace* W);
  /** @} */
}
#endif // SPACY_VECTOR_SPACE_HH
