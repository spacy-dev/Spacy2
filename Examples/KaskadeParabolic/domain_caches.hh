#pragma once

#include <dune/common/fvector.hh>
#include <fem/functional_aux.hh>
#include <utilities/linalg/scalarproducts.hh>

#include <tuple>

namespace Kaskade
{
    template <class Operator, class Integrand, int state_index>
    class OperatorWithConstantSource
            : public CacheBase< Operator, OperatorWithConstantSource<Operator, Integrand, state_index> >
    {
        using Scalar = typename Operator::Scalar;
        using AnsatzVars = typename Operator::AnsatzVars;
        using TestVars = typename Operator::TestVars;

        static constexpr int dim = AnsatzVars::Grid::dimension;

        static constexpr int state_space_index =
                boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables, state_index>::type::spaceIndex;

        template <int entry>
        using AnsatzVarArg = VariationalArg<Scalar, dim, AnsatzVars::template Components<entry>::m>;

        template <int entry>
        using TestVarArg = VariationalArg<Scalar, dim, TestVars::template Components<entry>::m>;


    public:
        OperatorWithConstantSource(const Operator& F,
                              typename AnsatzVars::VariableSet const& x,
                              int):
            f_(F.f_), x_(x), g_(1)
        {}

        template <class Position, class Evaluators>
        void evaluateAt(const Position&, Evaluators const& evaluators)
        {
            using boost::fusion::at_c;

            f_.template update<state_index>( std::make_tuple(
                                                 at_c<state_index>(x_.data).value(at_c<state_space_index>(evaluators)),
                                                 at_c<state_index>(x_.data).gradient(at_c<state_space_index>(evaluators))
                                                 ) );
        }

        Scalar
        d0() const
        {
            return 0;
        }

        template<int row>
        Scalar d1_impl (const TestVarArg<row>& arg) const
        {
            return sp( f_(), arg.gradient ) - sp( g_, arg.value );
        }

        template<int row, int col>
        Scalar d2_impl (const TestVarArg<row>& arg1, const AnsatzVarArg<col>& arg2) const
        {
            return sp( f_.template d1<state_index>( std::make_tuple(arg2.value, arg2.gradient) ), arg1.gradient );
        }

    private:
        Integrand f_;
        const typename AnsatzVars::VariableSet& x_;
        Dune::FieldVector<Scalar,AnsatzVars::template Components<state_index>::m> g_;
        LinAlg::EuclideanScalarProduct sp;
    };


    template <class Functional, class Integrand, int state_index>
    class FunctionalWithConstantSource
            : public CacheBase< Functional, FunctionalWithConstantSource<Functional, Integrand, state_index> >
    {
        using Scalar = typename Functional::Scalar;
        using AnsatzVars = typename Functional::AnsatzVars;
        using TestVars = typename Functional::TestVars;

        static constexpr int dim = AnsatzVars::Grid::dimension;

        static constexpr int state_space_index =
                boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables, state_index>::type::spaceIndex;

        template <int entry>
        using AnsatzVarArg = VariationalArg<Scalar, dim, AnsatzVars::template Components<entry>::m>;

        template <int entry>
        using TestVarArg = VariationalArg<Scalar, dim, TestVars::template Components<entry>::m>;


    public:
        FunctionalWithConstantSource(const Functional& F,
                              typename AnsatzVars::VariableSet const& x,
                              int):
            f_(F.f_), x_(x), y_(0), g_(1)
        {}

        template <class Position, class Evaluators>
        void evaluateAt(const Position&, Evaluators const& evaluators)
        {
            using boost::fusion::at_c;

            y_ = at_c<state_index>(x_.data).value(at_c<state_space_index>(evaluators));
            f_.update( at_c<state_index>(x_.data).gradient(at_c<state_space_index>(evaluators)) );
        }

        Scalar
        d0() const
        {
            return f_() - sp( g_, y_ );
        }

        template<int row>
        Scalar d1_impl (const TestVarArg<row>& arg) const
        {
            return f_.d1( arg.gradient ) - sp( g_, arg.value );
        }

        template<int row, int col>
        Scalar d2_impl (const TestVarArg<row>& arg1, const AnsatzVarArg<col>& arg2) const
        {
            return f_.d2( arg1.gradient, arg2.gradient );
        }

    private:
        Integrand f_;
        const typename AnsatzVars::VariableSet& x_;
        Dune::FieldVector<Scalar,AnsatzVars::template Components<state_index>::m> y_, g_;
        LinAlg::EuclideanScalarProduct sp;
    };
}
