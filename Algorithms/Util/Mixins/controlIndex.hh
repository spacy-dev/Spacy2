#ifndef ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for index of the control variable.
     */
    class ControlIndex : public MixinConnection<ControlIndex>
    {
    public:
      /**
       * @brief Constructor.
       * @param index index of the control variable
       */
      explicit ControlIndex(unsigned index = 1) noexcept;

      /**
       * @brief Set index of the control variable.
       * @param index of the control variable
       */
      void setControlIndex(unsigned index);

      /**
       * @brief Get index of control variable.
       * @return control index
       */
      double controlIndex() const noexcept;

      /**
       * @brief Attach control index.
       *
       * When setControlIndex(unsigned index) is called, then also
       * other.setControlIndex(unsigned index) is invoked.
       */
      void attachControlIndex(ControlIndex& other);

      /**
       * @brief Detach verbosity before it gets deleted.
       */
      void detachControlIndex(ControlIndex& other);

      /**
       * @brief update function for observer pattern.
       */
      void update(ControlIndex* changedSubject);

    private:
      unsigned index_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH
