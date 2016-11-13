#pragma once

#include "../../boundary_caches.hh"
#include "../../traits.hh"

#include <functional>
#include <memory>
#include <type_traits>

#include <fem/variables.hh>
#include <utilities/linalg/scalarproducts.hh>

namespace Kaskade
{
    enum class RoleOfFunctional{ NORMAL, TANGENTIAL };

    template <class Ids, class Constraint, class CostFunctional, class AnsatzVars_, class TestVars_=AnsatzVars_, RoleOfFunctional role = RoleOfFunctional::NORMAL>
    class StepFunctional
    {
    public:
        using Scalar = double;
        using AnsatzVars = AnsatzVars_;
        using TestVars = TestVars_;
        using OriginVars = AnsatzVars;
        static constexpr int dim = AnsatzVars::Grid::dimension;
        static ProblemType const type = std::is_same<AnsatzVars,TestVars>::value ? VariationalFunctional : WeakFormulation;

        typedef typename AnsatzVars::Grid Grid;
        typedef typename Grid::template Codim<0>::Entity Cell;

        template <int row>
        using D1 = NonConstD1<row>;

        static int const ySIdx = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables,Ids::state>::type::spaceIndex;
        static int const uSIdx = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables,Ids::control>::type::spaceIndex;
        static int const pSIdx = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables,Ids::adjoint>::type::spaceIndex;


        class DomainCache : public CacheBase<StepFunctional,DomainCache>
        {
        public:
            DomainCache(StepFunctional const& f,
                        typename AnsatzVars::VariableSet const& vars_,int):
                f_(f), vars(vars_),
                constraint_(f_.constraint_),
                costFunctional_(f_.costFunctional_),
                update_reference_(f_.update_reference_)
            {}

            template <class Position, class Evaluators>
            void evaluateAt(Position const& x, Evaluators const& evaluators)
            {
                using namespace boost::fusion;
                y = at_c<Ids::state>(vars.data).value(at_c<ySIdx>(evaluators));
                u = at_c<Ids::control>(vars.data).value(at_c<uSIdx>(evaluators));
                p = at_c<Ids::adjoint>(vars.data).value(at_c<pSIdx>(evaluators));

                dy = at_c<Ids::state>(vars.data).gradient(at_c<ySIdx>(evaluators));
                dp = at_c<Ids::adjoint>(vars.data).gradient(at_c<pSIdx>(evaluators));

                costFunctional_.template update<Ids::state>(y);
                costFunctional_.template update<Ids::control>(u);
                constraint_.template update<Ids::state>( std::make_tuple(y,dy) );
//                update_reference_(at_c<Ids::state>(f_.interpolated_reference_.data).value(at_c<ySIdx>(evaluators)));
            }

            Scalar d0() const
            {
                return costFunctional_();
            }

            template<int row>
            Scalar d1_impl (VariationalArg<Scalar,dim, TestVars::template Components<row>::m> const& arg) const
            {
                if( row == Ids::state )
                    return costFunctional_.template d1<Ids::state>(if_(arg.value,y)) +
                            sp( dp,
                                constraint_.template d1<Ids::state>( std::make_tuple( if_(arg.value,y), if_(arg.gradient,dy)) )
                                );
                if( row == Ids::control )
                    return costFunctional_.template d1<Ids::control>(if_(arg.value,u)) - sp(p, if_(arg.value, u));
                if( row == Ids::adjoint )
                    return sp( if_(arg.gradient,dp), constraint_() );
                return 0;
            }

