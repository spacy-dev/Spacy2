#pragma once

#include <Spacy/Util/mixins.hh>

namespace Spacy
{
    namespace Newton
    {
        struct Parameter :
                Mixin::Eps,
                Mixin::MaxSteps,
                Mixin::RegularityTest,
                Mixin::RelativeAccuracy,
                Mixin::Timer<std::chrono::milliseconds>
        {};
    }
}
