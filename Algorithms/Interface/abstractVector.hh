#ifndef ALGORITHM_INTERFACE_ABSTRACT_VECTOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_VECTOR_HH

#include <iostream>
#include <memory>
#include <vector>

#include "Util/Mixins/cloneable.hh"
#include "Util/Mixins/eps.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace Interface
  {
    class AbstractVector :
        public Mixin::Cloneable<AbstractVector>,
        public Mixin::Eps
    {
    public:
      explicit AbstractVector(const ::Algorithm::VectorSpace& space);

      virtual ~AbstractVector() = default;

      virtual void copyTo(AbstractVector&) const = 0;

      virtual void print(std::ostream&) const = 0;

      virtual AbstractVector& operator=(const AbstractVector& y) = 0;

      virtual AbstractVector& operator+=(const AbstractVector& y) = 0;

      virtual AbstractVector& axpy(double a, const AbstractVector& y);

      virtual AbstractVector& operator-=(const AbstractVector& y) = 0;

      virtual AbstractVector& operator*=(double) = 0;

      virtual std::unique_ptr<AbstractVector> operator- () const = 0;

      double operator()(const AbstractVector& y) const;

      /**
     * @brief Size of coefficient vector.
     */
      virtual unsigned size() const = 0;

      /**
     * @brief Access entry in coefficient vector.
     */
      virtual double& coefficient(unsigned) = 0;

      /**
     * @brief Access entry in coefficient vector.
     */
      virtual const double& coefficient(unsigned) const = 0;

      /**
       * @brief Access unique space index.
       */
      unsigned spaceIndex() const;

      /**
       * @brief Access underlying function space.
       */
      const ::Algorithm::VectorSpace& space() const;

      /**
       * @brief Checks whether two function space elements coincide up to the maximal attainable accuracy.
       */
      bool equals(const AbstractVector& y) const;

      /**
       * @brief Checks whether a function space element is admissible.
       *
       * The default implementation always returns true. You can overwrite this function if your search space is a
       * subset of a function space.
       */
      virtual bool isAdmissible() const;

    protected:
      virtual double applyAsDualTo(const AbstractVector& y) const = 0;

    private:
      const ::Algorithm::VectorSpace& space_;
    };

    std::ostream& operator<<(std::ostream& os, const AbstractVector& element);


    /// Compute scalar product \f$(x,y)\f$.
    double operator* (const AbstractVector& x, const AbstractVector& y);
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_VECTOR_HH