            template<int row, int col>
            Scalar d2_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const &arg1, VariationalArg<Scalar,dim,TestVars::template Components<col>::m> const &arg2) const
            {
                if( row == Ids::state && col == Ids::state ) {
                    if( role == RoleOfFunctional::TANGENTIAL )
                        return costFunctional_.template d2<Ids::state,Ids::state>(if_(arg1.value,y),if_(arg2.value,y)) +
                                sp( dp,
                                    constraint_.template d2<Ids::state,Ids::state>(
                                        std::make_tuple(if_(arg1.value,y), if_(arg1.gradient,dy)),
                                        std::make_tuple(if_(arg2.value,y), if_(arg2.gradient,dy))
                                        )
                                    );
                    else
                        return costFunctional_.template d2<Ids::state,Ids::state>(if_(arg1.value,y),if_(arg2.value,y)) +
                                1e-6*(1e-2+1e2*(u*u))*sp( if_(arg1.gradient,dy), if_(arg2.gradient, dy) );
                }

                if( row == Ids::control && col == Ids::control )
                    return costFunctional_.template d2<Ids::control,Ids::control>(arg1.value, arg2.value);

                if( row == Ids::adjoint && col ==Ids::control )
                    return -sp( if_(arg2.value, u), if_(arg1.value, p) );
                if( row == Ids::control && col == Ids::adjoint )
                    return -sp( if_(arg1.value, u), if_(arg2.value, p) );

                if( row == Ids::state   && col == Ids::adjoint )
                    return sp( if_(arg2.gradient, dp),
                               constraint_.template d1<Ids::state>(
                                   std::make_tuple(if_(arg1.value, y), if_(arg1.gradient, dy)))
                               );
                if( row == Ids::adjoint && col == Ids::state )
                    return sp( if_(arg1.gradient, dp),
                               constraint_.template d1<Ids::state>(
                                   std::make_tuple(if_(arg2.value, y), if_(arg2.gradient, dy)))
                               );
                return 0;
            }

        private:
            const StepFunctional& f_;
            typename AnsatzVars::VariableSet const& vars;
            Constraint constraint_;
            CostFunctional costFunctional_;
            std::function<void(const Dune::FieldVector<double,1>&)> update_reference_;
            Dune::FieldVector<Scalar,AnsatzVars::template Components<Ids::state>::m> y;
            Dune::FieldVector<Scalar,AnsatzVars::template Components<Ids::control>::m> u;
            Dune::FieldVector<Scalar,AnsatzVars::template Components<Ids::adjoint>::m> p;
            Dune::FieldMatrix<Scalar,AnsatzVars::template Components<Ids::state>::m,dim> dy;
            Dune::FieldMatrix<Scalar,AnsatzVars::template Components<Ids::adjoint>::m,dim> dp;
            LinAlg::EuclideanScalarProduct sp;
        };

        using BoundaryCache = Optimization::HomogeneousDirichletBoundary<StepFunctional, Ids::state, Ids::adjoint>;


        explicit StepFunctional(Constraint constraint, CostFunctional costFunctional,
                                typename AnsatzVars::VariableSet interpolated_reference,
                                std::function<void(const Dune::FieldVector<double,1>&)> update_reference)
            : constraint_(std::move(constraint)),
              costFunctional_(std::move(costFunctional)),
              interpolated_reference_(std::move(interpolated_reference)),
              update_reference_(std::move(update_reference))
        {}

        template <int row, int col>
        struct D2 : public FunctionalBase<WeakFormulation>::D2<row,col>
        {
            static bool const present = !( ( row == Ids::adjoint && col == Ids::adjoint ) ||
                                           ( row == Ids::state && col == Ids::control ) ||
                                           ( row == Ids::control && col == Ids::state ) );
            static bool const symmetric = row==col;
            static bool const lumped = false;
        };

        template <class Cell>
        int integrationOrder(Cell const& /* cell */, int shapeFunctionOrder, bool boundary ) const
        {
            if( boundary ) return 2*shapeFunctionOrder;
            return 4*shapeFunctionOrder - 2;
        }

        Constraint constraint_;
        CostFunctional costFunctional_;
        typename AnsatzVars::VariableSet interpolated_reference_;
        std::function<void(const Dune::FieldVector<double,1>&)> update_reference_;
    };


    template <class Ids, class Constraint, class CostFunctional, class AnsatzVars, class TestVars=AnsatzVars>
    using NormalStepFunctional = StepFunctional<Ids,Constraint,CostFunctional,AnsatzVars,TestVars,RoleOfFunctional::NORMAL>;

    template <class Ids, class Constraint, class CostFunctional, class AnsatzVars, class TestVars=AnsatzVars>
    using TangentialStepFunctional = StepFunctional<Ids,Constraint,CostFunctional,AnsatzVars,TestVars,RoleOfFunctional::TANGENTIAL>;
}
