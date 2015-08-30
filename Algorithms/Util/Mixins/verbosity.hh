#ifndef ALGORITHM_UTIL_MIXIN_VERBOSITY_HH
#define ALGORITHM_UTIL_MIXIN_VERBOSITY_HH

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief %Mixin class for verbosity.
     */
    class Verbosity : public MixinConnection<Verbosity>
    {
    public:
      /**
       * @brief Constructor.
       * @param verbosityLevel verbosity level (0=silent,...)
       */
      explicit Verbosity(unsigned verbosityLevel = 0) noexcept;

      /**
       * @brief Enable/disable verbosity.
       * @param verbose true: if verbosityLevel = 0, set verbosityLevel = 1; false: if set verbosityLevel = 0
       */
      void setVerbosity(bool verbose);

      /**
       * @brief Check if verbosity is turned on.
       * @return true if verbosityLevel > 0
       */
      bool verbose() const noexcept;

      /**
       * @brief Set verbosity level.
       * @param level verbosity level
       */
      void setVerbosityLevel(unsigned level) noexcept;

      /**
       * @brief Access verbosity level.
       * @return verbosity level
       */
      unsigned verbosityLevel() const noexcept;

      /**
       * @brief Attach verbosity.
       *
       * When setVerbosity(bool verbose) is called, then also
       * other.setVerbosity(verbose) is invoked.
       * When setDetailedVerbosity(bool verbose) is called, then also
       * other.setDetailedVerbosity(verbose) is invoked.
       */
      void attachVerbosity(Verbosity& other);

      /**
       * @brief Detach verbosity before it gets deleted.
       */
      void detachVerbosity(Verbosity& other);

      /**
       * @brief update function for observer pattern.
       */
      void update(Verbosity* changedSubject);

    private:
      unsigned verbosityLevel_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_VERBOSITY_HH
