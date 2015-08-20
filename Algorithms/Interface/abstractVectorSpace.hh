#ifndef ALGORITHM_INTERFACE_ABSTRACT_VECTOR_SPACE_HH
#define ALGORITHM_INTERFACE_ABSTRACT_VECTOR_SPACE_HH

#include <memory>

#include "norm.hh"
#include "abstractVector.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractScalarProduct;
    class AbstractNorm;

    static unsigned spaceIndex = 0;

    /**
     * @brief Abstract interface for function spaces.
     */
    class AbstractVectorSpace : public std::enable_shared_from_this<AbstractVectorSpace>
    {
    public:
      /**
       * @brief Constructor for Banach spaces. Sets norm.
       */
//      AbstractVectorSpace(Norm norm);

      /**
       * @brief Constructor for Hilbert spaces. Sets scalar product.
       */
      AbstractVectorSpace(std::shared_ptr<AbstractScalarProduct> sp);


      virtual ~AbstractVectorSpace() = default;

      void setNorm(Norm norm);

      Norm norm() const;

//      std::shared_ptr<AbstractNorm> sharedNorm() const;

      std::unique_ptr<AbstractVector> element() const;

      unsigned index() const noexcept;

      void addPrimalSpace(const AbstractVectorSpace& primalSpace);

      void addDualSpace(const AbstractVectorSpace& dualSpace);

      bool isPrimalWRT(const AbstractVectorSpace& dualSpace) const;

      bool isDualWRT(const AbstractVectorSpace& primalSpace) const;

//      const AbstractVectorSpace& dualSpace() const;

      std::shared_ptr<AbstractVectorSpace> dualSpacePtr() const;

      void setDualSpace(std::shared_ptr<AbstractVectorSpace> dualSpace);

      /// Set scalar product.
      void setScalarProduct(std::shared_ptr<AbstractScalarProduct> sp);

      /// Get scalar product.
      std::shared_ptr<AbstractScalarProduct> scalarProduct() const;

      bool isHilbertSpace() const;

    protected:
      Norm norm_;
      virtual std::unique_ptr<AbstractVector> elementImpl() const = 0;

    private:
      std::vector<unsigned> primalSpaces_, dualSpaces_; ///< primal and dual spaces with respect to this space

      std::weak_ptr<AbstractVectorSpace> dualSpace_;
      const unsigned index_ = spaceIndex++;

      std::shared_ptr<AbstractScalarProduct> sp_;
    };

    void makePrimalDual(AbstractVectorSpace& primalSpace, AbstractVectorSpace& dualSpace);
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_VECTOR_SPACE_HH
