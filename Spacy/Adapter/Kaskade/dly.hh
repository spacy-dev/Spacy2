#pragma once

#include <Spacy/Util/Mixins/accuracy.hh>
#include <Spacy/Adapter/Kaskade/Definitions/lumped.hh>
#include <Spacy/Adapter/Kaskade/Definitions/error_distribution.hh>

namespace Spacy
{
  namespace Kaskade
  {
    template <template <class,class> class Definition,
              class AnsatzVars, class AnsatzVarsExtension>
    class DLY :
        public Mixin::RelativeAccuracy
    {
      using Grid = typename AnsatzVars::Grid;
      using Definition_HH = Definition<AnsatzVars,AnsatzVars>;
      using Definition_HE = Definition<AnsatzVars,AnsatzVarsExtension>;
      using Definition_EE = Definition<AnsatzVarsExtension,AnsatzVarsExtension>;
      using LumpedDefinition_EE = LumpedDefinition< Definition_EE >;
      using Scalar = typename Definition_HH::Scalar;
      static constexpr int dim = AnsatzVars::Grid::dimension;
      using Assembler_HE = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt< Definition_HE > >;
      using Assembler_EE = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt< LumpedDefinition_EE > >;
      using ErrorAssembler = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt< ErrorDistribution<Definition_HH,AnsatzVarsExtension> > >;
      using A_HE = ::Kaskade::AssembledGalerkinOperator<Assembler_HE>;
      using A_EE = ::Kaskade::AssembledGalerkinOperator<Assembler_EE>;
      using CoeffVector_H = typename AnsatzVars::template CoefficientVectorRepresentation<>::type;
      using CoeffVector_E = typename AnsatzVarsExtension::template CoefficientVectorRepresentation<>::type;
      using CoeffVector_R = typename AnsatzVarsExtension::template CoefficientVectorRepresentation<>::type;
    public:
      template <class... Args>
      DLY(::Kaskade::GridManager<Grid>& gridManager, const AnsatzVars& ansatzVars, const AnsatzVarsExtension& ansatzVarsExtension, const Args&... args)
        : Mixin::RelativeAccuracy(0.25),
          gridManager_(gridManager), ansatzVars_(ansatzVars), ansatzVarsExtension_(ansatzVarsExtension),
          def_A_hh(args...), def_A_he(args...),
          def_A_ee(args...), def_A_ee_lumped(args...)
      {}

      bool operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& dx)
      {
        std::cout << "estimating error" << std::endl;
        const auto& x_ = cast_ref< Vector<AnsatzVars> >(x).get();
        typename AnsatzVars::VariableSet vx_(ansatzVars_);
        using boost::fusion::at_c;
        at_c<0>(vx_.data).coefficients() = at_c<0>(x_.data).coefficients();

        Assembler_HE assembler_he( ansatzVars_.spaces );
        Assembler_EE assembler_ee( ansatzVarsExtension_.spaces);

        assembler_he.assemble(::Kaskade::linearization(def_A_he, vx_));

        auto xe = typename AnsatzVarsExtension::VariableSet(ansatzVarsExtension_);
        assembler_ee.assemble(::Kaskade::linearization(def_A_ee_lumped,xe));

        auto A_he = A_HE(assembler_he);
        auto A_ee = A_EE(assembler_ee);

        CoeffVector_E rhs(assembler_ee.rhs());
//        const auto& dx_ = cast_ref< Vector<AnsatzVars> >(dx).get();
        CoeffVector_H dx_( AnsatzVars::template CoefficientVectorRepresentation<>::init(ansatzVars_.spaces) );
        copyToCoefficientVector<AnsatzVars>(dx,dx_);
        CoeffVector_E y( AnsatzVarsExtension::template CoefficientVectorRepresentation<>::init(ansatzVarsExtension_.spaces) );
        A_he.apply(dx_,y);
        rhs -= y;
        CoeffVector_R estimate( AnsatzVarsExtension::template CoefficientVectorRepresentation<>::init(ansatzVarsExtension_.spaces) );

        ::Kaskade::directInverseOperator(A_ee,DirectType::UMFPACK).apply(rhs,estimate);

        auto estimate_H = vx_;
        estimate_H *= 0;
        typename AnsatzVarsExtension::VariableSet estimate_E(ansatzVarsExtension_);
        at_c<0>(estimate_E.data).coefficients() = at_c<0>(estimate.data);
        ErrorDistribution<Definition_HH,AnsatzVarsExtension> distr(def_A_hh,vx_,estimate_H,estimate_E);
        ErrorAssembler errorAssembler( distr.spaces() );
        auto xd0 = typename ErrorDistribution<Definition_HH,AnsatzVarsExtension>::AnsatzVars::VariableSet(distr.variableSetDescription());
        auto lin = ::Kaskade::linearization(distr,xd0);
        errorAssembler.assemble(lin);
        using CellWiseCoeff = typename ErrorDistribution<Definition_HH,AnsatzVarsExtension>::AnsatzVars::template CoefficientVectorRepresentation<>;
        using CellWiseError = typename CellWiseCoeff::type;
        auto cellWiseError = CellWiseError(errorAssembler.rhs());
        at_c<0>(xd0.data).coefficients() = at_c<0>(cellWiseError.data);

        double errorEstimate = cellWiseError*cellWiseError;

        std::cout << "err: " << errorEstimate << "/" << norm(dx) << std::endl;
        if( errorEstimate < getRelativeAccuracy() * norm(dx)) return false;

        gridManager_.globalRefine(1);
        std::cout << "done" << std::endl;
        return true;
//        auto const& is = ansatzVars_.gridManager().grid().leafIndexSet();
//        std::vector<std::pair<double,int> > errorDistribution(is.size(0),std::make_pair(0.0,0));
//        // refine grid
//        auto cend = ansatzVars_.gridView.template end<0>();
//        for( auto ci = ansatzVars_.gridView.template begin<0>(); ci!=cend; ++ci )
//          errorDistribution[is.index(*ci)] = std::make_pair( fabs(boost::fusion::at_c<0>(xd0.data).value(*ci,Dune::FieldVector<Scalar,dim>(1./(dim+1)))) , is.index(*ci));

//        std::sort(errorDistribution.begin(), errorDistribution.end(), [](const auto& x1, const auto& x2)
//        {
//          return fabs(x1.first) > fabs(x2.first);
//        });

//        Scalar n2 = gridManager_.grid().size(0);
//        Scalar relTol = getRelativeAccuracy()/n2;

//        // do refinement
//        size_t counter = 0;
//        Scalar minErr = errorDistribution.back().first, maxErr = errorDistribution[0].first;
//        std::vector<size_t> mainContributors;
//        //constexpr int dim = ErrorRepresentation::Descriptions::Grid::dimension;
//        //Dune::FieldVector<Scalar,dim> x0(0.2);

//        std::cout << "marking cells..." << std::flush;
//        for (auto ci=gridManager_.grid().leafGridView().template begin<0>(); ci!=cend; ++ci) // iterate over cells
//        {
//          for(size_t i=0; i<errorDistribution.size(); ++i) // iterate over chosen part of the error distribution
//            if(gridManager_.grid().leafIndexSet().index(*ci) == errorDistribution[i].second)
//            {
//              if(errorDistribution[i].first > relTol){
//                if(errorDistribution[i].first > 4 * relTol) mainContributors.push_back(errorDistribution[i].second);
//                ++counter;
//                gridManager_.mark(1,*ci);
//              }
//            }
//        }
//        std::cout << "done." << std::endl;
//        std::cout << "totalErrorSquared: " << getRelativeAccuracy() << " -> tolerance: " << relTol << std::endl;
//        std::cout << "maxErr: " << maxErr << ", minErr: " << minErr << std::endl;
//        std::cout << "refining " << counter << " of " << gridManager_.grid().size(0) << " cells" << std::endl;

//        std::cout << "adapting..." << std::flush;
//        gridManager_.adaptAtOnce();
//        std::cout << "done." << std::endl;
      }



    private:
      ::Kaskade::GridManager<Grid>& gridManager_;
      AnsatzVars ansatzVars_;
      AnsatzVarsExtension ansatzVarsExtension_;
      Definition<AnsatzVars,AnsatzVars> def_A_hh;
      Definition<AnsatzVars,AnsatzVarsExtension> def_A_he;
      Definition<AnsatzVarsExtension,AnsatzVarsExtension> def_A_ee;
      LumpedDefinition< Definition<AnsatzVarsExtension,AnsatzVarsExtension> > def_A_ee_lumped;
    };
  }
}
