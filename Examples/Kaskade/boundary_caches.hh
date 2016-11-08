#pragma once

#include <dune/common/fvector.hh>
#include <fem/functional_aux.hh>

namespace Kaskade
{
    namespace PDE
    {
        template <class Functional, int state_index>
        class HomogeneousDirichletBoundary : public CacheBase< Functional, HomogeneousDirichletBoundary<Functional,state_index> >
        {
            using Scalar = typename Functional::Scalar;
            using AnsatzVars = typename Functional::AnsatzVars;
            using TestVars = typename Functional::TestVars;

            template <int entry>
            using AnsatzVarArg = VariationalArg<Scalar, AnsatzVars::Grid::dimension, AnsatzVars::template Components<entry>::m>;

            template <int entry>
            using TestVarArg = VariationalArg<Scalar, TestVars::Grid::dimension, TestVars::template Components<entry>::m>;

            static constexpr int state_space_index =
                    boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables, state_index>::type::spaceIndex;

        public:
            HomogeneousDirichletBoundary(const Functional&,
                                         const typename AnsatzVars::VariableSet& x,
                                         int) : x_(x)
            {}

            template <class Position, class Evaluators>
            void evaluateAt(const Position&, const Evaluators& evaluators)
            {
                using boost::fusion::at_c;
                u_ = at_c<state_index>(x_.data).value(at_c<state_space_index>(evaluators));
            }

            Scalar d0() const
            {
                return 0.5*penalty*(u_*u_);
            }

            template<int row>
            Scalar d1_impl (const TestVarArg<row>& arg) const
            {
                return penalty*(u_*arg.value);
            }

            template<int row, int col>
            Scalar d2_impl (const TestVarArg<row>& arg1, const AnsatzVarArg<col>& arg2) const
            {
                return penalty*(arg1.value*arg2.value);
            }

        private:
            const typename AnsatzVars::VariableSet& x_;
            Dune::FieldVector<Scalar,AnsatzVars::template Components<state_index>::m> u_;
            Scalar penalty = 1e9;
        };
    }

    namespace Optimization
    {
        template <class Functional, int state_index, int adjoint_index>
        class HomogeneousDirichletBoundary : public CacheBase< Functional, HomogeneousDirichletBoundary<Functional,state_index,adjoint_index> >
        {
            using Scalar = typename Functional::Scalar;
            using AnsatzVars = typename Functional::AnsatzVars;
            using TestVars = typename Functional::TestVars;

            template <int entry>
            using AnsatzVarArg = VariationalArg<Scalar, AnsatzVars::Grid::dimension, AnsatzVars::template Components<entry>::m>;

            template <int entry>
            using TestVarArg = VariationalArg<Scalar, TestVars::Grid::dimension, TestVars::template Components<entry>::m>;

            static constexpr int state_space_index =
                    boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables, state_index>::type::spaceIndex;
            static constexpr int adjoint_space_index =
                    boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables, adjoint_index>::type::spaceIndex;

        public:
            HomogeneousDirichletBoundary(const Functional&,
                                         const typename AnsatzVars::VariableSet& x,
                                         int) : x_(x)
            {}

            template <class Position, class Evaluators>
            void evaluateAt(const Position&, const Evaluators& evaluators)
            {
                using boost::fusion::at_c;
                y_ = at_c<state_index>(x_.data).value(at_c<state_space_index>(evaluators));
                p_ = at_c<adjoint_index>(x_.data).value(at_c<adjoint_space_index>(evaluators));
            }

            Scalar d0() const
            {
                return 0;
            }

            template<int row>
            Scalar d1_impl (const TestVarArg<row>& arg) const
            {
                if( row == state_index ) return penalty*(p_*if_(arg.value,y_));
                if( row == adjoint_index ) return penalty*(y_*if_(arg.value,p_));
            }

            template<int row, int col>
            Scalar d2_impl (const TestVarArg<row>& arg1, const AnsatzVarArg<col>& arg2) const
            {
                if( (row == state_index && col == adjoint_index) || (row == adjoint_index && col == state_index) )
                    return penalty*(if_(arg1.value,y_) * if_(arg2.value,p_));
                return 0;
            }

        private:
            const typename AnsatzVars::VariableSet& x_;
            Dune::FieldVector<Scalar,AnsatzVars::template Components<state_index>::m> y_;
            Dune::FieldVector<Scalar,AnsatzVars::template Components<adjoint_index>::m> p_;
            Scalar penalty = 1e9;
        };
    }
}
