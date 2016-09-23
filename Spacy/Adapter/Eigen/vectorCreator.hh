#pragma once

#include <Eigen/Dense>

#include <Spacy/Adapter/Generic/vectorCreator.hh>

namespace Spacy
{
    /** @addtogroup EigenGroup, VectorSpaceGroup @{ */
    namespace Rn
    {
        /// %Vector creator for %Rn, based on the %Eigen library.
        using VectorCreator = Generic::VectorCreator< ::Eigen::VectorXd >;

        /**
         * @brief Generate a (non-product) Hilbert space based on the %Eigen library.
         * @return @ref ::Spacy::makeHilbertSpace() "::Spacy::makeHilbertSpace( VectorCreator{dim} , EuclideanScalarProduct{} )"
         */
        VectorSpace makeHilbertSpace(unsigned dim);

        /// Generate a product Hilbert space based on the %Eigen library.
        VectorSpace makeHilbertSpace(const std::vector<unsigned>& dims);

        /// Generate a primal-dual product Hilbert space based on the %Eigen library.
        VectorSpace makeHilbertSpace(const std::vector<unsigned>& dims,
                                     const std::vector<unsigned>& primalIds,
                                     const std::vector<unsigned>& dualIds);
    }
    /** @} */
}
