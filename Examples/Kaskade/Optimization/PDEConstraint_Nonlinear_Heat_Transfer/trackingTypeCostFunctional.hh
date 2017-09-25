#ifndef TRACKINGTYPECOSTFUNCTIONAL_HH
#define TRACKINGTYPECOSTFUNCTIONAL_HH

#include <dune/common/fvector.hh>

namespace Kaskade
{
  template <class ReferenceSolution, int stateId = 0, int controlId = 1, class Direction = void>
  class TrackingTypeCostFunctional
  {
  public:
    typedef typename ReferenceSolution::Descriptions AnsatzVars;
    typedef typename ReferenceSolution::Descriptions::Scalar Scalar;
    static constexpr int dim = AnsatzVars::Grid::dimension;

    static int const stateSpaceId = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables,stateId>::type::spaceIndex;

    template <class Evaluators>
    void evaluateAt(Dune::FieldVector<Scalar, AnsatzVars::template Components<stateId>::m> const& y_,
                    Dune::FieldVector<Scalar, AnsatzVars::template Components<controlId>::m> const& u_,
                    Evaluators const& evaluators)
    {
      y = y_;
      u = u_;
      y_ref = boost::fusion::at_c<stateId>(y_ref_fse.data).value(boost::fusion::at_c<stateSpaceId>(evaluators));
    }

    Scalar d0(int id = -1) const
    {
      auto y_z = (y - y_ref)*d.d0();
      if(id == stateId) return 0.5 * beta * y_z*y_z;
      if(id == controlId) return 0.5 * alpha * u * u;
      return 0.5* beta *y_z * y_z + 0.5 * alpha * u * u;
    }

    template<int row>
    Scalar d1 (VariationalArg<Scalar,dim,AnsatzVars::template Components<row>::m> const& arg) const
    {
      if(row==stateId) return beta*((y - y_ref) * d.d0()) * (if_(arg.value,y) * d.d0());//( (y - y_ref) * d.d0() ) * ( if_(arg.value,y) * d.d0() + y * d.d1(arg) );
      if(row==controlId) return alpha*(u*if_(arg.value,u));
      return 0;
    }

    template<int row, int col>
    Scalar d2 (VariationalArg<Scalar,dim,AnsatzVars::template Components<row>::m> const& arg1,
               VariationalArg<Scalar,dim,AnsatzVars::template Components<col>::m> const& arg2) const
    {
      if(row==stateId && col==stateId) return beta*(if_(arg1.value,y)*d.d0()) * (if_(arg2.value,y)*d.d0());/*( if_(arg1.value,y) * d.d0() + y * d.d1(arg1) ) * ( if_(arg2.value,y) * d.d0() + y * d.d1(arg2) )
          + ( (y - y_ref) * d.d0() ) * ( if_(arg1.value,y) * d.d1(arg2) + if_(arg2.value,y) * d.d1(arg1) );*///if_(arg1.value,y) * if_(arg2.value,y);
      if(row==controlId && col==controlId) return alpha*if_(arg1.value,u)*if_(arg2.value,u);
      return 0;
    }

    TrackingTypeCostFunctional(Scalar alpha_, ReferenceSolution const& fse, Direction const& d_, Scalar beta_=1.0) : alpha(alpha_), beta(beta_), y_ref_fse(fse), y_ref(0), y(0), u(0), d(d_)
    {
      assert(alpha > 0);
    }

    TrackingTypeCostFunctional(TrackingTypeCostFunctional const&) = default;
    TrackingTypeCostFunctional& operator=(TrackingTypeCostFunctional const&) = default;

    void setReferenceSolution(ReferenceSolution const& fse)
    {
      y_ref_fse = fse;
    }

  private:
    Scalar alpha, beta;
    ReferenceSolution y_ref_fse;
    Dune::FieldVector<Scalar,AnsatzVars::template Components<stateId>::m> y_ref, y;
    Dune::FieldVector<Scalar,AnsatzVars::template Components<controlId>::m> u;
    Direction d;
  };


  template <class ReferenceSolution, int stateId, int controlId>
  class TrackingTypeCostFunctional<ReferenceSolution,stateId,controlId,void>
  {
  public:
    typedef typename ReferenceSolution::Descriptions AnsatzVars;
    typedef typename ReferenceSolution::Descriptions::Scalar Scalar;
    static constexpr int dim = AnsatzVars::Grid::dimension;

    static int const stateSpaceId = boost::fusion::result_of::value_at_c<typename AnsatzVars::Variables,stateId>::type::spaceIndex;

    template <class Evaluators>
    void evaluateAt(Dune::FieldVector<Scalar, AnsatzVars::template Components<stateId>::m> const& y_,
                    Dune::FieldVector<Scalar, AnsatzVars::template Components<controlId>::m> const& u_,
                    Evaluators const& evaluators)
    {
      y = y_;
      u = u_;
      y_ref = boost::fusion::at_c<stateId>(y_ref_fse.data).value(boost::fusion::at_c<stateSpaceId>(evaluators));
    }

    Scalar d0(int id = -1) const
    {
      auto y_z = y - y_ref;
      if(id == stateId) return 0.5 * beta * y_z*y_z;
      if(id == controlId) return 0.5 * alpha * u * u;
      return 0.5*beta*y_z * y_z + 0.5 * alpha * u * u;
    }

    template<int row>
    Scalar d1 (VariationalArg<Scalar,dim,AnsatzVars::template Components<row>::m> const& arg) const
    {
      if(row==stateId) return beta*(y - y_ref) * if_(arg.value,y);
      if(row==controlId) return alpha*(u*if_(arg.value,u));
      return 0;
    }

    template<int row, int col>
    Scalar d2 (VariationalArg<Scalar,dim,AnsatzVars::template Components<row>::m> const& arg1,
               VariationalArg<Scalar,dim,AnsatzVars::template Components<col>::m> const& arg2) const
    {
      if(row==stateId && col==stateId) return beta*if_(arg1.value,y) * if_(arg2.value,y);
      if(row==controlId && col==controlId) return alpha*if_(arg1.value,u)*if_(arg2.value,u);
      return 0;
    }

    TrackingTypeCostFunctional(Scalar alpha_, ReferenceSolution const& fse, Scalar beta_=1.0) : alpha(alpha_), beta(beta_), y_ref_fse(fse), y_ref(0), y(0), u(0)
    {
      assert(alpha > 0);
    }

    TrackingTypeCostFunctional(TrackingTypeCostFunctional const&) = default;
    TrackingTypeCostFunctional& operator=(TrackingTypeCostFunctional const&) = default;

    void setReferenceSolution(ReferenceSolution const& fse)
    {
      y_ref_fse = fse;
    }

  private:
    Scalar alpha, beta;
    ReferenceSolution y_ref_fse;
    Dune::FieldVector<Scalar,AnsatzVars::template Components<stateId>::m> y_ref, y;
    Dune::FieldVector<Scalar,AnsatzVars::template Components<controlId>::m> u;
  };

}

#endif // TRACKINGTYPECOSTFUNCTIONAL_HH
