#ifndef ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for index of control variable.
     */
    class ControlIndex : public MixinConnection<ControlIndex>
    {
    public:
      /**
       * @brief Constructor. Sets index of control variable.
       */
      explicit ControlIndex(unsigned index = 1) noexcept;

      /**
       * @brief Set index of control variable.
       */
      void setControlIndex(unsigned index) noexcept;

      /**
       * @brief Get index of control variable.
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

      void update(ControlIndex* changedSubject);

    private:
      unsigned index_ = 1;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH
