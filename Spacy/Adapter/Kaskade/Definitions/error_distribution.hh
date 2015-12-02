#ifndef SPACY_ADAPTER_KASKADE_ERROR_DISTRIBUTION_HH
#define SPACY_ADAPTER_KASKADE_ERROR_DISTRIBUTION_HH

#include <string>

#include <boost/utility.hpp>
#include <boost/fusion/include/as_vector.hpp>

#include "fem/assemble.hh"
#include "fem/functionspace.hh"
#include "fem/lagrangespace.hh"
#include "fem/shapefunctioncache.hh"
#include "utilities/linalg/scalarproducts.hh"

// forward declarations
namespace Dune
{
  template <class,int> class FieldVector;
  template <class,int,int> class FieldMatrix;
  template <class, int> class QuadratureRule;
}

namespace Spacy
{
  namespace Kaskade
  {
    using namespace boost::fusion;
    using namespace AssemblyDetail;

    enum class ErrorNorm { Energy, L2, H1, H1_half };

    template <class ArgY>
    double computeL2Error(const ArgY& y)
    {
      return y.value*y.value;
    }

    template <class ArgY>
    double computeH1HalfError(const ArgY& y)
    {
      return ::Kaskade::LinAlg::EuclideanScalarProduct()(y.gradient,y.gradient);
    }


    template <class Functional, class ExtendedAnsatzVars >
    class ErrorDistribution
    {
      typedef typename Functional::AnsatzVars OriginalAnsatzVars;
      typedef typename OriginalAnsatzVars::Grid Grid;
      typedef ::Kaskade::ShapeFunctionCache<Grid, typename Functional::Scalar> SfCache;
      typedef ::Kaskade::ShapeFunctionCache<Grid, typename Functional::Scalar> SfCache2;
      typedef typename OriginalAnsatzVars::Spaces OriginalSpaces;
      typedef typename ExtendedAnsatzVars::Spaces ExtendedSpaces;
      typedef typename result_of::as_vector<typename result_of::transform<OriginalSpaces, GetEvaluators<SfCache> >::type>::type OriginalEvaluators;
      typedef typename result_of::as_vector<typename result_of::transform<ExtendedSpaces, GetEvaluators<SfCache2> >::type>::type ExtendedEvaluators;
      typedef typename Grid::ctype CoordType;
      typedef Dune::QuadratureRule<typename Functional::AnsatzVars::Grid::ctype, Functional::AnsatzVars::Grid::dimension> QuadRule;
      typedef Dune::QuadratureRule<typename Functional::AnsatzVars::Grid::ctype, Functional::AnsatzVars::Grid::dimension-1> FaceQuadRule;
    public:
      using Scalar = typename Functional::Scalar;
      using AnsatzSpace = ::Kaskade::FEFunctionSpace<DiscontinuousLagrangeMapper<Scalar,typename Grid::LeafGridView> >;
      using AnsatzSpaces = boost::fusion::vector<AnsatzSpace const*>;
      using AnsatzVariableInformation = ::Kaskade::Variable<SpaceIndex<0>,Components<1>,VariableId<0> >;
      using VariableDescriptions = boost::fusion::vector<AnsatzVariableInformation>;
      using AnsatzVars = ::Kaskade::VariableSetDescription<AnsatzSpaces,VariableDescriptions>;
      using TestVars = AnsatzVars;
      using OriginVars = AnsatzVars;
      using ErrorVector = typename AnsatzVars::template CoefficientVectorRepresentation<>::type;
      using Cell = typename AnsatzVars::Grid::template Codim<0>::Entity;

      static int const dim = Grid::dimension;
      static ProblemType const type = Functional::type;

      static constexpr int yIdx = 0;
      static constexpr int spaceIndex = result_of::value_at_c<typename OriginalAnsatzVars::Variables, yIdx>::type::spaceIndex;
      static constexpr int extensionSpaceIndex = result_of::value_at_c<typename ExtendedAnsatzVars::Variables, yIdx>::type::spaceIndex;

      class DomainCache
      {
      public:
        DomainCache(ErrorDistribution const& f_, typename OriginVars::VariableSet const& /*vars*/, int flags=7)
          : f(f_), domainCache(f.functional,f.iterate,flags)
        {
        }

        template <class Entity>
        void moveTo(Entity const &entity)
        {
          e  = &entity;
          domainCache.moveTo(entity);
        }

