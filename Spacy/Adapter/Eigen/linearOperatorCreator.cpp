#include "linearOperatorCreator.hh"

#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/cast.hh>

#include <Eigen/Dense>

#include "vectorCreator.hh"

namespace Spacy
{
  namespace Rn
  {

    LinearOperatorCreator::LinearOperatorCreator(const VectorSpace& X, const VectorSpace& Y)
      : Generic::LinearOperatorCreator< LinearOperator >
        ( [&X,&Y](const VectorSpace* space)
          {
            return LinearOperator( ::Eigen::MatrixXd::Zero( cast_ref<VectorCreator>(X.creator()).dim() , cast_ref<VectorCreator>(Y.creator()).dim()) , *space , X , Y );
          } ,
          X , Y )
    {}
  }
}

