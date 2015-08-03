#ifndef ALGORITHM_UTIL_MIXIN_VERBOSITY_HH
#define ALGORITHM_UTIL_MIXIN_VERBOSITY_HH

#include "Util/DesignPatterns/observer.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for verbosity.
     */
    class Verbosity : public DesignPattern::Observer::Observer, public DesignPattern::Observer::Subject
    {
    public:
      /**
       * @brief Constructor. Sets verbosity.
       */
      explicit Verbosity(bool verbose = false) noexcept;

      /**
       * @brief Enable/disable verbosity.
       */
      void setVerbosity(bool) noexcept;

      /**
       * @brief Enable/disable detailed verbosity.
       */
      void setDetailedVerbosity(bool) noexcept;

      /**
       * @brief Check if verbosity is turned on.
       */
      bool verbose() const noexcept;

      /**
       * @brief Check if verbosity is turned on.
       */
      bool verbose_detailed() const noexcept;

      /**
       * @brief Attach verbosity.
       *
       * When setVerbosity(bool verbose) is called, then also
       * other.setVerbosity(verbose) is invoked.
       * When setDetailedVerbosity(bool verbose) is called, then also
       * other.setDetailedVerbosity(verbose) is invoked.
       */
      void attachVerbosity(Verbosity* other);

      /**
       * @brief Detach verbosity before it gets deleted.
       */
      void detachVerbosity(Verbosity* other);

    private:
      void update(DesignPattern::Observer::Subject* changedSubject) final override;

      bool verbose_ = false, verbose_detailed_ = false;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_VERBOSITY_HH
