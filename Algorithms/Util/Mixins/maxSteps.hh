#ifndef ALGORITHM_UTIL_MIXIN_MAXSTEPS_HH
#define ALGORITHM_UTIL_MIXIN_MAXSTEPS_HH

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for maximal number of steps/iterations.
     */
    class MaxSteps : public MixinConnection<MaxSteps>
    {
    public:
      /**
       * @brief Constructor. Set maximal number of steps/iterations.
       */
      explicit MaxSteps(unsigned maxSteps = 100) noexcept;

      /**
       * @brief Set maximal number of steps/iterations for iterative solvers.
       */
      void setMaxSteps(unsigned);

      /**
       * @brief Get maximal number of steps/iterations for iterative solvers.
       */
      unsigned maxSteps() const noexcept;

      /**
       * @brief Attach MaxSteps.
       *
       * When setMaxSteps(unsigned steps) is called, then also
       * other.setMaxSteps(unsigned steps) is invoked.
       */
      void attachMaxSteps(MaxSteps& other);

      /**
       * @brief Detach Eps before it gets deleted.
       */
      void detachMaxSteps(MaxSteps& other);

      /**
       * @brief update function for observer pattern.
       */
      void update(MaxSteps* changedSubject);

    private:
      unsigned maxSteps_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_MAXSTEPS_HH
