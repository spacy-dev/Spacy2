#pragma once

#include "../traits.hh"
#include "../boundary_caches.hh"
#include "../domain_caches.hh"

#include <fem/functional_aux.hh>

#include <algorithm>

namespace Kaskade
{
    template <class Integrand, class VarSet, int state_index = 0>
    class FungFunctional : public FunctionalBase<WeakFormulation>
    {
    public:
        using Scalar = double;
        using OriginVars = VarSet;
        using AnsatzVars = VarSet;
        using TestVars = VarSet;

        template <int row>
        using D1 = NonConstD1<row>;

        template <int row, int col>
        using D2 = D2PresentAndNonSymmetric<row,col>;

        using DomainCache = FunctionalWithConstantSource<FungFunctional, Integrand, state_index>;

        using BoundaryCache = PDE::HomogeneousDirichletBoundary<FungFunctional, state_index>;


        Integrand f_;

        explicit FungFunctional(Integrand f) : f_(std::move(f))
        {}

        template <class Cell>
        int integrationOrder(Cell const& /*cell*/, int shapeFunctionOrder, bool boundary) const
        {
            if (boundary)
                return 2*shapeFunctionOrder;
            else
            {
                int stiffnessMatrixIntegrationOrder = 2*(shapeFunctionOrder-1);
                int sourceTermIntegrationOrder = shapeFunctionOrder;
                return std::max(stiffnessMatrixIntegrationOrder,sourceTermIntegrationOrder);
            }
        }
    };
}
