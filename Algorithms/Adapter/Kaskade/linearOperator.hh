#ifndef ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH
#define ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH

#include <memory>
#include <utility>

#include "../../operator.hh"
#include "../../vector.hh"
#include "vectorSpace.hh"
#include "linearizedOperator.hh"
#include "Util/Mixins/disableAssembly.hh"
#include "Util/Base/operatorBase.hh"

#include "directSolver.hh"
#include "vector.hh" // copy

namespace Algorithm
{
  namespace Kaskade
  {
    template <class OperatorImpl, class AnsatzVariableSetDescription, class TestVariableSetDescription>
    class LinearOperator : public OperatorBase
    {
      using Spaces = typename AnsatzVariableSetDescription::Spaces;
      using Variables = typename AnsatzVariableSetDescription::Variables;
      using Domain = typename AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Range = typename TestVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Matrix = ::Kaskade::MatrixAsTriplet<double>;

    public:
      LinearOperator(const OperatorImpl& A,
               ::Algorithm::VectorSpace* domain_,
               ::Algorithm::VectorSpace* range_)
        : OperatorBase(domain_,range_),
          A_(A),
          spaces_( extractSpaces<AnsatzVariableSetDescription>(domain()) )
      {}

      LinearOperator(const OperatorImpl& f, const ::Algorithm::VectorSpace& domain, const ::Algorithm::VectorSpace& range)
        : LinearOperator(f,&domain,&range)
      {}

      ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const
      {
        Domain x_( AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<AnsatzVariableSetDescription>(x,x_);
        Range y_( TestVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

        A_.apply(x_,y_);

        auto y = range().element();
        copyFromCoefficientVector<TestVariableSetDescription>(y_,y);

        return y;
      }

      ::Algorithm::Vector d1(const ::Algorithm::Vector&, const ::Algorithm::Vector& dx) const
      {
        Domain dx_( AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<AnsatzVariableSetDescription>(dx,dx_);
        Range y_( TestVariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

        A_.apply( dx_ , y_ );

        auto y = range().element();
        copyFromCoefficientVector<TestVariableSetDescription>(y_,y);

        return y;
      }


      LinearSolver solver() const
      {
        return DirectSolver<AnsatzVariableSetDescription,TestVariableSetDescription>( A_ , spaces_, range_ptr() , domain_ptr() );
      }

    private:
      OperatorImpl A_;
      Spaces spaces_;
    };




    template <class OperatorImpl, class AnsatzVariableSetDescription, class TestVariableSetDescription>
    auto makeLinearOperator(const OperatorImpl& f,
                            ::Algorithm::VectorSpace* domain,
                            ::Algorithm::VectorSpace* range)
    {
      return LinearOperator<OperatorImpl, AnsatzVariableSetDescription, TestVariableSetDescription>( f, domain , range );
    }

    template <class OperatorImpl, class AnsatzVariableSetDescription, class TestVariableSetDescription>
    auto makeLinearOperator(const OperatorImpl& f,
                      const ::Algorithm::VectorSpace& domain,
                      const ::Algorithm::VectorSpace& range)
    {
      return LinearOperator<OperatorImpl, AnsatzVariableSetDescription, TestVariableSetDescription>( f, domain , range );
    }
  }
}

#endif // ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH
