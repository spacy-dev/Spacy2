#ifndef ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH

#include "Util/DesignPatterns/observer.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for index of state variable.
     */
    class StateIndex : public DesignPattern::Observer::Subject , public DesignPattern::Observer::Observer
    {
    public:
      /**
       * @brief Constructor. Sets index of state variable.
       */
      explicit StateIndex(unsigned index = 0) noexcept;

      /**
       * @brief Set index of state variable.
       */
      void setStateIndex(unsigned index) noexcept;

      /**
       * @brief Get index of state variable.
       */
      double stateIndex() const noexcept;

      /**
       * @brief Attach state index.
       *
       * When setStateIndex(unsigned index) is called, then also
       * other.setStateIndex(unsigned index) is invoked.
       */
      void attachStateIndex(StateIndex* other);

      /**
       * @brief Detach verbosity before it gets deleted.
       */
      void detachStateIndex(StateIndex* other);

    private:
      void update(DesignPattern::Observer::Subject* changedSubject) final override;

      unsigned index_ = 0;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH
