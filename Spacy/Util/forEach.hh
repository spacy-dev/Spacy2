#pragma once

namespace Spacy
{
    template <int... ids>
    struct VariableIds {};

    template <class Ids>
    struct ForEach;


    template <int id, int... ids>
    struct ForEach< VariableIds<id, ids...> >
    {
        template <template <int> class Wrapper, class Functor>
        static void apply(Functor f)
        {
            Wrapper<id>{f}();
            ForEach< VariableIds<ids...> >::template apply<Wrapper>(f);
        }
    };
}
