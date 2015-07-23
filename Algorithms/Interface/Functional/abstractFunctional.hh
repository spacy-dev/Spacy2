#ifndef ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH

#include <memory>

#include "Util/Mixins/cloneable.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// \cond
    class AbstractBanachSpace;
    class AbstractFunctionSpaceElement;
    /// \endcond

    /**
     * @brief Interface class for functionals.
     */
    class AbstractFunctional : public Mixin::Cloneable<AbstractFunctional>
    {
    public:
      /// Construct functional defined on the banach space domain.
      AbstractFunctional(std::shared_ptr<AbstractBanachSpace> domain);

      virtual ~AbstractFunctional();

      /// Evaluate functional at x.
      double operator()(const AbstractFunctionSpaceElement& x) const;

      /// Access to the banach space on which this functional is defined.
      const AbstractBanachSpace& getDomain() const;

      /// Shared a
      /// ccess to the banach space on which this functional is defined.
      std::shared_ptr<AbstractBanachSpace> getSharedDomain() const;

    protected:
      virtual double d0(const AbstractFunctionSpaceElement&) const = 0;

    private:
      std::shared_ptr<AbstractBanachSpace> domain_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C0_FUNCTIONAL_HH
