#pragma once

#include "macros.hh"

GENERATE_MIXIN_HEADER(double,AbsoluteAccuracy,1e-15)

GENERATE_MIXIN_HEADER(double,RelativeAccuracy,1e-15)

GENERATE_MIXIN_HEADER(double,MinimalAccuracy,1e-15)

GENERATE_MIXIN_HEADER(double,DampingAccuracy,5e-2)

#include "undefMacros.hh"
