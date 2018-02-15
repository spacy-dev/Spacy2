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
                                                                                              std::vector<Real>{}, std::vector<std::shared_ptr<Spaces> >{} , *space );
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

      /// @cond
      template <class,class> class NormOperator;
      /// @endcond

      template <class AnsatzVariableSetDescription, class TestVariableSetDescription>
      class NormOperatorCreator
      {
        using Spaces = typename AnsatzVariableSetDescription::Spaces;
        using Scalar = typename AnsatzVariableSetDescription::Scalar;
        using Domain = typename AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
        using Range = typename TestVariableSetDescription::template CoefficientVectorRepresentation<>::type;
        using Matrix = ::Kaskade::MatrixAsTriplet<Scalar>;
        using OperatorImpl = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain,Range>;
        using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain,Range>;

      public:
        NormOperatorCreator(const VectorSpace& domain, const VectorSpace& range)
          : domain_(domain),
            range_(range)
        {}

        NormOperator<AnsatzVariableSetDescription,TestVariableSetDescription> operator()(const VectorSpace* space) const
        {
          std::cout<< " calling the LinearBlockOperatorPDE creator "<<std::endl;
          return NormOperator<AnsatzVariableSetDescription,TestVariableSetDescription>(std::vector<KaskadeOperator>(),
                                                                                              std::vector<Real>{}, std::vector<std::shared_ptr<Spaces> >{} , *space );
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
    namespace OCP
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

        using VYSetDescription = Detail::ExtractDescription_t<AnsatzVariableSetDescription,0>;
        using VUSetDescription = Detail::ExtractDescription_t<AnsatzVariableSetDescription,1>;
        using VPSetDescription = Detail::ExtractDescription_t<AnsatzVariableSetDescription,2>;

        using CoefficientVectorY = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
        using CoefficientVectorU = typename VUSetDescription::template CoefficientVectorRepresentation<>::type;
        using CoefficientVectorP = typename VPSetDescription::template CoefficientVectorRepresentation<>::type;

        template<class X, class Y>
        using KaskadeOperatorXY = ::Kaskade::MatrixRepresentedOperator<Matrix,X,Y>;

        using Mytype = KaskadeOperatorXY<CoefficientVectorY,CoefficientVectorY>;
        using Mutype = KaskadeOperatorXY<CoefficientVectorY,CoefficientVectorY>;
        using Atype = KaskadeOperatorXY<CoefficientVectorY,CoefficientVectorP>;
        using ATtype = KaskadeOperatorXY<CoefficientVectorP,CoefficientVectorY>;
        using Btype = KaskadeOperatorXY<CoefficientVectorU,CoefficientVectorP>;
        using BTtype = KaskadeOperatorXY<CoefficientVectorP,CoefficientVectorU>;

        using Masstype = KaskadeOperatorXY<CoefficientVectorY,CoefficientVectorP>;

      public:
        LinearBlockOperatorCreator(const VectorSpace& domain, const VectorSpace& range)
          : domain_(domain),
            range_(range)
        {}

        LinearBlockOperator<AnsatzVariableSetDescription,TestVariableSetDescription> operator()(const VectorSpace* space) const
        {
          std::cout<< " calling the LinearBlockOperatorOCP creator "<<std::endl;
          return LinearBlockOperator<AnsatzVariableSetDescription,TestVariableSetDescription>(std::vector<Mytype>(),
                                                                                              std::vector<Mutype>(), std::vector<Atype>(), std::vector<Btype>(),
                                                                                              std::vector<ATtype>(), std::vector<BTtype>() , std::vector<Masstype>(),
                                                                                              std::vector<Masstype>() ,
                                                                                              std::vector<Real>{}, std::vector<std::shared_ptr<Spaces> >{} , *space );;
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
