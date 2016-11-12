#pragma once

#include <fem/fixdune.hh>
#include <fem/functional_aux.hh>
#include <fem/variables.hh>

#include <algorithm>

using namespace Kaskade;

template <class VarSet>
class CostFunctional : public FunctionalBase<VariationalFunctional>
{
public:
    using Scalar = double;
    using OriginVars = VarSet;
    using AnsatzVars = VarSet;
    using TestVars = VarSet;

    static constexpr int dim = AnsatzVars::Grid::dimension;
    static constexpr int uIdx = 0;
    static constexpr int pIdx = 1;
    static constexpr int uSpaceIdx = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables, uIdx>::type::spaceIndex;
    static constexpr int pSpaceIdx = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables, pIdx>::type::spaceIndex;

    class DomainCache : public CacheBase<CostFunctional,DomainCache>
    {
    public:
        DomainCache(CostFunctional const&,
                    typename AnsatzVars::VariableSet const& vars_,
                    int flags=7):
            data(vars_)
        {}


        template <class Position, class Evaluators>
        void evaluateAt(Position const& x, Evaluators const& evaluators)
        {
            u  = boost::fusion::at_c<uIdx>(data.data).value(boost::fusion::at_c<uSpaceIdx>(evaluators));
            p  = boost::fusion::at_c<pIdx>(data.data).value(boost::fusion::at_c<pSpaceIdx>(evaluators));
        }

        Scalar
        d0() const
        {
            return 0.5*(u*u) + 0.5*(p*p);
        }

        template<int row>
        Scalar d1_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const& arg) const
        {
            if( row == uIdx )
                return u*arg.value;

            return p*arg.value;
        }

        template<int row, int col>
        Scalar d2_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const &arg1,
                        VariationalArg<Scalar,dim,AnsatzVars::template Components<col>::m> const &arg2) const
        {
            if( row == col ) return arg1.value*arg2.value;
            return 0;
        }

    private:
        typename AnsatzVars::VariableSet const& data;
        Dune::FieldVector<Scalar,AnsatzVars::template Components<uIdx>::m> u;
        Dune::FieldVector<Scalar,AnsatzVars::template Components<pIdx>::m> p;
    };


    class BoundaryCache : public CacheBase<CostFunctional,BoundaryCache>
    {
    public:
        BoundaryCache(const CostFunctional&,
                      typename AnsatzVars::VariableSet const& vars_,
                      int flags=7)
        {}

        template <class Position, class Evaluators>
        void evaluateAt(Position const&, Evaluators const& evaluators)
        {}

        Scalar d0() const
        {
            return 0;
        }

        template<int row>
        Scalar d1_impl (VariationalArg<Scalar,dim> const& arg) const
        {
            return 0;
        }

        template<int row, int col>
        Scalar d2_impl (VariationalArg<Scalar,dim> const &arg1,
                        VariationalArg<Scalar,dim> const &arg2) const
        {
            return 0;
        }
    };

    template <int row>
    struct D1: public FunctionalBase<VariationalFunctional>::D1<row>
    {
        static bool const present   = true;
        static bool const constant  = false;

    };

    template <int row, int col>
    struct D2: public FunctionalBase<VariationalFunctional>::D2<row,col>
    {
        static bool const present = true;
        static bool const symmetric = true;
        static bool const lumped = false;
    };

    template <class Cell>
    int integrationOrder(Cell const& /* cell */, int shapeFunctionOrder, bool boundary) const
    {
        return 2*shapeFunctionOrder;
    }
};
