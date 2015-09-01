#ifndef ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH

#include "mixinConnection.hh"

namespace VSA
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief %Mixin class for index of state variable.
     */
    class StateIndex : public MixinConnection<StateIndex>
    {
    public:
      /**
       * @brief Constructor.
       * @param index index of state variable
       */
      explicit StateIndex(unsigned index = 0) noexcept;

      /**
       * @brief Set index of state variable.
       * @param index new index of state variable
       */
      void setStateIndex(unsigned index);

      /**
       * @brief Access index of state variable.
       * @return index of state variable
       */
      double stateIndex() const noexcept;

      /**
       * @brief Attach state index.
       *
       * When setStateIndex(unsigned index) is called, then also
       * other.setStateIndex(unsigned index) is invoked.
       */
      void attachStateIndex(StateIndex& other);

      /**
       * @brief Detach verbosity before it gets deleted.
       */
      void detachStateIndex(StateIndex& other);

      /**
       * @brief update function for observer pattern.
       */
      void update(StateIndex* changedSubject);

    private:
      unsigned index_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH
