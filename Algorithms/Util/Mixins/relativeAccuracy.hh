#ifndef ALGORITHM_UTIL_MIXIN_RELATIVE_ACCURACY_HH
#define ALGORITHM_UTIL_MIXIN_RELATIVE_ACCURACY_HH

#include "forwardConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for relative accuracy.
     */
    class RelativeAccuracy : public ForwardConnection<double>
    {
    public:
      /**
       * @brief Constructor. Sets relative accuracy.
       */
      explicit RelativeAccuracy(double relativeAccuracy = 1e-15) noexcept;

      /**
       * @brief Set relative accuracy.
       */
      void setRelativeAccuracy(double) noexcept;

      /**
       * @brief Get relative accuracy.
       */
      double relativeAccuracy() const noexcept;

      /**
       * @brief Connect relative accuracy to f.
       *
       * When setRelativeAccuracy(double relativeAccuracy) is called, then also
       * f.setRelativeAccuracy(relativeAccuracy) is invoked.
       */
      template <class F>
      void connectRelativeAccuracy(F& f)
      {
        connect( std::bind(&F::setRelativeAccuracy, std::ref(f), std::placeholders::_1) );
      }

    private:
      double relativeAccuracy_ = 1e-15;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_RELATIVE_ACCURACY_HH
