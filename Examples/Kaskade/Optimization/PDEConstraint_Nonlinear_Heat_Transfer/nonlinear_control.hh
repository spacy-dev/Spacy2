/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*  This file is part of the library KASKADE 7                               */
/*    see http://www.zib.de/Numerik/numsoft/kaskade7/                        */
/*                                                                           */
/*  Copyright (C) 2002-2009 Zuse Institute Berlin                            */
/*                                                                           */
/*  KASKADE 7 is distributed under the terms of the ZIB Academic License.    */
/*    see $KASKADE/academic.txt                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef NONLINEAR_CONTROL_HH
#define NONLINEAR_CONTROL_HH

#include <memory>
#include <type_traits>

#include "fem/variables.hh"
#include "utilities/linalg/scalarproducts.hh"
#include "fem/diffops/trackingTypeCostFunctional.hh"
#include "fem/diffops/antonsNonlinearTestProblems.hh"

/// \cond
using namespace Kaskade;


/****************************************************************************************/
/* Boundary */
/****************************************************************************************/
enum class RoleOfFunctional{ NORMAL, TANGENTIAL };

template <int stateId, int controlId, int adjointId, class RType, class AnsatzVars_, class TestVars_=AnsatzVars_, class OriginVars_=AnsatzVars_, RoleOfFunctional role = RoleOfFunctional::NORMAL, bool lump=false>
class StepFunctional
{
public:
  typedef RType  Scalar;
  typedef AnsatzVars_ AnsatzVars;
  typedef TestVars_ TestVars;
  typedef OriginVars_ OriginVars;
  static constexpr int dim = AnsatzVars::Grid::dimension;
  static ProblemType const type = std::is_same<AnsatzVars,TestVars>::value ? VariationalFunctional : WeakFormulation;
  
  typedef typename AnsatzVars::Grid Grid;
  typedef typename Grid::template Codim<0>::Entity Cell;
  
  static int const uIdx = controlId;
  static int const yIdx = stateId;
  static int const pIdx = adjointId;
  static int const ySIdx = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables,yIdx>::type::spaceIndex;
  static int const uSIdx = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables,uIdx>::type::spaceIndex;
  static int const pSIdx = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables,pIdx>::type::spaceIndex;
  
  
  class DomainCache : public CacheBase<StepFunctional,DomainCache>
  {
  public:
    DomainCache(StepFunctional const& f_,
                typename AnsatzVars::VariableSet const& vars_,int flags=7):
      f(f_), vars(vars_), c(f.c,f.d), J(f.J)
    {}
    
    template <class Position, class Evaluators>
    void evaluateAt(Position const& x, Evaluators const& evaluators)
    {
      using namespace boost::fusion;
      y = at_c<yIdx>(vars.data).value(at_c<ySIdx>(evaluators));
      u = at_c<uIdx>(vars.data).value(at_c<uSIdx>(evaluators));
      p.value = at_c<pIdx>(vars.data).value(at_c<pSIdx>(evaluators));
      
      dy = at_c<yIdx>(vars.data).gradient(at_c<ySIdx>(evaluators));
      p.gradient = at_c<pIdx>(vars.data).gradient(at_c<pSIdx>(evaluators));
      
      c.evaluateAt(y,dy);
      J.evaluateAt(y,u,evaluators);
      kappa = f.d * ( y * y ) + f.c;
    }
    
    Scalar d0() const
    {
      return J.d0();
    }
    
    template<int row>
    Scalar d1_impl (VariationalArg<Scalar,dim, TestVars::template Components<row>::m> const& arg) const
    {
      if(row==yIdx)
      {
        return J.template d1<yIdx>(arg) + c.template d2<yIdx,yIdx>(p,arg);
      }
      if(row==uIdx) return J.template d1<uIdx>(arg) - p.value*if_(arg.value,u);
      if(row==pIdx) return c.template d1<yIdx>(arg) - u*if_(arg.value,p.value);
      return 0;
    }
    
    template<int row, int col>
    Scalar d2_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const &arg1, VariationalArg<Scalar,dim,TestVars::template Components<col>::m> const &arg2) const
    {
      if(row==yIdx && col==yIdx)
      {
        if(role == RoleOfFunctional::TANGENTIAL )
          return J.template d2<yIdx,yIdx>(arg1,arg2) + c.template d3<yIdx,yIdx,yIdx>(p,arg1,arg2);
	else
	  return kappa*sp(if_(arg1.gradient,dy),if_(arg2.gradient,dy)) + J.template d2<yIdx,yIdx>(arg1,arg2);
      }
      
      if(row==uIdx && col==uIdx)
        return J.template d2<uIdx,uIdx>(arg1,arg2);
      
      if(row==pIdx && col==uIdx) return - if_(arg1.value,p.value) * if_(arg2.value,u);
      if(row==uIdx && col==pIdx) return - if_(arg1.value,u) * if_(arg2.value,p.value);
      
      if(row==yIdx && col==pIdx) return c.template d2<yIdx,yIdx>(arg2,arg1);
      if(row==pIdx && col==yIdx) return c.template d2<yIdx,yIdx>(arg1,arg2);
      return 0;
    }
    
