#ifndef ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for index of adjoint variable.
     */
    class AdjointIndex : public MixinConnection<AdjointIndex>
    {
    public:
      /**
       * @brief Constructor. Sets index of adjoint variable.
       */
      explicit AdjointIndex(unsigned index = 2) noexcept;

      /**
       * @brief Set index of adjoint variable.
       */
      void setAdjointIndex(unsigned index);

      /**
       * @brief Get index of adjoint variable.
       */
      double adjointIndex() const noexcept;

      /**
       * @brief Attach adjoint index.
       *
       * When setAdjointIndex(unsigned index) is called, then also
       * other.setAdjointIndex(unsigned index) is invoked.
       */
      void attachAdjointIndex(AdjointIndex& other);

      /**
       * @brief Detach verbosity before it gets deleted.
       */
      void detachAdjointIndex(AdjointIndex& other);

      /**
       * @brief update function for observer pattern.
       */
      void update(AdjointIndex* changedSubject);

    private:
      unsigned index_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH
