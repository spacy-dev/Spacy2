#ifndef ALGORITHM_UTIL_MIXIN_MAXSTEPS_HH
#define ALGORITHM_UTIL_MIXIN_MAXSTEPS_HH

#include "Util/DesignPatterns/observer.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for maximal number of steps/iterations.
     */
    class MaxSteps : public DesignPattern::Observer::Subject , public DesignPattern::Observer::Observer
    {
    public:
      /**
       * @brief Constructor. Set maximal number of steps/iterations.
       */
      explicit MaxSteps(unsigned maxSteps = 100) noexcept;

      /**
       * @brief Set maximal number of steps/iterations for iterative solvers.
       */
      void setMaxSteps(unsigned) noexcept;

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
      void attachMaxSteps(MaxSteps* other);

      /**
       * @brief Detach Eps before it gets deleted.
       */
      void detachMaxSteps(MaxSteps* other);

    private:
      void update(DesignPattern::Observer::Subject* changedSubject) override;

      unsigned maxSteps_ = 100;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_MAXSTEPS_HH
