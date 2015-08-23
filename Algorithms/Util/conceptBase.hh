#ifndef CONCEPTBASE_HH
#define CONCEPTBASE_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>

namespace Algorithm
{
  using ConceptBase =
  boost::mpl::vector<
    // copy
    boost::type_erasure::copy_constructible<> ,
    // run-time type information for cast_ref
    boost::type_erasure::typeid_<> ,
    boost::type_erasure::relaxed
  >;
}
#endif // CONCEPTBASE_HH

