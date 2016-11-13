#pragma once

#include "macros.hh"

#include <Spacy/Spaces/RealSpace/real.hh>

GENERATE_MIXIN_HEADER(Real,AbsoluteAccuracy,1e-15)

GENERATE_MIXIN_HEADER(Real,RelativeAccuracy,1e-15)

GENERATE_MIXIN_HEADER(Real,MinimalAccuracy,1e-15)

GENERATE_MIXIN_HEADER(Real,DampingAccuracy,5e-2)

#include "undefMacros.hh"
