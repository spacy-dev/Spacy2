#pragma once

#include <fem/functional_aux.hh>

namespace Kaskade
{
    // Kaskade does not properly support working with non-symmetric blocks.
    // Thus these are assembled explicitly, which is not supported for 'VariationalFunctional'.
    // For this reason 'WeakFormulation' is used for D1 and D2.
    template <int row>
    struct NonConstD1 : FunctionalBase<WeakFormulation>::D1<row>
    {
        static constexpr bool present = true;
        static constexpr bool constant = false;
    };

    template <int row, int col>
    struct D2PresentAndNonSymmetric : FunctionalBase<WeakFormulation>::D2<row,col>
    {
        static bool const present = true;
        static bool const symmetric = false;
        static bool const lumped = false;
    };

    template <int row, int col>
    struct D2PresentAndSymmetric : FunctionalBase<WeakFormulation>::D2<row,col>
    {
        static bool const present = true;
        static bool const symmetric = false;
        static bool const lumped = false;
    };

    template <class Ids, int row, int col>
    struct D2PresentInNonlinearElasticity
    {
        static constexpr bool value = !(row == Ids::adjoint && col == Ids::adjoint);
    };

    template <class Ids, int row, int col>
    struct D2PresentInNonlinearHeat
    {
        static constexpr bool value = !( (row == Ids::adjoint && col == Ids::adjoints) ||
                                         (row == Ids::state   && col == Ids::control)  ||
                                         (row == Ids::control && col == Ids::state) );
    };


    template <template <int,int> class Present,
              int row, int col>
    struct OptimalControlD2 : FunctionalBase<WeakFormulation>::D2<row,col>
    {
        static constexpr bool present = Present<row,col>::value;
        static constexpr bool symmetric = row==col;
    };
}
