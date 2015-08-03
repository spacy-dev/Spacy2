#ifndef ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH

#include "Util/DesignPatterns/observer.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for index of adjoint variable.
     */
    class AdjointIndex : public DesignPattern::Observer::Subject , public DesignPattern::Observer::Observer
    {
    public:
      /**
       * @brief Constructor. Sets index of adjoint variable.
       */
      explicit AdjointIndex(unsigned index = 2) noexcept;

      /**
       * @brief Set index of adjoint variable.
       */
      void setAdjointIndex(unsigned index) noexcept;

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
      void attachAdjointIndex(AdjointIndex* other);

      /**
       * @brief Detach verbosity before it gets deleted.
       */
      void detachAdjointIndex(AdjointIndex* other);

    private:
      void update(DesignPattern::Observer::Subject* changedSubject) final override;

      unsigned index_ = 2;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH
