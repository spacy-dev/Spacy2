#ifndef ALGORITHM_ADAPTER_FENICS_UTIL_HH
#define ALGORITHM_ADAPTER_FENICS_UTIL_HH

#include "Algorithms/vector.hh"

/// \cond
namespace dolfin
{
  class Form;
  class GenericVector;
  class Function;
}
/// \endcond

namespace Algorithm
{
  namespace FEniCS
  {
    /**
     * @ingroup FenicsGroup
     * @brief Copy coefficients of F to G.
     *
     * Whatever the copy-constructor for dolfin forms does it does not copy. So if
     * you want to copy these do the following.
     * Create A new form from the function spaces stored in G. Then copy the coefficients with
     * this function.
     */
    void copyCoefficients(const dolfin::Form& F, dolfin::Form& G);

    /**
     * @ingroup FenicsGroup
     * @brief Copy from \ref VectorAnchor "::Algorithm::Vector" to dolfin::GenericVector.
     *
     * Does consider product space structures.
     * @see ProductSpace
     */
    void copy(const ::Algorithm::Vector& x, dolfin::GenericVector& y);

    /**
     * @ingroup FenicsGroup
     * @brief Copy from \ref VectorAnchor "::Algorithm::Vector" to dolfin::Function.
     *
     * Does consider product space structures.
     * @see ProductSpace
     */
    void copy(const ::Algorithm::Vector& x, dolfin::Function& y);

    /**
     * @ingroup FenicsGroup
     * @brief Copy from dolfin::GenericVector to \ref VectorAnchor "::Algorithm::Vector".
     *
     * Does consider product space structures.
     * @see ProductSpaceGroup
     */
    void copy(const dolfin::GenericVector& y, ::Algorithm::Vector& x);

    /**
     * @brief Copy from dolfin::Function to \ref VectorAnchor "::Algorithm::Vector".
     *
     * Does consider product space structures.
     * @see ProductSpaceGroup
     */
    void copy(const dolfin::Function& y, ::Algorithm::Vector& x);
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_UTIL_HH

