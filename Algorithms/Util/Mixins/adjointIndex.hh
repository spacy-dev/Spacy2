#ifndef ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH
#define ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH

#include "forwardConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for index of adjoint variable.
     */
    class AdjointIndex
    {
    public:
      /**
       * @brief Constructor. Sets index of adjoint variable.
       */
      explicit AdjointIndex(unsigned index = 2) noexcept;

      /**
       * @brief Set index of adjoint variable.
       */
      void setAdjointIndex(unsigned index) noexcept;

      /**
       * @brief Get index of adjoint variable.
       */
      double adjointIndex() const noexcept;

      /**
       * @brief Connect adjoint index to f.
       *
       * When setAdjointIndex(unsigned index) is called, then also
       * f.setAdjointIndex(unsigned index) is invoked.
       */
      template <class F>
      void connectAdjointIndex(F& f)
      {
        connection_.connect( std::bind(&F::setAdjointIndex, std::ref(f), std::placeholders::_1) );
      }

    private:
      unsigned index_ = 2;
      ForwardConnection<unsigned> connection_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_ADJOINT_INDEX_HH
