#ifndef ALGORITHM_UTIL_MIXIN_EPS_HH
#define ALGORITHM_UTIL_MIXIN_EPS_HH

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for maximal attainable accuracy.
     */
    class Eps : public MixinConnection<Eps>
    {
    public:
      /**
       * @brief Constructor. Sets maximal attainable accuracy.
       */
      explicit Eps(double eps = 1e-15) noexcept;

      /**
       * @brief Set maximal attainable accuracy.
       */
      void setEps(double);

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

      /**
       * @brief Attach Eps.
       *
       * When setEps(double eps) is called, then also
       * other.setEps(eps) is invoked.
       */
      void attachEps(Eps& other);

      /**
       * @brief Detach Eps before it gets deleted.
       */
      void detachEps(Eps& other);

      /**
       * @brief update function for observer pattern.
       */
      void update(Eps* changedSubject);

    private:
      double eps_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_EPS_HH
