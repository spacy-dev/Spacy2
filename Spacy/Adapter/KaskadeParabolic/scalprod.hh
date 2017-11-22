/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*  This file is part of the library KASKADE 7                               */
/*    see http://www.zib.de/projects/kaskade7-finite-element-toolbox         */
/*                                                                           */
/*  Copyright (C) 2002-2015 Zuse Institute Berlin                            */
/*                                                                           */
/*  KASKADE 7 is distributed under the terms of the ZIB Academic License.    */
/*    see $KASKADE/academic.txt                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include <algorithm>
#include "fem/functional_aux.hh"
#include "fem/fixdune.hh"
#include "fem/variables.hh"
#include "utilities/linalg/scalarproducts.hh"
namespace Spacy
{
    namespace KaskadeParabolic
    {
        template <class RType, class AnsatzVars_, class TestVars_=AnsatzVars_, class OriginVars_= AnsatzVars_>
        class ScalarProdFunctional : public ::Kaskade::FunctionalBase<::Kaskade::VariationalFunctional>
        {
        public:
            using Scalar = RType;
            using OriginVars = OriginVars_;
            using AnsatzVars = AnsatzVars_;
            using TestVars = TestVars_;

            static constexpr int dim = AnsatzVars::Grid::dimension;
            static constexpr int uIdx = 0;


            /// \class DomainCache
            ///
            class DomainCache : public ::Kaskade::CacheBase<ScalarProdFunctional,DomainCache>
            {
            public:
                DomainCache(ScalarProdFunctional const&,
                            typename AnsatzVars::VariableSet const& vars_,
                            int flags=7):
                    data(vars_)
                {}


                template <class Position, class Evaluators>
                void evaluateAt(Position const& x, Evaluators const& evaluators)
                {
                }

                Scalar
                d0() const
                {
                    return 0;
                }

                template<int row>
                Scalar d1_impl (::Kaskade::VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const& arg) const
                {
                            return 0;
                }

                template<int row, int col>
                Scalar d2_impl (::Kaskade::VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const &arg1,
                                ::Kaskade::VariationalArg<Scalar,dim,AnsatzVars::template Components<row>::m> const &arg2) const
                {
                    if (row == uIdx && col == uIdx) return sp(arg1.value,arg2.value);
                    return 0;
                }

            private:
                typename AnsatzVars::VariableSet const& data;

                ::Kaskade::LinAlg::EuclideanScalarProduct sp;
            };

            /// \class BoundaryCache
            ///
            class BoundaryCache : public ::Kaskade::CacheBase<ScalarProdFunctional,BoundaryCache>
            {
            public:
                BoundaryCache(ScalarProdFunctional<RType,AnsatzVars> const&,
                              typename AnsatzVars::VariableSet const& vars_,
                              int flags=7):
                    data(vars_)
                {}

                template <class Position, class Evaluators>
                void evaluateAt(Position const&, Evaluators const& evaluators)
                {
                }

                Scalar
                d0() const
                {
                    return 0;
                }

                template<int row>
                Scalar d1_impl (::Kaskade::VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const& arg) const
                {

                    return 0;
                }

                template<int row, int col>
                Scalar d2_impl (::Kaskade::VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const &arg1,
                                ::Kaskade::VariationalArg<Scalar,dim,AnsatzVars::template Components<row>::m> const &arg2) const
                {
                    if (row == uIdx && col == uIdx) return sp(arg1.value,arg2.value);
                    return 0;
                }


            private:
                typename AnsatzVars::VariableSet const& data;
                ::Kaskade::LinAlg::EuclideanScalarProduct sp;

            };

            /// \struct D2
            ///
            template <int row>
            struct D1: public ::Kaskade::FunctionalBase<::Kaskade::VariationalFunctional>::D1<row>
            {
                static bool const present   = true;
                static bool const constant  = false;

            };

            template <int row, int col>
            struct D2: public ::Kaskade::FunctionalBase<::Kaskade::VariationalFunctional>::D2<row,col>
            {
                static bool const present = true;
                static bool const symmetric = true;
                static bool const lumped = false;
            };

            /// \fn integrationOrder
            ///
            template <class Cell>
            int integrationOrder(Cell const& /* cell */, int shapeFunctionOrder, bool boundary) const
            {
                if (boundary)
                    return 2*shapeFunctionOrder;
                else
                {
                    int stiffnessMatrixIntegrationOrder = 2*(shapeFunctionOrder-1);
                    int sourceTermIntegrationOrder = shapeFunctionOrder;        // as rhs f is constant, i.e. of order 0

                    return std::max(stiffnessMatrixIntegrationOrder,sourceTermIntegrationOrder);
                }
            }
        };
    }
}
