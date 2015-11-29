/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*  This file is part of the library KASKADE 7                               */
/*    see http://www.zib.de/en/numerik/software/kaskade-7.html               */
/*                                                                           */
/*  Copyright (C) 2002-2015 Zuse Institute Berlin                            */
/*                                                                           */
/*  KASKADE 7 is distributed under the terms of the ZIB Academic License.    */
/*    see $KASKADE/academic.txt                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SPACY_TEST_ADAPTER_KASKADE_HEAT_TRANSFER_HH
#define SPACY_TEST_ADAPTER_KASKADE_HEAT_TRANSFER_HH

#include <algorithm>
#include "fem/functional_aux.hh"
#include "fem/fixdune.hh"
#include "fem/variables.hh"
#include "utilities/linalg/scalarproducts.hh"

using namespace Kaskade;

template <class RType, class VarSet>
class HeatFunctional : public FunctionalBase<VariationalFunctional>
{
public:
  using Scalar = RType;
  using OriginVars = VarSet;
  using AnsatzVars = VarSet;
  using TestVars = VarSet;

  static constexpr int dim = AnsatzVars::Grid::dimension;
  static constexpr int uIdx = 0;
  static constexpr int uSpaceIdx = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables,
                                             uIdx>::type::spaceIndex;

  class DomainCache : public CacheBase<HeatFunctional,DomainCache>
  {
  public:
    DomainCache(HeatFunctional const&,
                typename AnsatzVars::VariableSet const& vars_,
                int flags=7):
      data(vars_)
    {}


    template <class Position, class Evaluators>
    void evaluateAt(Position const& x, Evaluators const& evaluators)
    {
      u  = boost::fusion::at_c<uIdx>(data.data).value(boost::fusion::at_c<uSpaceIdx>(evaluators));
      du = boost::fusion::at_c<uIdx>(data.data).derivative(boost::fusion::at_c<uSpaceIdx>(evaluators));
      f = 1.0;
    }

    Scalar
    d0() const
    {
      return sp(du,du)/2 - f*u;
    }

    template<int row>
    Scalar d1_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const& arg) const
    {
      return sp(du,arg.derivative) - f*arg.value;
    }

    template<int row, int col>
    Scalar d2_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const &arg1,
                  VariationalArg<Scalar,dim,AnsatzVars::template Components<row>::m> const &arg2) const
    {
      return sp(arg1.derivative,arg2.derivative);
    }

  private:
    typename AnsatzVars::VariableSet const& data;
    Dune::FieldVector<Scalar,AnsatzVars::template Components<uIdx>::m> u, f;
    Dune::FieldMatrix<Scalar,AnsatzVars::template Components<uIdx>::m,dim> du;
    LinAlg::EuclideanScalarProduct sp;
  };


  class BoundaryCache : public CacheBase<HeatFunctional,BoundaryCache>
  {
  public:
    BoundaryCache(HeatFunctional<RType,AnsatzVars> const&,
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

  template <int row>
  struct D1: public FunctionalBase<VariationalFunctional>::D1<row>
  {
    static bool const present   = true;
    static bool const constant  = false;

  };

  template <int row, int col>
  struct D2: public FunctionalBase<VariationalFunctional>::D2<row,col>
  {
    static bool const present = true;
    static bool const symmetric = true;
    static bool const lumped = false;
  };

  template <class Cell>
  int integrationOrder(Cell const& /* cell */, int shapeFunctionOrder, bool boundary) const
  {
    if (boundary)
      return 2*shapeFunctionOrder;
    else
    {
      int stiffnessMatrixIntegrationOrder = 2*(shapeFunctionOrder-1);
      int sourceTermIntegrationOrder = shapeFunctionOrder;        // as rhs f is constant, i.e. of order 0

      return std::max(stiffnessMatrixIntegrationOrder,sourceTermIntegrationOrder);
    }
  }
};

#endif // SPACY_TEST_ADAPTER_KASKADE_HEAT_TRANSFER_HH
