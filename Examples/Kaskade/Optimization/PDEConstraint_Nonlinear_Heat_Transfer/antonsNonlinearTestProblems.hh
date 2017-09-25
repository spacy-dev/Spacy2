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
#ifndef ANTONSNONLINEARTESTPROBLEM_1_HH
#define ANTONSNONLINEARTESTPROBLEM_1_HH

#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>

#include "fem/fixdune.hh"
#include "utilities/linalg/scalarproducts.hh"

namespace Kaskade
{
  template <class AnsatzVars_, int stateId = 0>
  class AntonsNonlinearTestProblem
  {
  public:
    typedef AnsatzVars_ AnsatzVars;
    typedef typename AnsatzVars::Scalar Scalar;
    static constexpr int dim = AnsatzVars::Grid::dimension;

    explicit AntonsNonlinearTestProblem(Scalar c_=1, Scalar d_=1, Scalar e_=0) : c(c_), d(d_), e(e_) 
    {
    }

    void evaluateAt(Dune::FieldVector<Scalar,AnsatzVars::template Components<stateId>::m> const& y_,
                    Dune::FieldMatrix<Scalar,AnsatzVars::template Components<stateId>::m,dim> const& dy_)
    {
      y = y_;
      dy = dy_;

      kappa   = d+c*y*y;
      dkappa  = c*2*y;
      ddkappa = c*2*unitMatrix<Scalar,AnsatzVars::template Components<stateId>::m>();
      F=0.25*e*y*y*y*y;
      dF=e*y*y*y;
      ddF=3*e*y*y;
      dddF=6*e*y;
    }


    template<int row>
    Scalar d1 (VariationalArg<Scalar,dim, AnsatzVars::template Components<row>::m> const& arg) const
    {
      if( row != stateId ) return 0;
      return kappa*dy[0]*arg.derivative[0]+dF*arg.value;//sp(dy,if_(arg.gradient,dy));
      //return kappa*sp(dy,if_(arg.gradient,dy));
    }

    template<int row, int col>
    Scalar d2 (VariationalArg<Scalar,dim,AnsatzVars::template Components<row>::m> const &arg1, VariationalArg<Scalar,dim,AnsatzVars::template Components<col>::m> const &arg2) const
    {
      if ( row != stateId || col != stateId ) return 0;
      return ( dkappa * arg2.value ) * sp(dy,arg1.derivative) + kappa * sp(arg2.derivative[0],arg1.derivative[0])+ddF*arg1.value*arg2.value;//sp(if_(arg2.gradient,dy),if_(arg1.gradient,dy));
//      return ( dkappa * if_(arg2.value,y) ) * sp(dy,if_(arg1.gradient,dy)) + kappa * arg2.gradient[0]*arg1.gradient[0];//sp(if_(arg2.gradient,dy),if_(arg1.gradient,dy));
    }

    template <int id1, int id2, int id3>
    Scalar d3 (VariationalArg<Scalar,dim,AnsatzVars::template Components<id1>::m> const& arg1,
               VariationalArg<Scalar,dim,AnsatzVars::template Components<id2>::m> const& arg2,
               VariationalArg<Scalar,dim,AnsatzVars::template Components<id3>::m> const& arg3) const
    {
      if( id1 != stateId || id2 != stateId || id3 != stateId ) return 0;
      return ( arg3.value * ( ddkappa * arg2.value ) ) * sp(dy,arg1.derivative) + ( dkappa * arg2.value ) * sp(arg3.derivative, arg1.derivative)
          + ( dkappa * arg3.value ) * sp(arg2.derivative,arg1.derivative)+dddF*arg1.value*arg2.value*arg2.value;
    }

  private:
    Scalar c, d, e, kappa, F, dF, ddF, dddF;
    Dune::FieldVector<Scalar,AnsatzVars::template Components<stateId>::m> dkappa, y;
    Dune::FieldMatrix<Scalar,AnsatzVars::template Components<stateId>::m,dim> dy;
    Dune::FieldMatrix<Scalar,AnsatzVars::template Components<stateId>::m,AnsatzVars::template Components<stateId>::m> ddkappa;
    LinAlg::EuclideanScalarProduct sp;
  };

}

#endif // ANTONSNONLINEARTESTPROBLEM_1_HH
