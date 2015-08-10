#ifndef ALGORITHM_INTERFACE_ABSTRACT_FUNCTION_SPACE_HH
#define ALGORITHM_INTERFACE_ABSTRACT_FUNCTION_SPACE_HH

#include <memory>
#include "abstractFunctionSpaceElement.hh"

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
    class AbstractFunctionSpace : public std::enable_shared_from_this<AbstractFunctionSpace>
    {
    public:
      /**
       * @brief Constructor for Banach spaces. Sets norm.
       */
      AbstractFunctionSpace(std::shared_ptr<AbstractNorm> norm);

      /**
       * @brief Constructor for Hilbert spaces. Sets scalar product.
       */
      AbstractFunctionSpace(std::shared_ptr<AbstractScalarProduct> sp);


      virtual ~AbstractFunctionSpace() = default;

      void setNorm(std::shared_ptr<AbstractNorm> norm);

      const AbstractNorm& norm() const;

      std::shared_ptr<AbstractNorm> sharedNorm() const;

      std::unique_ptr<AbstractFunctionSpaceElement> element() const;

      unsigned index() const noexcept;

      void addPrimalSpace(const AbstractFunctionSpace& primalSpace);

      void addDualSpace(const AbstractFunctionSpace& dualSpace);

      bool isPrimalWRT(const AbstractFunctionSpace& dualSpace) const;

      bool isDualWRT(const AbstractFunctionSpace& primalSpace) const;

//      const AbstractFunctionSpace& dualSpace() const;

      std::shared_ptr<AbstractFunctionSpace> dualSpacePtr() const;

      void setDualSpace(std::shared_ptr<AbstractFunctionSpace> dualSpace);

      /// Set scalar product.
      void setScalarProduct(std::shared_ptr<AbstractScalarProduct> sp);

      /// Get scalar product.
      std::shared_ptr<AbstractScalarProduct> scalarProduct() const;

      bool isHilbertSpace() const;

    protected:
      std::shared_ptr<AbstractNorm> norm_ = nullptr;
      virtual std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const = 0;

    private:
      std::vector<unsigned> primalSpaces_, dualSpaces_; ///< primal and dual spaces with respect to this space

      std::weak_ptr<AbstractFunctionSpace> dualSpace_;
      const unsigned index_ = spaceIndex++;

      std::shared_ptr<AbstractScalarProduct> sp_;
    };

    void makePrimalDual(AbstractFunctionSpace& primalSpace, AbstractFunctionSpace& dualSpace);


    /// Compute scalar product \f$(x,y)\f$.
    double operator* (const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y);
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_FUNCTION_SPACE_HH
