/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*  This file is part of the library KASKADE 7                               */
/*    see http://www.zib.de/Numerik/numsoft/kaskade7/                        */
/*                                                                           */
/*  Copyright (C) 2002-2014 Zuse Institute Berlin                            */
/*                                                                           */
/*  KASKADE 7 is distributed under the terms of the ZIB Academic License.    */
/*    see $KASKADE/academic.txt                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#pragma once

#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>

#include "fem/fixdune.hh"
#include "utilities/linalg/scalarproducts.hh"

#include "fem/variables.hh"
#include "fem/functional_aux.hh"

namespace Kaskade
{
  /**
       * @brief linear constraint for optimal control problems
       */
  template <class AnsatzVars_, int stateId = 0>
  class LinearModelConstraint
  {
  public:
    typedef AnsatzVars_ AnsatzVars;
    typedef typename AnsatzVars::Scalar Scalar;
    static constexpr int dim = AnsatzVars::Grid::dimension;

    /**
     * @brief Construct linear constraint
     * @param d factor before laplacian, smaller d -> harder problem
     * @param mu instability factor, mu zero -> stable as laplace is stable, mu > 0 -> more stable , mu < 0 -> instable
     */
    explicit LinearModelConstraint(Scalar d_,Scalar mu_):mu(mu_),d(d_) { }

    void evaluateAt(Dune::FieldVector<Scalar,AnsatzVars::template Components<stateId>::m> const& y_,
                    Dune::FieldMatrix<Scalar,AnsatzVars::template Components<stateId>::m,dim> const& dy_)
    {
      y = y_;
      dy = dy_;
    }


    template<int row>
    Scalar d1 (VariationalArg<Scalar,dim, AnsatzVars::template Components<row>::m> const& arg) const
    {
      if( row != stateId ) return 0;
      return d*sp(dy,arg.derivative) + mu*y*arg.value;
    }

    template<int row, int col>
    Scalar d2 (VariationalArg<Scalar,dim,AnsatzVars::template Components<row>::m> const &arg1, VariationalArg<Scalar,dim,AnsatzVars::template Components<col>::m> const &arg2) const
    {
      if ( row != stateId || col != stateId ) return 0;
      return  d*sp(arg2.derivative,arg1.derivative) + mu*arg2.value*arg1.value;
    }

    template <int id1, int id2, int id3>
    Scalar d3 (VariationalArg<Scalar,dim,AnsatzVars::template Components<id1>::m> const& arg1,
               VariationalArg<Scalar,dim,AnsatzVars::template Components<id2>::m> const& arg2,
               VariationalArg<Scalar,dim,AnsatzVars::template Components<id3>::m> const& arg3) const
    {
      if( id1 != stateId || id2 != stateId || id3 != stateId ) return 0;
      return  0;
    }

  private:
    Scalar d,mu;
    Dune::FieldVector<Scalar,AnsatzVars::template Components<stateId>::m> y;
    Dune::FieldMatrix<Scalar,AnsatzVars::template Components<stateId>::m,dim> dy;
    LinAlg::EuclideanScalarProduct sp;
  };

  /**
       * @brief linear pde with nonconstant source
       */
  template <class RType, class VarSet, class DescriptionControl>
  class LinearModelPDE : public FunctionalBase<WeakFormulation>
  {
  public:
    using Scalar = RType;
    using OriginVars = VarSet;
    using AnsatzVars = VarSet;
    using TestVars = VarSet;

    using SourceVars = DescriptionControl;

    static constexpr int dim = AnsatzVars::Grid::dimension;
    static constexpr int uIdx = 0;
    static constexpr int uSpaceIdx = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables,
                                               uIdx>::type::spaceIndex;

    class DomainCache : public CacheBase<LinearModelPDE,DomainCache>
    {
    public:
      DomainCache(LinearModelPDE const& h_,
                  typename AnsatzVars::VariableSet const& vars_,
                  int flags=7):
        h(h_), data(vars_)
      {}

      template <class Entity>
      void moveTo(Entity const& entity)
      {
        e = entity;
      }

      template <class Position, class Evaluators>
      void evaluateAt(Position const& x, Evaluators const& evaluators)
      {
        u  = boost::fusion::at_c<uIdx>(data.data).value(boost::fusion::at_c<uSpaceIdx>(evaluators));
        du = boost::fusion::at_c<uIdx>(data.data).derivative(boost::fusion::at_c<uSpaceIdx>(evaluators));

        f = boost::fusion::at_c<0>(h.control_.data).value(e ,x);
      }

      Scalar
      d0() const
      {
        return sp(du,du)/2 - f*u;
      }

      template<int row>
      Scalar d1_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const& arg) const
      {
        return h.d*sp(du,arg.derivative) + h.mu*u*arg.value - f*arg.value;
      }

      template<int row, int col>
      Scalar d2_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const &arg1,
                    VariationalArg<Scalar,dim,AnsatzVars::template Components<row>::m> const &arg2) const
      {
        return h.d*sp(arg2.derivative,arg1.derivative) + h.mu*arg1.value*arg2.value;
      }

    private:
      LinearModelPDE const& h;
      typename AnsatzVars::VariableSet const& data;
      Dune::FieldVector<Scalar,AnsatzVars::template Components<uIdx>::m> u, f, dkappa;
      Dune::FieldMatrix<Scalar,AnsatzVars::template Components<uIdx>::m,dim> du;
      LinAlg::EuclideanScalarProduct sp;

      typename AnsatzVars::Grid::template Codim<0>::Entity e;
    };

    class BoundaryCache : public CacheBase<LinearModelPDE,BoundaryCache>
    {
    public:
      BoundaryCache(LinearModelPDE<RType,AnsatzVars,DescriptionControl> const&,
                    typename AnsatzVars::VariableSet const& vars_,
                    int flags=7):
        data(vars_), penalty(1e9), u(0.), uDirichletBoundaryValue(0.)
      {}

      template <class Position, class Evaluators>
      void evaluateAt(Position const&, Evaluators const& evaluators)
      {
        u = boost::fusion::at_c<uIdx>(data.data).value(boost::fusion::at_c<uSpaceIdx>(evaluators));
      }

      Scalar d0() const
      {
        return penalty*(u-uDirichletBoundaryValue)*(u-uDirichletBoundaryValue)/2;
      }

      template<int row>
      Scalar d1_impl (VariationalArg<Scalar,dim> const& arg) const
      {
        return penalty*(u-uDirichletBoundaryValue)*arg.value;
      }

      template<int row, int col>
      Scalar d2_impl (VariationalArg<Scalar,dim> const &arg1,
                      VariationalArg<Scalar,dim> const &arg2) const
      {
        return penalty*arg1.value*arg2.value;
      }

    private:
      typename AnsatzVars::VariableSet const& data;
      Scalar penalty;
      Dune::FieldVector<Scalar,AnsatzVars::template Components<uIdx>::m> u, uDirichletBoundaryValue;
    };

    /**
     * @brief Construct linear pde
     * @param d_ factor before laplacian, smaller d -> harder problem
     * @param mu_ instability factor, mu zero -> stable as laplace is stable, mu > 0 -> more stable , mu < 0 -> instable
     * @param control_ source term of the pde
     */
    explicit LinearModelPDE(Scalar d_, Scalar mu_, typename DescriptionControl::VariableSet control): d(d_),mu(mu_),control_(control) {}
    template <int row>
    struct D1: public FunctionalBase<WeakFormulation>::D1<row>
    {
      static bool const present   = true;
      static bool const constant  = false;

    };

    template <int row, int col>
    struct D2: public FunctionalBase<WeakFormulation>::D2<row,col>
    {
      static bool const present = true;
      static bool const symmetric = false;
      static bool const lumped = false;
    };

    template <class Cell>
    int integrationOrder(Cell const& /* cell */, int shapeFunctionOrder, bool boundary) const
    {
      if (boundary)
        return 2*shapeFunctionOrder;
      else
      {
        int stiffnessMatrixIntegrationOrder = 2*(shapeFunctionOrder-1) + 2*shapeFunctionOrder;
        int sourceTermIntegrationOrder = shapeFunctionOrder;        // as rhs f is constant, i.e. of order 0

        return std::max(stiffnessMatrixIntegrationOrder,sourceTermIntegrationOrder);
      }
    }
    Scalar d, mu;
    typename DescriptionControl::VariableSet control_;

  };

}

