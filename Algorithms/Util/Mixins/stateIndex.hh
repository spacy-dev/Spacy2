#ifndef ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH

#include "forwardConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for index of state variable.
     */
    class StateIndex
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
       * @brief Connect state index to f.
       *
       * When setStateIndex(unsigned index) is called, then also
       * f.setStateIndex(unsigned index) is invoked.
       */
      template <class F>
      void connectStateIndex(F& f)
      {
        connection_.connect( std::bind(&F::setStateIndex, std::ref(f), std::placeholders::_1) );
      }

    private:
      unsigned index_ = 0;
      ForwardConnection<unsigned> connection_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_STATE_INDEX_HH
