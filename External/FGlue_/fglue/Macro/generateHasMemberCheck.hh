#include <utility>

#include "fglue/TMP/void_t.hh"

#define CONNECT(NAME,SOURCE,TARGET) SOURCE.set##NAME##(TARGET.get##NAME##());

#define GENERATE_MEM_FN_CHECK(NAME) \
template <class T> \
using TryMemFn_##NAME = decltype( &T::NAME ); \
 \
template <class T,class = void> struct HasMemFn_##NAME : std::false_type {}; \
 \
template <class T> \
struct HasMemFn_##NAME<T,FGlue::void_t< TryMemFn_##NAME<T> > > : std::is_member_function_pointer< TryMemFn_##NAME<T> > {};

//#define CONNECT_IF_POSSIBLE(NAME,SOURCE,TARGET)

