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
     * @brief Abstract interface for functionals \f$f:\ X\rightarrow \mathbb{R}\f$.
     */
    class AbstractFunctional : public Mixin::Cloneable<AbstractFunctional>
    {
    public:
      /**
       * @brief Constructor.
       * @param domain Domain space \f$X\f$.
       */
      AbstractFunctional(std::shared_ptr<AbstractBanachSpace> domain);

      virtual ~AbstractFunctional();

      /// Apply functional, i.e. compute \f$f(x)\f$.
      double operator()(const AbstractFunctionSpaceElement& x) const;

      /// Access domain space \f$X\f$.
      const AbstractBanachSpace& domain() const;

      /// Access shared domain space \f$X\f$.
      std::shared_ptr<AbstractBanachSpace> sharedDomain() const;

    protected:
      virtual double d0(const AbstractFunctionSpaceElement&) const = 0;

    private:
      std::shared_ptr<AbstractBanachSpace> domain_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C0_FUNCTIONAL_HH
