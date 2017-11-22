#include "RegularizeViaPreconditioner.hh"

#include <Spacy/Util/log.hh>

namespace Spacy
{
    namespace CG
    {
        const auto log_tag = "RegularizeViaPreconditioner";

        RegularizeViaPreconditioner::RegularizeViaPreconditioner( double minIncrease,
                                                                  double maxIncrease, double eps )
            : Mixin::Eps( eps ), minIncrease_( minIncrease ), maxIncrease_( maxIncrease )
        {
        }

        void RegularizeViaPreconditioner::init()
        {
            theta_ = 0;
        }

        void RegularizeViaPreconditioner::apply( Real& qAq, Real qPq ) const
        {
            qAq += theta_ * qPq;
        }

        void RegularizeViaPreconditioner::update( Real qAq, Real qPq )
        {
            const auto oldTheta = theta_ > 0 ? theta_ : Real( eps() );
            theta_ += ( 1 - qAq ) / abs( qPq );
            LOG( log_tag, "Computed regularization parameter: ", theta_ );
            theta_ = min( max( minIncrease_ * oldTheta, theta_ ), maxIncrease_ * oldTheta );
            LOG( log_tag, "Regularization parameter (old) ", oldTheta );
            LOG( log_tag, "Regularization parameter (new) ", theta_ );
        }

        void RegularizeViaPreconditioner::adjustResidual( Real alpha, const Vector& Pq,
                                                          Vector& r ) const
        {
            r -= ( alpha * theta_ ) * Pq;
        }

        Real RegularizeViaPreconditioner::theta() const
        {
            return theta_;
        }
    }
}
