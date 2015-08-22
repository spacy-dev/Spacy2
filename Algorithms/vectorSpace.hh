#ifndef ALGORITHM_VECTOR_SPACE_HH
#define ALGORITHM_VECTOR_SPACE_HH

#include <vector>

#include "norm.hh"
#include "scalarProduct.hh"

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/member.hpp>

#include "Util/conceptBase.hh"
#include "Util/Mixins/impl.hh"

#include "vector.hh"

BOOST_TYPE_ERASURE_MEMBER( (has_element) , element , 1 )

namespace Algorithm
{
  namespace Detail { static unsigned spaceIndex = 0; }
  /// \cond
  class VectorSpace;
  /// \endcond

  /**
   * @brief Vector space implementation class. Plug your implementations in here.
   */
  using VectorSpaceImpl = boost::type_erasure::any< boost::mpl::vector< ConceptBase , has_element<Vector(const VectorSpace*), const boost::type_erasure::_self> > >;


  /**
   * @brief Function space \f$(X,\|\cdot\|)\f$.
   */
  class VectorSpace : public Mixin::Impl<VectorSpaceImpl>
  {
  public:
    /**
     * @brief Construct function space from implementation derived from AbstractVectorSpace.
     */
//    VectorSpace(VectorSpaceImpl impl, Norm norm);

    VectorSpace(const VectorSpaceImpl& impl, const ScalarProduct& sp);

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
    std::shared_ptr<ScalarProduct> sp_;
    unsigned index_ = Detail::spaceIndex++;
    std::vector<unsigned> primalSpaces_, dualSpaces_; ///< primal and dual spaces with respect to this space
    VectorSpace* dualSpace_;
  };

  /**
   * @brief Relate function spaces.
   */
  void connectPrimalDual(VectorSpace& primalSpace, VectorSpace& dualSpace);

}
#endif // ALGORITHM_VECTOR_SPACE_HH
