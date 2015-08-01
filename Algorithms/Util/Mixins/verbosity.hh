#ifndef ALGORITHM_UTIL_MIXIN_VERBOSITY_HH
#define ALGORITHM_UTIL_MIXIN_VERBOSITY_HH

#include "forwardConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for verbosity.
     */
    class Verbosity
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
       * @brief Connect verbosity to f.
       *
       * When setVerbosity(bool verbose) is called, then also
       * f.setVerbosity(verbose) is invoked.
       * When setDetailedVerbosity(bool verbose) is called, then also
       * f.setDetailedVerbosity(verbose) is invoked.
       */
      template <class F>
      void connectVerbosity(F& f)
      {
        connectVerbosity_.connect( std::bind(&F::setVerbosity, std::ref(f), std::placeholders::_1) );
        connectDetailedVerbosity_.connect( std::bind(&F::setDetailedVerbosity, std::ref(f), std::placeholders::_1) );
      }

    private:
      bool verbose_ = false, verbose_detailed_ = false;
      ForwardConnection<bool> connectVerbosity_, connectDetailedVerbosity_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_VERBOSITY_HH
