#ifndef SPACY_LINEAR_SOLVER_HH
#define SPACY_LINEAR_SOLVER_HH

#include <functional>

#include "Util/Mixins/impl.hh"
#include "Util/Mixins/target.hh"

#include "vector.hh"

/** @addtogroup SpacyGroup
 * @{
 */
namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  /**
   * \brief Type-erased linear solver.
   *
   * The minimal signature (besides copy and/or move constructor/assignment) of a linear solver is:
   * @code
   * // My linear solver representing A^-1.
   * class MyLinearSolver
   * {
   * public:
   *   // Compute A^-1(x).
   *   ::Spacy::Vector operator()(const ::Spacy::Vector& x) const;
   * };
   * @endcode
   */
  using LinearSolver = std::function<Vector(const Vector&)>;

  /**
   * \brief Type-erased indefinite linear solver. Additionally monitors if the underlying operator is positive definite.
   *
   * The minimal signature (besides copy and/or move constructor/assignment) of an indefinite linear solver is:
   * @code
   * // My indefinite linear solver S representing A^-1.
   * class MyIndefiniteLinearSolver
   * {
   * public:
   *   // Copy constructor.
   *   MyIndefiniteLinearSolver(const MyIndefiniteLinearSolver&);
   *
   *   // Move constructor.
   *   MyIndefiniteLinearSolver(MyIndefiniteLinearSolver&&);
   *
   *   // Compute A^-1(x).
   *   ::Spacy::Vector operator()(const ::Spacy::Vector& x) const;
   *
   *   // Check if A is positive definite
   *   bool isPositiveDefinite() const;
   * };
   * @endcode
   */
  class IndefiniteLinearSolver : public Mixin::Target<IndefiniteLinearSolver>
  {
    template <class T>
    using TryMemFn_isPositiveDefinite = decltype(std::declval<T>().isPositiveDefinite());

    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Vector operator()(const Vector&) const = 0;
      virtual bool isPositiveDefinite() const = 0 ;
    };

    template <class Impl>
    struct Base :
        public AbstractBase,
        public Mixin::CopyingUniqueImpl<Impl>
    {
      explicit Base(const Impl& impl)
        : Mixin::CopyingUniqueImpl<Impl>(std::make_unique<Impl>(impl))
      {}

      Vector operator()(const Vector& x) const final override
      {
        return this->impl()(x);
      }

      bool isPositiveDefinite() const final override
      {
        return this->impl().isPositiveDefinite();
      }
    };

  public:
    IndefiniteLinearSolver() = default;

    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,IndefiniteLinearSolver>::value>,
              class = void_t< TryMemFn_isPositiveDefinite<Impl> > >
    IndefiniteLinearSolver(Impl&& impl)
      : base_( Base< std::decay_t<Impl> >( std::forward<Impl>(impl) ) )
    {}

    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,IndefiniteLinearSolver>::value>,
              class = void_t< TryMemFn_isPositiveDefinite<Impl> > >
    IndefiniteLinearSolver& operator=(Impl&& impl)
    {
      base_ = Base< std::decay_t<Impl> >( std::forward<Impl>(impl) );
      return *this;
    }

    Vector operator()(const Vector& x) const;

    bool isPositiveDefinite() const;

    operator bool() const;

    friend bool is_empty(const IndefiniteLinearSolver&);

  private:
    Mixin::CopyingUniqueImpl<AbstractBase> base_ = {};
  };

  bool is_empty(const IndefiniteLinearSolver& solver);
}
/** @} */

#endif // SPACY_LINEAR_SOLVER_HH
