#ifndef ALGORITHM_UTIL_MIXIN_EPS_HH
#define ALGORITHM_UTIL_MIXIN_EPS_HH

#include "mixinConnection.hh"

namespace VSA
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief %Mixin class for maximal attainable accuracy \f$\varepsilon\f$.
     */
    class Eps : public MixinConnection<Eps>
    {
    public:
      /**
       * @brief Constructor.
       * @param eps maximal attainable accuracy \f$\varepsilon\f$
       */
      explicit Eps(double eps = 1e-15) noexcept;

      /**
       * @brief Set maximal attainable accuracy \f$\varepsilon\f$.
       * @param eps new maximal attainable accuracy
       */
      void setEps(double eps);

      /**
       * @brief Access maximal attainable accuracy.
       * @return \f$\varepsilon\f$
       */
      double eps() const noexcept;

      /**
       * @brief Access square root of maximal attainable accuracy.
       * @return \f$\sqrt\varepsilon\f$
       */
      double sqrtEps() const noexcept;

      /**
       * @brief Get third root of maximal attainable accuracy.
       * @return \f$\varepsilon^{1/3}\f$
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
