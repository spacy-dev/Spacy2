#ifndef ALGORITHM_CONCEPT_SINGLE_CONCEPTS_HH
#define ALGORITHM_CONCEPT_SINGLE_CONCEPTS_HH

#include <boost/type_erasure/member.hpp>

// vector concepts
BOOST_TYPE_ERASURE_MEMBER( (has_space) , space , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_spaceIndex) , spaceIndex , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_size) , size , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_isAdmissible) , isAdmissible , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_norm) , norm , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_scalarProduct) , scalarProduct , 1 )


// operator concepts
BOOST_TYPE_ERASURE_MEMBER( (has_domain) , domain , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_domain_ptr) , domain_ptr , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_range) , range , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_range_ptr) , range_ptr , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_d1) , d1 , 2 )

BOOST_TYPE_ERASURE_MEMBER( (has_linearization) , linearization , 1 )

BOOST_TYPE_ERASURE_MEMBER( (has_solver) , solver , 0 )


// additional functional concepts
BOOST_TYPE_ERASURE_MEMBER( (has_d1_dual) , d1 , 1 )

BOOST_TYPE_ERASURE_MEMBER( (has_d2) , d2 , 3 )

BOOST_TYPE_ERASURE_MEMBER( (has_d2_dual) , d2 , 2 )

BOOST_TYPE_ERASURE_MEMBER( (has_hessian) , hessian , 1 )


// linear solver concepts
BOOST_TYPE_ERASURE_MEMBER( (has_isPositiveDefinite) , isPositiveDefinite , 0 )

#endif // ALGORITHM_CONCEPT_SINGLE_CONCEPTS_HH
