#ifndef SPACY_KASKADE_DLY_DEFINITION_HH
#define SPACY_KASKADE_DLY_DEFINITION_HH

#include "fem/functional_aux.hh"
#include "fem/fixdune.hh"

using namespace Kaskade;

/// \cond
namespace Spacy
{
  namespace Kaskade
  {

    template <class Definition>
    class LumpedDefinition : public FunctionalBase<Definition::type>
    {
    public:
      using Scalar = typename Definition::Scalar;
      using OriginVars = typename Definition::OriginVars;
      using AnsatzVars = typename Definition::AnsatzVars;
      using TestVars = typename Definition::TestVars;
      using Cell = typename AnsatzVars::Grid::template Codim<0>::Entity;
      static const ::Kaskade::ProblemType type = Definition::type;

      class DomainCache : public CacheBase<LumpedDefinition,DomainCache>
      {
      public:
        DomainCache(LumpedDefinition const& f,
                    typename AnsatzVars::VariableSet const& vars,
                    int flags=7)
          : domainCache(f.f_,vars,flags)
        {}

        void moveTo(const Cell& cell)
        {
          domainCache.moveTo(cell);
        }


        template <class Position, class Evaluators>
        void evaluateAt(Position const& x, Evaluators const& evaluators)
        {
          domainCache.evaluateAt(x,evaluators);
        }

        Scalar
        d0() const
        {
          return domainCache.d0();
        }

        template<int row,int dim>
        Scalar d1(const VariationalArg<Scalar,dim>& arg) const
        {
          domainCache.template d1<row>(arg);
        }

        template<int row, int col, int dim>
        Scalar d2(const VariationalArg<Scalar,dim>& arg1,
                  const VariationalArg<Scalar,dim>& arg2) const
        {
         return domainCache.template d2<row,col>(arg1,arg2);
        }

      private:
        typename Definition::DomainCache domainCache;
      };

      class BoundaryCache : public CacheBase<LumpedDefinition,BoundaryCache>
      {
      public:
        BoundaryCache(LumpedDefinition const& f,
                      typename AnsatzVars::VariableSet const& vars,
                      int flags=7)
          : boundaryCache(f.f_,vars,flags)
        {}

        template <class Position, class Evaluators>
        void evaluateAt(Position const& x, Evaluators const& evaluators)
        {
          boundaryCache.evaluateAt(x,evaluators);
        }

        template <class FaceIterator>
        void moveTo(const FaceIterator& face)
        {
          boundaryCache.moveTo(face);
        }

        Scalar d0() const
        {
          return boundaryCache.d0();
        }

        template<int row, int dim>
        Scalar d1(const VariationalArg<Scalar,dim>& dx) const
        {
          return boundaryCache.template d1<row>(dx);
        }

        template<int row, int col, int dim>
        Scalar d2(const VariationalArg<Scalar,dim>& dx,
                  const VariationalArg<Scalar,dim>& dy) const
        {
          return boundaryCache.template d2<row,col>(dx,dy);
        }

      private:
        typename Definition::BoundaryCache boundaryCache;
      };

      template <int row>
      struct D1: public FunctionalBase<Definition::type>::template D1<row>
      {
        static bool const present   = Definition::template D1<row>::present;
        static bool const constant  = Definition::template D1<row>::constant;

      };

      template <int row, int col>
      struct D2: public FunctionalBase<Definition::type>::template D2<row,col>
      {
        static bool const present = Definition::template D2<row,col>::present;
        static bool const symmetric = Definition::template D2<row,col>::symmetric;
        static bool const lumped = true;
      };

      template <class Cell>
      int integrationOrder(Cell const&  cell , int shapeFunctionOrder, bool boundary) const
      {
        return f_.integrationOrder(cell,shapeFunctionOrder,boundary);
      }

      template <class... Args>
      LumpedDefinition(Args... args)
        : f_(std::forward<Args>(args)...)
      {}

      Definition f_;
    };
  }
}
/// \endcond

#endif // SPACY_KASKADE_DLY_DEFINITION_HH
