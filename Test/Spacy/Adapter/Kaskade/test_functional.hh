#pragma once

#include <algorithm>
#include <fem/functional_aux.hh>
#include <fem/fixdune.hh>
#include <fem/variables.hh>
#include <utilities/linalg/scalarproducts.hh>
#include <iostream>
namespace Kaskade
{
    template <class RType, class VarSet>
    class TestFunctional : public FunctionalBase<WeakFormulation>
    {
    public:
        using Scalar = RType;
        using OriginVars = VarSet;
        using AnsatzVars = VarSet;
        using TestVars = VarSet;

        static constexpr int dim = AnsatzVars::Grid::dimension;
        static constexpr int uIdx = 0;
        static constexpr int uSpaceIdx = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables,
        uIdx>::type::spaceIndex;

        class DomainCache : public CacheBase<TestFunctional,DomainCache>
        {
        public:
            DomainCache(TestFunctional const&,
                        typename AnsatzVars::VariableSet const& vars_,
                        int flags=7):
                data(vars_)
            {}


            template <class Position, class Evaluators>
            void evaluateAt(Position const& x, Evaluators const& evaluators)
            {
                u  = boost::fusion::at_c<uIdx>(data.data).value(boost::fusion::at_c<uSpaceIdx>(evaluators));
                du = boost::fusion::at_c<uIdx>(data.data).derivative(boost::fusion::at_c<uSpaceIdx>(evaluators));
                f = 1.0;
            }

            Scalar
            d0() const
            {
                return 0.5*sp(du,du) + 0.5*(u*u) - f*u;
            }

            template<int row>
            Scalar d1_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const& arg) const
            {
                return sp(du,arg.derivative) + (u*arg.value) - f*arg.value;
            }

            template<int row, int col>
            Scalar d2_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const &arg1,
                            VariationalArg<Scalar,dim,AnsatzVars::template Components<col>::m> const &arg2) const
            {
                return sp(arg1.derivative,arg2.derivative) + (arg1.value * arg2.value);
            }

        private:
            typename AnsatzVars::VariableSet const& data;
            Dune::FieldVector<Scalar,AnsatzVars::template Components<uIdx>::m> u, f;
            Dune::FieldMatrix<Scalar,AnsatzVars::template Components<uIdx>::m,dim> du;
            LinAlg::EuclideanScalarProduct sp;
        };


        class BoundaryCache : public CacheBase<TestFunctional,BoundaryCache>
        {
        public:
            BoundaryCache(TestFunctional const&,
                          typename AnsatzVars::VariableSet const&,
                          int)
            {}

            template <class Position, class Evaluators>
            void evaluateAt(Position const&, Evaluators const&)
            {
            }

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
        struct D1: public FunctionalBase<WeakFormulation>::D1<row>
        {
            static bool const present   = true;
            static bool const constant  = false;

        };

        template <int row, int col>
        struct D2: public FunctionalBase<WeakFormulation>::D2<row,col>
        {
            static bool const present = true;
            static bool const symmetric = true;
            static bool const lumped = false;
        };

        template <class Cell>
        int integrationOrder(Cell const& /* cell */, int shapeFunctionOrder, bool boundary) const
        {
            if (boundary)
                return 2*shapeFunctionOrder;
            else
            {
                int stiffnessMatrixIntegrationOrder = 2*(shapeFunctionOrder-1);
                int sourceTermIntegrationOrder = shapeFunctionOrder*shapeFunctionOrder;        // as rhs f is constant, i.e. of order 0

                return std::max(stiffnessMatrixIntegrationOrder,sourceTermIntegrationOrder);
            }
        }
    };
}
