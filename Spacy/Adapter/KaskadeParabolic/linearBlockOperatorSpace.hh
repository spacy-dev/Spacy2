#pragma once

#include "vector.hh"
#include "fem/istlinterface.hh"
namespace Spacy
{
  /// @cond
  class VectorSpace;
  /// @endcond

  namespace KaskadeParabolic
  {
    namespace PDE
    {
      /// @cond
      template <class,class> class LinearBlockOperator;
      /// @endcond

      template <class AnsatzVariableSetDescription, class TestVariableSetDescription>
      class LinearBlockOperatorCreator
      {
        using Spaces = typename AnsatzVariableSetDescription::Spaces;
        using Scalar = typename AnsatzVariableSetDescription::Scalar;
        using Domain = typename AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
        using Range = typename TestVariableSetDescription::template CoefficientVectorRepresentation<>::type;
        using Matrix = ::Kaskade::MatrixAsTriplet<Scalar>;
        using OperatorImpl = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain,Range>;
        using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain,Range>;

      public:
        LinearBlockOperatorCreator(const VectorSpace& domain, const VectorSpace& range)
          : domain_(domain),
            range_(range)
        {}

        LinearBlockOperator<AnsatzVariableSetDescription,TestVariableSetDescription> operator()(const VectorSpace* space) const
        {
          std::cout<< " calling the LinearBlockOperatorPDE creator "<<std::endl;
          return LinearBlockOperator<AnsatzVariableSetDescription,TestVariableSetDescription>(std::vector<KaskadeOperator>(),std::vector<KaskadeOperator>(),
                                                                                              std::vector<double>{}, std::vector<std::shared_ptr<Spaces> >{} , *space );
        }

        const VectorSpace& domain() const
        {
          return domain_;
        }

        const VectorSpace& range() const
        {
          return range_;
        }

      private:
        const VectorSpace &domain_, &range_;
      };
    }
  }
}
