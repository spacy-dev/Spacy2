#ifndef ALGORITHM_VECTOR_SPACE_HH
#define ALGORITHM_VECTOR_SPACE_HH

#include <functional>
#include <vector>
#include <boost/type_erasure/any.hpp>

#include "Algorithms/norm.hh"
#include "Algorithms/scalarProduct.hh"

#include "Algorithms/Util/Concepts/vectorConcept.hh"
#include "Algorithms/Util/Concepts/vectorCreatorConcept.hh"
#include "Algorithms/Util/Mixins/impl.hh"

namespace Algorithm
{
  /// \cond
  namespace Detail { static unsigned spaceIndex = 0; }
  /// \endcond

  /**
   * @ingroup VHatGroup
   * @anchor VectorCreatorAnchor
   * @brief Vector creator for feeding into VectorSpace.
   * See \ref VectorCreatorConceptAnchor "VectorCreatorConcept".
   */
  using VectorCreator = boost::type_erasure::any< Concepts::VectorCreatorConcept >;


  /**
   * @ingroup VHatGroup
   * @brief Function space \f$(X,\|\cdot\|)\f$.
   * @see @ref VectorCreatorAnchor "VectorCreator".
   */
  class VectorSpace : public Mixin::Impl<VectorCreator>
  {
  public:
    /**
     * @brief Construct function space from @ref VectorCreatorAnchor "VectorCreator" and @ref NormAnchor "Norm".
     * @param impl object satisfying the @ref VectorCreatorConceptAnchor "VectorCreatorConcept"
     * @param norm object satisfying the @ref NormConceptAnchor "NormConcept"
     */
    VectorSpace(VectorCreator impl, Norm norm);

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
    boost::type_erasure::any<Concepts::VectorConcept> vector() const;

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
     * @brief Add space \f$Y\f$ for which this space \f$X\f$ acts as dual space.
     *
     * This is necessary to allow evaluation of \f$y(x)\f$ for \f$ x\in X \f$ and \f$ y\in Y\f$.
     */
    void addPrimalSpace(const VectorSpace& Y);

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
     * @brief Checks whether \f$Y\f$ has been assigned as primal space with respect to this space \f$X\f$.
     */
    bool isDualWRT(const VectorSpace& Y) const;

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
    void setRestriction(std::function<bool(const boost::type_erasure::any<Concepts::VectorConcept>&)> f);

    /**
     * @brief Checks if vector is admissible. Default implementation always returns true;
     * @return true if x is admissible, else false
     */
    bool isAdmissible(const boost::type_erasure::any<Concepts::VectorConcept>& x) const;

  private:
    Norm norm_;
    ScalarProduct sp_ = {};
    unsigned index_ = Detail::spaceIndex++;
    std::vector<unsigned> primalSpaces_ = {}, dualSpaces_ = {}; ///< primal and dual spaces with respect to this space
    const VectorSpace* dualSpace_ = nullptr;
    std::function<bool(const boost::type_erasure::any<Concepts::VectorConcept>&)> restriction_ = [](const boost::type_erasure::any<Concepts::VectorConcept>&){ return true; };
  };

  /**
   * @ingroup VHatGroup
   * @brief Construct Banach space.
   * @param creator object satisfying \ref VectorCreatorConceptAnchor "VectorCreatorConcept"
   * @param norm norm satisfying \ref NormConceptAnchor "NormConcept"
   * @return VectorSpace(impl,norm)
   */
  VectorSpace makeBanachSpace(VectorCreator creator, Norm norm);

  /**
   * @ingroup VHatGroup
   * @brief Construct Hilbert space.
   * @param creator object satisfying \ref VectorCreatorConceptAnchor "VectorCreatorConcept"
   * @param scalarProduct scalar product satisfying \ref ScalarProductConceptAnchor "ScalarProductConcept"
   * @return hilbert space
   */
  VectorSpace makeHilbertSpace(VectorCreator creator, ScalarProduct scalarProduct);

  /**
   * @ingroup VHatGroup
   * @brief Relate function spaces.
   * @param X primal space
   * @param Y dual space
   *
   * Makes \f$X\f$ the primal space of \f$Y\f$ and
   * makes \f$Y\f$ the dual space of \f$X\f$.
   * This admits the evaluation of \f$y(x)\f$ for \f$x\in X\f$ and \f$y\in Y\f$.
   */
  void connectPrimalDual(VectorSpace& X, VectorSpace& Y);
}
#endif // ALGORITHM_VECTOR_SPACE_HH
