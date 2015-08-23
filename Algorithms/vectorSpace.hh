#ifndef ALGORITHM_VECTOR_SPACE_HH
#define ALGORITHM_VECTOR_SPACE_HH

#include <vector>

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>

#include "norm.hh"
#include "scalarProduct.hh"
#include "vector.hh"

#include "Util/Concepts/conceptBase.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  /// \cond
  namespace Detail { static unsigned spaceIndex = 0; }
  class VectorSpace;
  /// \endcond

  using VectorCreatorConcept =
  boost::mpl::vector<
    ConceptBase ,
    boost::type_erasure::callable<Vector(const VectorSpace*), const boost::type_erasure::_self>
  >;

  using VectorCreator = boost::type_erasure::any< VectorCreatorConcept >;


  /**
   * @brief Function space \f$(X,\|\cdot\|)\f$.
   */
  class VectorSpace : public Mixin::Impl<VectorCreator>
  {
  public:
    /**
     * @brief Construct function space from implementation derived from AbstractVectorSpace.
     */
    VectorSpace(VectorCreator impl, Norm norm);

    /**
     * @brief Move vector space.
     */
    VectorSpace(VectorSpace&& other);

    /**
     * @brief Change norm of space.
     */
    void setNorm(Norm norm);

    /**
     * @brief Access norm.
     */
    const Norm& norm() const;

    /**
     * @brief Create new function space element.
     */
    Vector element() const;

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
     * @brief Access pointer to dual space \f$Y=X^*\f$.
     */
    VectorSpace* dualSpace_ptr() const;

    /**
     * @brief Set dual space \f$Y=X^*\f$.
     */
    void setDualSpace(VectorSpace* Y);

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

  private:
    Norm norm_;
    std::shared_ptr<ScalarProduct> sp_ = nullptr;
    unsigned index_ = Detail::spaceIndex++;
    std::vector<unsigned> primalSpaces_, dualSpaces_; ///< primal and dual spaces with respect to this space
    VectorSpace* dualSpace_;
  };

  /**
   * @brief Construct Banach space.
   */
  VectorSpace makeBanachSpace(VectorCreator impl, Norm norm);

  /**
   * @brief Construct Hilbert space.
   */
  VectorSpace makeHilbertSpace(VectorCreator impl, ScalarProduct scalarProduct);

  /**
   * @brief Relate function spaces.
   */
  void connectPrimalDual(VectorSpace& primalSpace, VectorSpace& dualSpace);

}
#endif // ALGORITHM_VECTOR_SPACE_HH
