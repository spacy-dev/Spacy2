#ifndef SPACY_KASKADE_MASS_MATRIX_HH
#define SPACY_KASKADE_MASS_MATRIX_HH

#include <algorithm>
#include "fem/functional_aux.hh"
#include "fem/fixdune.hh"

/// \cond
namespace Spacy
{
  namespace Kaskade
  {

    template <class RType, class VarSet>
    class MassMatrix : public ::Kaskade::FunctionalBase< ::Kaskade::WeakFormulation >
    {
    public:
      using Scalar = RType;
      using OriginVars = VarSet;
      using AnsatzVars = VarSet;
      using TestVars = VarSet;

      static constexpr int dim = AnsatzVars::Grid::dimension;

      class DomainCache : public ::Kaskade::CacheBase<MassMatrix,DomainCache>
      {
      public:
        DomainCache(MassMatrix const&,
                    typename AnsatzVars::VariableSet const&,
                    int=7)
        {}


        template <class Position, class Evaluators>
        void evaluateAt(Position const& x, Evaluators const& evaluators)
        {}

        Scalar
        d0() const
        {
          return 0;
        }

        template<int row>
        Scalar d1_impl(const ::Kaskade::VariationalArg<Scalar,dim,TestVars::template Components<row>::m>& arg) const
        {
          return 0;
        }

        template<int row, int col>
        Scalar d2_impl(const ::Kaskade::VariationalArg<Scalar,dim,TestVars::template Components<row>::m>& arg1,
                       const ::Kaskade::VariationalArg<Scalar,dim,AnsatzVars::template Components<row>::m>& arg2) const
        {
          if( row == col ) return (arg1.value,arg2.value) * arg2.value;
          return 0;
        }
      };

      class BoundaryCache : public ::Kaskade::CacheBase<MassMatrix,BoundaryCache>
      {
      public:
        BoundaryCache(MassMatrix<RType,AnsatzVars> const&,
                      typename AnsatzVars::VariableSet const&,
                      int=7)
        {}

        template <class Position, class Evaluators>
        void evaluateAt(Position const&, Evaluators const& evaluators)
        {}

        Scalar d0() const
        {
          return 0;
        }

        template<int row, int n>
        Scalar d1_impl(const ::Kaskade::VariationalArg<Scalar,dim,n>&) const
        {
          return 0;
        }

        template<int row, int col, int n, int m>
        Scalar d2_impl(const ::Kaskade::VariationalArg<Scalar,dim,n>&,
                       const ::Kaskade::VariationalArg<Scalar,dim,m>&) const
        {
          return 0;
        }
      };

      template <int row>
      struct D1: public ::Kaskade::FunctionalBase< ::Kaskade::WeakFormulation >::D1<row>
      {
        static bool const present   = false;
        static bool const constant  = false;

      };

      template <int row, int col>
      struct D2: public ::Kaskade::FunctionalBase< ::Kaskade::WeakFormulation >::D2<row,col>
      {
        static bool const present = row==col;
        static bool const symmetric = true;
        static bool const lumped = false;
      };

      template <class Cell>
      int integrationOrder(Cell const& /* cell */, int shapeFunctionOrder, bool boundary) const
      {
        return 2*shapeFunctionOrder;
      }
    };
  }
}
/// \endcond

#endif // SPACY_KASKADE_MASS_MATRIX_HH