        template <class Position, class Evaluators>
        void evaluateAt(Position const& x_, Evaluators const& evaluators)
        {
          x = x_;
          OriginalEvaluators originalEvaluators(transform(f.iterate.descriptions.spaces,::Kaskade::GetEvaluators<SfCache>(&sfCache)));
          ExtendedEvaluators extendedEvaluators(transform(f.errorEstimateH.descriptions.spaces,::Kaskade::GetEvaluators<SfCache>(&extendedSFCache)));
          QuadRule qr = QuadratureTraits<QuadRule>().rule(e->type(),f.qOrder);
          moveEvaluatorsToCell(originalEvaluators,*e);
          moveEvaluatorsToCell(extendedEvaluators,*e);
          useQuadratureRuleInEvaluators(originalEvaluators,qr,0);
          useQuadratureRuleInEvaluators(extendedEvaluators,qr,0);

          size_t nQuadPos = qr.size();
          for (size_t g=0; g<nQuadPos; ++g)
          {
            auto w = qr[g].weight();
            // pos of integration point
            Dune::FieldVector<CoordType,dim> quadPos = qr[g].position();
            // for all spaces involved, update the evaluators associated
            // to this quadrature point
            moveEvaluatorsToIntegrationPoint(originalEvaluators,quadPos);
            moveEvaluatorsToIntegrationPoint(extendedEvaluators,quadPos);

            // prepare evaluation of functional
            domainCache.evaluateAt(qr[g].position(),originalEvaluators);

            y_e.value += w * at_c<yIdx>(f.errorEstimateH.data).value(at_c<extensionSpaceIndex>(extendedEvaluators));
            y_h.value += w * at_c<yIdx>(f.errorEstimateL.data).value(at_c<spaceIndex>(originalEvaluators));
            y_e.gradient += w * at_c<yIdx>(f.errorEstimateH.data).gradient(at_c<extensionSpaceIndex>(extendedEvaluators));
            y_h.gradient += w * at_c<yIdx>(f.errorEstimateL.data).gradient(at_c<spaceIndex>(originalEvaluators));
          }
        }

        Scalar d0() const { return 0; }

        template<int row, int dim>
        Dune::FieldVector<Scalar, TestVars::template Components<row>::m>
        d1 (::Kaskade::VariationalArg<Scalar,dim> const& arg) const
        {
          ::Kaskade::VariationalArg<Scalar,dim,OriginalAnsatzVars::template Components<yIdx>::m> y(y_e);

          if( !f.onlyExtension )
          {
            y.value += y_h.value;
            y.gradient += y_h.gradient;
          }

          if( f.errorNorm == ErrorNorm::L2 ) return computeL2Error(y);
          if( f.errorNorm == ErrorNorm::H1_half ) return computeH1HalfError(y);
          if( f.errorNorm == ErrorNorm::H1 ) return computeL2Error(y) + computeH1HalfError(y);

          return ( Functional::template D2<yIdx,yIdx>::present ? domainCache.template d2_impl<yIdx,yIdx>(y,y) : 0. ) * arg.value[0];
        }

        template<int row, int col, int dim>
        auto d2 (::Kaskade::VariationalArg<Scalar,dim> const&, ::Kaskade::VariationalArg<Scalar,dim> const&) const
        {
          return Dune::FieldMatrix<Scalar,1,1>(0);
        }

      private:
        ErrorDistribution const& f;
        typename Functional::DomainCache domainCache;
        const Cell* e;
        Dune::FieldVector<Scalar,dim> x;
        ::Kaskade::VariationalArg<Scalar,dim,OriginalAnsatzVars::template Components<yIdx>::m> y_h, y_e;
        SfCache sfCache;
        SfCache2 extendedSFCache;
        double value = 0;
      };

      class BoundaryCache
      {
      public:
        BoundaryCache(ErrorDistribution const& f_, typename OriginVars::VariableSet const& vars, int flags=7)
          : f(f_), boundaryCache(f.functional,f.iterate,flags)
        {}

        template <class FaceIterator>
        void moveTo(FaceIterator const& entity)
        {
          e = &entity;
          boundaryCache.moveTo(entity);
        }

