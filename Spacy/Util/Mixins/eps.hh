#pragma once

#include <Spacy/Spaces/RealSpace/real.hh>

#include "mixinConnection.hh"

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for maximal attainable accuracy \f$\varepsilon\f$.
    class Eps : public MixinConnection<Eps>
    {
    public:
      /**
       * @param eps maximal attainable accuracy \f$\varepsilon\f$
       */
      explicit Eps(Real eps = 1e-15) noexcept;

      void setEps(Real eps);

      /// Access \f$\varepsilon\f$.
      Real eps() const noexcept;

      ///Access \f$\sqrt\varepsilon\f$.
      Real sqrtEps() const noexcept;

      /// Access \f$\varepsilon^{1/3}\f$.
      Real cbrtEps() const noexcept;

      /// update function for observer pattern.
      void update(Eps* changedSubject);

    private:
      Real eps_;
    };
  }
}