  private:
    StepFunctional const& f;
    typename AnsatzVars::VariableSet const& vars;
    Dune::FieldVector<Scalar,AnsatzVars::template Components<yIdx>::m> y, y_z;
    Dune::FieldVector<Scalar,/*1*/AnsatzVars::template Components<uIdx>::m> u;
    Dune::FieldMatrix<Scalar,AnsatzVars::template Components<yIdx>::m,dim> dy;
    VariationalArg<Scalar,dim,AnsatzVars::template Components<pIdx>::m> p;
    AntonsNonlinearTestProblem<AnsatzVars,yIdx> c;
    TrackingTypeCostFunctional<typename AnsatzVars::VariableSet,yIdx,uIdx> J;
    LinAlg::EuclideanScalarProduct sp;
    double kappa;
  };
  
  class BoundaryCache : public CacheBase<StepFunctional,BoundaryCache>
  {
  public:
    BoundaryCache(StepFunctional const& f,
                  typename AnsatzVars::VariableSet const& vars_, int flags=7):
      vars(vars_), gamma(f.gamma)
    {}
    
    template <class Evaluators>
    void evaluateAt(Dune::FieldVector<typename AnsatzVars::Grid::ctype,AnsatzVars::Grid::dimension-1> const& x, Evaluators const& evaluators)
    {
      using namespace boost::fusion;
      y = at_c<yIdx>(vars.data).value(at_c<ySIdx>(evaluators));
      p = at_c<pIdx>(vars.data).value(at_c<pSIdx>(evaluators));
    }
    
    Scalar d0() const
    {
      return 0;
    }
    
    template<int row>
    Scalar d1_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const& arg) const
    {
      if(row==yIdx) return gamma*(p*if_(arg.value,y));
      if(row==pIdx) return gamma*(y*if_(arg.value,p));
      return 0;
    }
    
    template<int row, int col>
    Scalar d2_impl (VariationalArg<Scalar,dim,TestVars::template Components<row>::m> const &arg1, VariationalArg<Scalar,dim,AnsatzVars::template Components<col>::m> const &arg2) const
    {
      if( (row==yIdx && col==pIdx) || (row==pIdx && col==yIdx) ) return gamma*(if_(arg1.value,y) * if_(arg2.value,p));
      return 0;
    }
    
  private:
    typename AnsatzVars::VariableSet const& vars;
    Scalar gamma;
    Dune::FieldVector<Scalar,AnsatzVars::template Components<yIdx>::m> y;
    Dune::FieldVector<Scalar,AnsatzVars::template Components<pIdx>::m> p;
  };
  
  
  explicit StepFunctional(Scalar alpha_, typename AnsatzVars::VariableSet const& ref, Scalar c_=1, Scalar d_=1) :
    gamma(1e9), c(c_), d(d_), alpha(alpha_), J(alpha,ref)
  {
    assert(gamma >= 0);
  }
  
  template <class T>
  bool inDomain(T const&) const
  {
    return true;
  }
  
  template <int row>
  struct D1 : public FunctionalBase<WeakFormulation>::D1<row>
  {
    static bool const present = true;
    static bool const constant = false;
  };
  
  template <int row, int col>
  struct D2 : public FunctionalBase<WeakFormulation>::D2<row,col>
  {
    static bool const present = !( ( row == pIdx && col == pIdx ) || ( row == yIdx && col == uIdx ) || ( row == uIdx && col == yIdx ) );
    static bool const symmetric = row==col;
    static bool const lumped = lump;
  };
  
  template <class Cell>
  int integrationOrder(Cell const& /* cell */, int shapeFunctionOrder, bool  boundary ) const
  {
    if( boundary ) return 2*shapeFunctionOrder;
    return 4*shapeFunctionOrder - 2;
  }
  
  Scalar gamma,  c, d, alpha;
  TrackingTypeCostFunctional<typename AnsatzVars::VariableSet,yIdx,uIdx> J;
};


template <int stateId, int controlId, int adjointId, class RType, class AnsatzVars, class TestVars=AnsatzVars, class OriginVars=AnsatzVars>
using NormalStepFunctional = StepFunctional<stateId,controlId,adjointId,RType,AnsatzVars,TestVars,OriginVars,RoleOfFunctional::NORMAL>;

template <int stateId, int controlId, int adjointId, class RType, class AnsatzVars, class TestVars=AnsatzVars, class OriginVars=AnsatzVars>
using TangentialStepFunctional = StepFunctional<stateId,controlId,adjointId,RType,AnsatzVars,TestVars,OriginVars,RoleOfFunctional::TANGENTIAL>;
/// \endcond
#endif