        template <class Evaluators>
        void evaluateAt(Dune::FieldVector<typename AnsatzVars::Grid::ctype, AnsatzVars::Grid::dimension-1> const& xlocal, Evaluators const& evaluators)
        {
//          OriginalEvaluators originalEvaluators(transform(f.iterate.descriptions.spaces,GetEvaluators<SfCache>(&sfCache)));
//          ExtendedEvaluators extendedEvaluators(transform(f.errorEstimateH.descriptions.spaces,GetEvaluators<SfCache>(&extendedSFCache)));
//          FaceQuadRule qr = QuadratureTraits<FaceQuadRule>().rule((*e)->geometryInInside().type(),f.qOrder);


//          moveEvaluatorsToCell(originalEvaluators,(*e)->inside());
//          moveEvaluatorsToCell(extendedEvaluators,(*e)->inside());
//          moveEvaluatorsToCell(originalEvaluators,*e);
//          moveEvaluatorsToCell(extendedEvaluators,*e);
//          useQuadratureRuleInEvaluators(originalEvaluators,qr,(*e)->indexInInside());
//          useQuadratureRuleInEvaluators(extendedEvaluators,qr,(*e)->indexInInside());

//          size_t nQuadPos = qr.size();
//          for (size_t g=0; g<nQuadPos; ++g)
//          {
//            auto w = qr[g].weight();
//            // pos of integration point
//            Dune::FieldVector<CoordType,dim> quadPos = (*e)->geometryInInside().global(qr[g].position());
//            // for all spaces involved, update the evaluators associated
//            // to this quadrature point
//            moveEvaluatorsToIntegrationPoint(originalEvaluators,quadPos);
//            moveEvaluatorsToIntegrationPoint(extendedEvaluators,quadPos);
//            // prepare evaluation of functional
//            boundaryCache.evaluateAt(qr[g].position(),originalEvaluators);

//            y_e.value += w * at_c<yIdx>(f.errorEstimateH.data).value(at_c<extensionSpaceIndex>(extendedEvaluators));
//            y_h.value += w * at_c<yIdx>(f.errorEstimateL.data).value(at_c<spaceIndex>(originalEvaluators));
//            y_e.gradient += w * at_c<yIdx>(f.errorEstimateH.data).gradient(at_c<extensionSpaceIndex>(extendedEvaluators));
//            y_h.gradient += w * at_c<yIdx>(f.errorEstimateL.data).gradient(at_c<spaceIndex>(originalEvaluators));
//          }
        }

        Scalar d0() const
        {
          return 0;
        }

        template<int row, int dim>
        Dune::FieldVector<Scalar,1> d1 (::Kaskade::VariationalArg<Scalar,dim> const& arg) const
        {
          return Dune::FieldVector<Scalar,1>(0);
//          ::Kaskade::VariationalArg<Scalar,dim,OriginalAnsatzVars::template Components<yIdx>::m> y(y_e);

//          if( !f.onlyExtension )
//          {
//            y.value += y_h.value;
//            y.gradient += y_h.gradient;
//          }

//          if( f.errorNorm != ErrorNorm::Energy ) return computeL2Error(y);

//          return ( Functional::template D2<yIdx,yIdx>::present ? boundaryCache.template d2_impl<yIdx,yIdx>(y,y) : 0. ) * arg.value[0];
        }

        template<int row, int col, int dim>
        Dune::FieldMatrix<Scalar,1,1> d2 (VariationalArg<Scalar,dim> const&, VariationalArg<Scalar,dim> const&) const { return Dune::FieldMatrix<Scalar,1,1>(0); }

      private:
        ErrorDistribution const& f;
        typename Functional::BoundaryCache boundaryCache;
        typename AnsatzVars::Grid::LeafGridView::IntersectionIterator const* e;
        VariationalArg<Scalar,dim,OriginalAnsatzVars::template Components<yIdx>::m> y_h;
        VariationalArg<Scalar,dim,ExtendedAnsatzVars::template Components<yIdx>::m> y_e;
        SfCache sfCache;
        SfCache2 extendedSFCache;
      };

      ErrorDistribution(Functional const& functional_, typename OriginalAnsatzVars::VariableSet const& iterate_,
                        typename OriginalAnsatzVars::VariableSet const& errorEstimateL_, typename ExtendedAnsatzVars::VariableSet const& errorEstimateH_)
        : functional(functional_), iterate(iterate_), errorEstimateL(errorEstimateL_), errorEstimateH(errorEstimateH_),
          ansatzSpace(boost::fusion::at_c<0>(iterate.descriptions.spaces)->gridManager(), iterate.descriptions.gridView,0),
          ansatzSpaces(&ansatzSpace), varName( {"error"} ), ansatzVars(ansatzSpaces,varName)

      {}

      template <int row>
      struct D1 : FunctionalBase<type>::template D1<row>
      {
        static bool const present   = true;
        static bool const constant  = false;
      };

      template <int row, int col>
      struct D2 : FunctionalBase<type>::template D2<row,col>
      {
        static bool const present = false;
        static bool const symmetric = false;
        static bool const lumped = false;
      };

      template <class Cell>
      int integrationOrder(Cell const&, int, bool) const { return 0; }

      AnsatzSpaces const& spaces() const { return ansatzSpaces; }

      AnsatzVars const& variableSetDescription() const { return ansatzVars; }

    private:
      friend class DomainCache;
      friend class BoundaryCache;

      Functional const& functional;
      typename OriginalAnsatzVars::VariableSet const& iterate;
      typename OriginalAnsatzVars::VariableSet const& errorEstimateL;
      typename ExtendedAnsatzVars::VariableSet const& errorEstimateH;
      AnsatzSpace ansatzSpace;
      AnsatzSpaces ansatzSpaces;
      std::string varName[1];
      AnsatzVars ansatzVars;
      ErrorNorm errorNorm = ErrorNorm::Energy;
      bool onlyExtension = false;
      int qOrder = 6;
    };
  }
}

#endif // SPACY_ADAPTER_KASKADE_ERROR_DISTRIBUTION_HH
