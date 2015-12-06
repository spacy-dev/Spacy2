// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_ACCURACY_HH
#define SPACY_UTIL_MIXIN_ACCURACY_HH

#include "macros.hh"

GENERATE_MIXIN_HEADER(double,AbsoluteAccuracy,1e-15)

GENERATE_MIXIN_HEADER(double,RelativeAccuracy,1e-15)

GENERATE_MIXIN_HEADER(double,MinimalAccuracy,1e-15)

GENERATE_MIXIN_HEADER(double,DampingAccuracy,1e-3)

#include "undefMacros.hh"

#endif // SPACY_UTIL_MIXIN_ACCURACY_HH
