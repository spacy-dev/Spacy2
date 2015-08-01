#ifndef ALGORITHM_UTIL_MIXIN_EPS_HH
#define ALGORITHM_UTIL_MIXIN_EPS_HH

#include "forwardConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Parameter class for maximal attainable accuracy.
     */
    class Eps
    {
    public:
      /**
       * @brief Constructor. Sets maximal attainable accuracy.
       */
      explicit Eps(double eps = 1e-15) noexcept;

      /**
       * @brief Set maximal attainable accuracy.
       */
      void setEps(double) noexcept;

      /**
       * @brief Get maximal attainable accuracy.
       */
      double eps() const noexcept;

      /**
       * @brief Get square root of maximal attainable accuracy.
       */
      double sqrtEps() const noexcept;

      /**
       * @brief Get third root of maximal attainable accuracy.
       */
      double cbrtEps() const noexcept;


      template <class F>
      void connectEps(F& f)
      {
        connection_.connect( std::bind(&F::setEps, std::ref(f), std::placeholders::_1) );
      }

    private:
      double eps_ = 1e-15;
      ForwardConnection<double> connection_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_EPS_HH
