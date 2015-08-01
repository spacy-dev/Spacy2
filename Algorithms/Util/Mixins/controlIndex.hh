#ifndef ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH

#include "forwardConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for index of control variable.
     */
    class ControlIndex
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
       * @brief Connect control index to f.
       *
       * When setControlIndex(unsigned index) is called, then also
       * f.setControlIndex(unsigned index) is invoked.
       */
      template <class F>
      void connectControlIndex(F& f)
      {
        connection_.connect( std::bind(&F::setControlIndex, std::ref(f), std::placeholders::_1) );
      }

    private:
      unsigned index_ = 1;
      ForwardConnection<unsigned> connection_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_CONTROL_INDEX_HH
