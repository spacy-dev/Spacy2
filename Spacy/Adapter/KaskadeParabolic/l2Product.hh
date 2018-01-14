#pragma once

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  class VectorSpace;
  /// @endcond

  namespace KaskadeParabolic
  {
    /**
     * @ingroup KaskadeParabolicGroup
     * @brief Generic l2 scalar product for Kaskade7. Based on the implementation of the dual pairing.
     */
    class l2Product
    {
    public:
      /**
       * @brief Compute l2 product.
       * @param x vector
       * @param y vector
       * @return \f$(x,y) = \sum_i x_i y_i \f$.
       */
      ::Spacy::Real operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const
      {
        /// We only want scalar products with mass matrices not just coefficient vector L2 norms
        std::cout<<"You did not declare a meaningful scalarproduct, returning zero"<<std::endl;
        return ::Spacy::Real{0.};
      }
    };
  }
}
