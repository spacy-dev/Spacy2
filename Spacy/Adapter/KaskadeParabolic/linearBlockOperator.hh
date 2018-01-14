#pragma once

#include <utility>

#include <Spacy/linearSolver.hh>
#include <Spacy/vector.hh>
#include <Spacy/Util/Base/OperatorBase.hh>
#include <Spacy/Util/Base/VectorBase.hh>

#include "linearBlockOperatorSpace.hh"
#include "directSolver.hh"
#include "util.hh" // copy
#include <Spacy/Util/cast.hh>
#include "vector.hh"
#include <Spacy/Util/Base/AddArithmeticOperators.hh>
#include <Spacy/Spaces/productSpace.hh>
#include <Spacy/Util/Mixins/Get.hh>

namespace Spacy
{
  /** @addtogroup KaskadeParabolicGroup
   * @{
   */

  namespace KaskadeParabolic
  {
    namespace PDE
    {
      /**
     * @brief Linear block operator for operators of parabolic PDEs in %Kaskade 7.
     * @tparam AnsatzVariableSetDescription %Kaskade::VariableSetDescription for ansatz variables
     * @tparam TestVariableSetDescription %Kaskade::VariableSetDescription for test variables
     * @see ::Spacy::LinearOperator
     */
      template<class AnsatzVariableSetDescription, class TestVariableSetDescription>
      class LinearBlockOperator :
          public OperatorBase,
          public VectorBase,
          public AddArithmeticOperators<
          LinearBlockOperator < AnsatzVariableSetDescription, TestVariableSetDescription>>
      {
        using AVD = AnsatzVariableSetDescription;
        using TVD = TestVariableSetDescription;
        using Spaces = typename AnsatzVariableSetDescription::Spaces;
        using Variables = typename AnsatzVariableSetDescription::Variables;

        using Domain = typename AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
        using Range = typename AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
        using Matrix = ::Kaskade::MatrixAsTriplet<double>;
        using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix, Domain, Range>;

        using BlockOperatorCreator = LinearBlockOperatorCreator<AnsatzVariableSetDescription, TestVariableSetDescription>;

        using VYSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<AVD ,0>;
        using CoefficientVectorY = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
        using KaskadeOperatorYY = ::Kaskade::MatrixRepresentedOperator<Matrix,CoefficientVectorY,CoefficientVectorY>;

        using ImplVec_domain = ::Spacy::KaskadeParabolic::Vector<AVD>;
        using ImplVec_range = ::Spacy::KaskadeParabolic::Vector<TVD>;


      public:

        /**
       * @brief Construct linear operator for %Kaskade 7.
       * @param MassY Mass matrices for each timestep
       * @param A stiffness matrices for each timestep
       * @param dtVec timesteps
       * @param spacesVec Kaskade Space for each timestep
       * @param space OperatorSpace of this linear Operator
       */
        LinearBlockOperator(std::vector<KaskadeOperator> MassY,std::vector<KaskadeOperator> A,const std::vector<Real> dtVec,
                            const std::vector<std::shared_ptr<Spaces> > &spacesVec,const VectorSpace &space)
          :
            dtVec_ (dtVec), spacesVec_(spacesVec),
            OperatorBase(cast_ref<BlockOperatorCreator>(space.creator()).domain(),
                         cast_ref<BlockOperatorCreator>(space.creator()).range()),
            VectorBase(space)
        {
          for (auto i = 0u; i < dtVec.size(); i++) {
            MassY_.emplace_back(std::move(MassY.at(i)));
            A_.emplace_back(std::move(A.at(i)));
          }

        }

        /**
       * @brief Construct linear operator for %Kaskade 7.
       * @param MassY Mass matrices for each timestep
       * @param A stiffness matrices for each timestep
       * @param dtVec timesteps
       * @param spacesVec Kaskade Space for each timestep
       * @param space OperatorSpace of this linear Operator
       * @param solverCreator Function for generation of a solver for this operator
       */
        LinearBlockOperator(std::vector<KaskadeOperator> MassY,std::vector<KaskadeOperator> A,const std::vector<Real> dtVec,
                            const std::vector<std::shared_ptr<Spaces> > &spacesVec,const VectorSpace &space,
                            std::function<LinearSolver(const LinearBlockOperator&)> solverCreator)
          :
            dtVec_ (dtVec), spacesVec_(spacesVec),
            OperatorBase(cast_ref<BlockOperatorCreator>(space.creator()).domain(),
                         cast_ref<BlockOperatorCreator>(space.creator()).range()),
            VectorBase(space),solverCreator_(std::move(solverCreator))
        {
          for (auto i = 0u; i < dtVec.size(); i++) {
            MassY_.emplace_back(std::move(MassY.at(i)));
            A_.emplace_back(std::move(A.at(i)));
          }

        }

        /// Compute \f$A(x)\f$.
        ::Spacy::Vector operator()(const ::Spacy::Vector &x) const
        {

          if (verbose)
            std::cout << "In evaluation of Block Op PDE " << std::endl;
          using ::Spacy::cast_ref;

          auto x_impl = cast_ref<ImplVec_domain>(x);


          /// Coeff Vec for argument
          std::vector<Domain> y_d;
          y_d.reserve(dtVec_.size());

          /// Coeff Vec for output
          std::vector<Range> y_r;
          y_r.reserve(dtVec_.size());

          for (auto i = 0u; i < dtVec_.size(); i++)
          {
            y_d.emplace_back(CoefficientVectorY(
                               VYSetDescription::template CoefficientVectorRepresentation<>::init(*(spacesVec_.at(i)))));
            y_r.emplace_back(CoefficientVectorY(
                               VYSetDescription::template CoefficientVectorRepresentation<>::init(*(spacesVec_.at(i)))));
            y_r.at(i) *= 0;
          }

          // copy into CoefficientVectors
          for (auto i = 0u; i < dtVec_.size(); i++)
          {
            boost::fusion::at_c<0>(y_d.at(i).data) = x_impl.getCoeffVec(i);

          }
          // Apply the assembled KaskadeOperators
          for (auto i = 0u; i < dtVec_.size(); i++)
          {
            double dt = ::Spacy::Mixin::get(dtVec_.at(i));

            // State Equation
            // perform
            // | M(0)                               |   |y_d(0) |
            // | -M(0)   dt*A(1) + M(1)             | * |  :    |
            // |        ...         ...             |   |y_d(N) |
            // |          -M(N-1)    dt*A(N) + M(N) |

            if (verbose)
              std::cout << "State Equation of timestep " << i << std::endl;

            MassY_.at(i).applyscaleadd(1., y_d.at(i), y_r.at(i));
            if (i != 0)
            {
              MassY_.at(i - 1).applyscaleadd(-1., y_d.at(i - 1), y_r.at(i));
              A_.at(i).applyscaleadd(dt, y_d.at(i), y_r.at(i));
            }
          }

          // cast result into Spacy Vector
          ::Spacy::Vector result = zero(range());
          ImplVec_range& res_impl = cast_ref<ImplVec_range>(result);

          for (auto i = 0u; i < dtVec_.size(); i++)
          {
            res_impl.getCoeffVec_nonconst(i)= boost::fusion::at_c<0>(y_r.at(i).data);
          }

          return result;
        }

        ::Spacy::Real operator()(const LinearBlockOperator &) const
        {
          return Real(0);
        }

        /// Access solver representing \f$A^{-1}\f$.
        auto solver() const
        {
          return solverCreator_(*this);
        }

        /// needed for interface, returns nonsense
        auto &get()
        {
          std::cout << "This method returns basically nonsense " << std::endl;
          return A_.at(0).get_non_const();
        }

        /// needed for interface, returns nonsense
        const auto &get() const
        {
          std::cout << "This method returns basically nonsense " << std::endl;
          return A_.at(0).template get<Matrix>();
        }

        /// needed for interface, returns nonsense
        const auto &A() const {
          std::cout << "This method returns basically nonsense " << std::endl;

          return A_.at(0);
        }

        /**
               * @brief getter for Kaskade Operator of a timestep
               * @param type Operator requested (mass or stiffness)
               * @param timeStep at which timestep
               * @return Kaskade Operator
               */
        const auto getKaskOp(std::string type, int timeStep) const {
          if (!type.compare("MassY")) {
            if (verbose)
              std::cout << " Returning Y Block " << std::endl;
            return MassY_.at(timeStep);
          }
          if (!type.compare("A")) {
            if (verbose)
              std::cout << " Returning A Block " << std::endl;
            return A_.at(timeStep);
          }
          else std::cout << " NO VALID TYPE GIVEN " << std::endl;
          return A_.at(0);
        }

        /**
               * @brief getter for Kaskade Spaces
               * @return Kaskade Spaces for each timestep
               */
        const std::vector<std::shared_ptr<Spaces> > &getSpacesVec() const {
          return spacesVec_;
        }

        /**
               * @brief getter for temporal timesteps
               * @return timestep sizes
               */
        const std::vector<Real> getDtVec() const {
          return dtVec_;
        }

      private:
        bool verbose = false;
        std::vector<KaskadeOperator> MassY_{};
        std::vector<KaskadeOperator> A_{};

        std::vector<Real> dtVec_{};
        std::vector<std::shared_ptr<Spaces> > spacesVec_{};
        std::function<LinearSolver( const LinearBlockOperator&)> solverCreator_ = {};
      };
    }
    namespace OCP
    {
      /**
     * @brief Linear operator interface for operators associated with time dependent optimal control problems in %Kaskade 7.
     * @tparam AnsatzVariableSetDescription %Kaskade::VariableSetDescription for ansatz variables
     * @tparam TestVariableSetDescription %Kaskade::VariableSetDescription for test variables
     * @see ::Spacy::LinearOperator
     */
      template <class AnsatzVariableSetDescription, class TestVariableSetDescription>
      class LinearBlockOperator :
          public OperatorBase ,
          public VectorBase ,
          public AddArithmeticOperators< LinearBlockOperator<AnsatzVariableSetDescription,TestVariableSetDescription> >
      {
        using Spaces = typename AnsatzVariableSetDescription::Spaces;
        using Variables = typename AnsatzVariableSetDescription::Variables;
        using Matrix = ::Kaskade::MatrixAsTriplet<double>;

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

        using PSV = ::Spacy::ProductSpace::Vector;
        using VectorImplY = ::Spacy::KaskadeParabolic::Vector<VYSetDescription>;
        using VectorImplU = ::Spacy::KaskadeParabolic::Vector<VUSetDescription>;
        using VectorImplP = ::Spacy::KaskadeParabolic::Vector<VPSetDescription>;

        using BlockOperatorCreator = LinearBlockOperatorCreator<AnsatzVariableSetDescription,TestVariableSetDescription>;

      public:

        /**
       * @brief Construct linear operator for %Kaskade 7.
       * @param My Lyy part of Lagrange functional for each timestep
       * @param Mu Luu part of Lagrange functional for each timestep
       * @param A stiffness matrix of constraint for each timestep
       * @param A_t transposed stiffness matrix of constraint for each timestep (to be replaced with applytransposed..)
       * @param B control matrix of constraint for each timestep
       * @param B_t transposed control matrix of constraint for each timestep (to be replaced with applytransposed..)
       * @param dtVec timesteps
       * @param spacesVec Kaskade Space for each timestep
       * @param space OperatorSpace of this linear Operator
       */
        LinearBlockOperator(std::vector<Mytype> My,std::vector<Mutype> Mu,std::vector<Atype> A,
                            std::vector<Btype> B,std::vector<ATtype> A_t,std::vector<BTtype> B_t,
                            std::vector<Masstype> Mass_yy, std::vector<Masstype> Mass_sd,
                            const std::vector<Real> dtVec, const std::vector<std::shared_ptr<Spaces> >& spacesVec , const VectorSpace& space)
          : dtVec_(dtVec), spacesVec_(spacesVec),
            OperatorBase(cast_ref<BlockOperatorCreator>(space.creator()).domain(),
                         cast_ref<BlockOperatorCreator>(space.creator()).range()),
            VectorBase(space)
        {
          if(verbose)
            for(auto i = 0u; i< dtVec.size();i++){
              My_.emplace_back(std::move(My.at(i)));
              Mu_.emplace_back(std::move(Mu.at(i)));
              A_.emplace_back(std::move(A.at(i)));
              B_.emplace_back(std::move(B.at(i)));
              A_t_.emplace_back(std::move(A_t.at(i)));
              B_t_.emplace_back(std::move(B_t.at(i)));
              Mass_yy_.emplace_back(std::move(Mass_yy.at(i)));
              if(i != dtVec.size()-1)
                Mass_sd_.emplace_back(std::move(Mass_sd.at(i)));
            }

        }

        /**
       * @brief Construct linear operator for %Kaskade 7.
       * @param My Lyy part of Lagrange functional for each timestep
       * @param Mu Luu part of Lagrange functional for each timestep
       * @param A stiffness matrix of constraint for each timestep
       * @param A_t transposed stiffness matrix of constraint for each timestep (to be replaced with applytransposed..)
       * @param B control matrix of constraint for each timestep
       * @param B_t transposed control matrix of constraint for each timestep (to be replaced with applytransposed..)
       * @param dtVec timesteps
       * @param spacesVec Kaskade Space for each timestep
       * @param space OperatorSpace of this linear Operator
       * @param solverCreator function/functor implementing the creation of a linear solver
       */
        LinearBlockOperator(std::vector<Mytype> My,std::vector<Mutype> Mu,std::vector<Atype> A,
                            std::vector<Btype> B,std::vector<ATtype> A_t,std::vector<BTtype> B_t,
                            std::vector<Masstype> Mass_yy, std::vector<Masstype> Mass_sd,
                            const std::vector<Real> dtVec, const std::vector<std::shared_ptr<Spaces>>& spacesVec , const VectorSpace& space,
                            std::function<LinearSolver(const LinearBlockOperator&)> solverCreator)
          : dtVec_(dtVec), spacesVec_(spacesVec),
            OperatorBase(cast_ref<BlockOperatorCreator>(space.creator()).domain(),
                         cast_ref<BlockOperatorCreator>(space.creator()).range()),
            VectorBase(space),solverCreator_(std::move(solverCreator))
        {
          for(auto i = 0u; i< dtVec.size();i++){
            My_.emplace_back(std::move(My.at(i)));
            Mu_.emplace_back(std::move(Mu.at(i)));
            A_.emplace_back(std::move(A.at(i)));
            B_.emplace_back(std::move(B.at(i)));
            A_t_.emplace_back(std::move(A_t.at(i)));
            B_t_.emplace_back(std::move(B_t.at(i)));
            Mass_yy_.emplace_back(std::move(Mass_yy.at(i)));
            if(i != dtVec.size()-1)
              Mass_sd_.emplace_back(std::move(Mass_sd.at(i)));
          }

        }

        /// Compute \f$A(x)\f$.
        ::Spacy::Vector operator()(const ::Spacy::Vector& x) const
        {

          if(verbose)
            std::cout<< "In evaluation of Block Op "<<std::endl;

          /// DOMAIN VECTOR
          auto x_ps = ::Spacy::cast_ref<PSV>(x);
          //subvectors as Spacy::Vector
          auto x_y = (::Spacy::cast_ref<PSV>(x_ps.component(PRIMAL))).component(0);
          auto x_u = (::Spacy::cast_ref<PSV>(x_ps.component(PRIMAL))).component(1);
          auto x_p = (::Spacy::cast_ref<PSV>(x_ps.component(DUAL))).component(0);
          //Implementation on as Spacy::KaskadeParabolic::Vector
          auto x_y_impl = ::Spacy::cast_ref<VectorImplY>(x_y);
          auto x_u_impl = ::Spacy::cast_ref<VectorImplU>(x_u);
          auto x_p_impl = ::Spacy::cast_ref<VectorImplP>(x_p);

          std::vector< CoefficientVectorY > y_d;
          std::vector< CoefficientVectorU > u_d;
          std::vector< CoefficientVectorP > p_d;

          y_d.reserve(dtVec_.size());
          u_d.reserve(dtVec_.size());
          p_d.reserve(dtVec_.size());

          std::vector< CoefficientVectorY > y_r;
          std::vector< CoefficientVectorU > u_r;
          std::vector< CoefficientVectorP > p_r;

          y_r.reserve(dtVec_.size());
          u_r.reserve(dtVec_.size());
          p_r.reserve(dtVec_.size());

          for(auto i = 0u;i<dtVec_.size();i++)
          {
            y_d.emplace_back(CoefficientVectorY(VYSetDescription::template CoefficientVectorRepresentation<>::init(*spacesVec_.at(i))));
            u_d.emplace_back(CoefficientVectorU(VUSetDescription::template CoefficientVectorRepresentation<>::init(*spacesVec_.at(i))));
            p_d.emplace_back(CoefficientVectorP(VPSetDescription::template CoefficientVectorRepresentation<>::init(*spacesVec_.at(i))));

            y_r.emplace_back(CoefficientVectorY(VYSetDescription::template CoefficientVectorRepresentation<>::init(*spacesVec_.at(i))));
            y_r.at(i) *= 0;
            u_r.emplace_back(CoefficientVectorU(VUSetDescription::template CoefficientVectorRepresentation<>::init(*spacesVec_.at(i))));
            u_r.at(i) *= 0;
            p_r.emplace_back(CoefficientVectorP(VPSetDescription::template CoefficientVectorRepresentation<>::init(*spacesVec_.at(i))));
            p_r.at(i) *= 0;
          }

          // copy into CoefficientVectors
          for(auto i = 0u;i < dtVec_.size(); i++) {
            //            copyToCoefficientVector<VYSetDescription>(x_primal_ps.component(i), y_d.at(i));
            //            copyToCoefficientVector<VUSetDescription>(x_primal_ps.component(i + dtVec_.size()), u_d.at(i));
            //            copyToCoefficientVector<VPSetDescription>(x_dual_ps.component(i), p_d.at(i));
            boost::fusion::at_c<0>(y_d.at(i).data) = x_y_impl.getCoeffVec(i);
            boost::fusion::at_c<0>(u_d.at(i).data) = x_u_impl.getCoeffVec(i);
            boost::fusion::at_c<0>(p_d.at(i).data) = x_p_impl.getCoeffVec(i);
            //
            //            std::cout << " In lBO yd before " << y_d.at(i).two_norm() << std::endl;
            //            std::cout << " In lBO ud before " << u_d.at(i).two_norm() << std::endl;
            //            std::cout << " In lBO pd before " << p_d.at(i).two_norm() << std::endl;
            //
            //            std::cout << " In lBO yr before " << y_r.at(i).two_norm() << std::endl;
            //            std::cout << " In lBO ur before " << u_r.at(i).two_norm() << std::endl;
            //            std::cout << " In lBO pr before " << p_r.at(i).two_norm() << std::endl;
          }
          // Apply the assembled KaskadeOperators
          for(auto i = 0u;i<dtVec_.size();i++)
          {
            double dt = ::Spacy::Mixin::get(dtVec_.at(i));

            // Adjoint Equation
            // perform
            // | 0                                 M(0)          -M_sd(0)^T                                        |   |y_d(0) |
            // |      dt*My(1)                                (dt*A_t(1) + M(1))  -M_sd(1)^T                       | * |  :    |
            // |            ...                                                ...        ...                      |   |y_d(N) |
            // |                 dt*My(N)                                     0      (dt*A_t(N) + M(N))            |   |p_d(0) |
            //                                                                                                         |  :    |
            //                                                                                                         |p_d(N) |

            if(verbose)
              std::cout<< "Adjoint Equation of timestep "<<i<<std::endl;
            // y_r = dt*My*y_d + y_r (last term should be zero)
            if(i!=0)
              My_.at(i).applyscaleadd(dt,y_d.at(i), y_r.at(i));

            //    writeMatlab(My_.at(i).get(),"My"+std::to_string(i));

            if(i!=0) {
              A_t_.at(i).applyscaleadd(dt, p_d.at(i), y_r.at(i));
              //    writeMatlab(A_t_.at(i).get(),"A_t_"+std::to_string(i));mass or stiffness)

              //         A_.at(i).applyscaleaddTransposed(dt, p_d.at(i), y_r.at(i));
            }

            //Mass_yy_.at(i).applyscaleaddTransposed(1., p_d.at(i),y_r.at(i));
            Mass_yy_.at(i).applyscaleadd(1., p_d.at(i),y_r.at(i));
            //writeMatlab(Mass_yy_.at(i).get(),"Mass_yy"+std::to_string(i));

            //
            if(i!=dtVec_.size()-1) {
              //Mass_sd_.at(i).applyscaleaddTransposed(-1., p_d.at(i + 1), y_r.at(i));
              Mass_sd_.at(i).applyscaleadd(-1., p_d.at(i + 1), y_r.at(i));
              //writeMatlab(Mass_sd_.at(i).get(), "Mass_sd" + std::to_string(i));
            }
            //

            // Stationarity Condition
            // perform
            // | 0                                 0                                               |   |u_d(0) |
            // |      dt*Mu(1)                              (dt*B_t(1))^T                          | * |  :    |
            // |            ...                                              ...                   |   |u_d(N) |mass or stiffness)
            // |                 dt*Mu(N)                                             dt*B_t(N)^T  |   |p_d(0) |
            //                                                                                     |   |  :    |
            //                                                                                         |p_d(N) |
            if(verbose)
              std::cout<< "Stationarity condition of timestep "<<i<<std::endl;

            if(i!=0)
            {
              Mu_.at(i).applyscaleadd(dt, u_d.at(i), u_r.at(i));
              // writeMatlab(Mu_.at(i).get(),"Mu"+std::to_string(i));

              B_t_.at(i).applyscaleadd(dt, p_d.at(i), u_r.at(i));
              //    writeMatlab(B_t_.at(i).get(),"B_t"+std::to_string(i));

              //       B_.at(i).applyscaleaddTransposed(dt, p_d.at(i), u_r.at(i));
            }

            // State Equationmass or stiffness)
            // perform
            // | M(0)                                            0                        |   |y_d(0) |
            // | -M_sd(0)   dt*A(1) + M(1)                           (dt*B(1))            | * |  :    |
            // |        ...         ...                                    ...            |   |y_d(N) |
            // |          -M_sd(N-1)    dt*A(N) + M(N)                         dt*B(N)    |   |u_d(0) |
            //                                                                                |  :    |
            //                                                                                |u_d(N) |
            if(verbose)
              std::cout<< "State Equation of timestep "<<i<<std::endl;

            Mass_yy_.at(i).applyscaleadd(1. ,y_d.at(i),p_r.at(i));
            if(i!=0){
              Mass_sd_.at(i-1).applyscaleadd(-1., y_d.at(i-1),p_r.at(i));
              A_.at(i).applyscaleadd(dt,y_d.at(i),p_r.at(i));
              //  writeMatlab(A_.at(i).get(),"A"+std::to_string(i));

              B_.at(i).applyscaleadd(dt,u_d.at(i),p_r.at(i));
              //  writeMatlab(B_.at(i).get(),"B"+std::to_string(i));mass or stiffness)

            }

          }
          //          auto z = y_d.at(0);
          //          Mass_yy_.at(0).apply(y_d.at(0),z);
          //          std::cout<<"NORM IN LBO "<<y_d.at(0)(z)<<std::endl;

          // cast result into Spacy Vector

          ::Spacy::Vector result = zero(range());
          PSV& res_ps = cast_ref<PSV>(result);

          auto& res_y = (::Spacy::cast_ref<PSV>(res_ps.component(PRIMAL))).component(0);
          auto& res_u = (::Spacy::cast_ref<PSV>(res_ps.component(PRIMAL))).component(1);
          auto& res_p = (::Spacy::cast_ref<PSV>(res_ps.component(DUAL))).component(0);
          //Implementation on as Spacy::KaskadeParabolic::Vector
          auto& res_y_impl = ::Spacy::cast_ref<VectorImplY>(res_y);
          auto& res_u_impl = ::Spacy::cast_ref<VectorImplU>(res_u);
          auto& res_p_impl = ::Spacy::cast_ref<VectorImplP>(res_p);


          for(auto i = 0u;i<dtVec_.size();i++) {
            //          std::cout<<"Result norm in LBO before "<<boost::fusion::at_c<0>(res_y_impl.get(i).data).coefficients().two_norm()<<std::endl;

            res_y_impl.getCoeffVec_nonconst(i) = boost::fusion::at_c<0>(y_r.at(i).data);
            res_u_impl.getCoeffVec_nonconst(i) = boost::fusion::at_c<0>(u_r.at(i).data);
            res_p_impl.getCoeffVec_nonconst(i) = boost::fusion::at_c<0>(p_r.at(i).data);
            //            std::cout<< "in lbo output "<< y_r.at(i).two_norm()<<std::endl;
            //            std::cout<< "in lbo output "<< u_r.at(i).two_norm()<<std::endl;
            //            std::cout<< "in lbo output "<< p_r.at(i).two_norm()<<std::endl;
            //            std::cout<<"Result norm in LBO after"<<boost::fusion::at_c<0>(res_y_impl.get(i).data).coefficients().two_norm()<<std::endl;

          }

          return result;
        }

        ::Spacy::Real operator()(const LinearBlockOperator&) const
        {
          return Real(0);
        }

        /// Access solver representing \f$A^{-1}\f$.
        auto solver() const
        {
          return solverCreator_(*this);
        }

        /// Needed for interface
        auto& get()
        {
          std::cout<< "This method returns basically nonsense "<<std::endl;
          return A_.at(0).get_non_const();
        }

        const auto& get() const
        {
          std::cout<< "This method returns basically nonsense "<<std::endl;

          return A_.at(0).template get<Matrix>();
        }

        const auto& A() const
        {
          std::cout<< "This method returns basically nonsense "<<std::endl;

          return A_.at(0);
        }

        /**
               * @brief getter for Kaskade Operator of a timestep
               * @param type Operator requested
               * @param timeStep at which timestep
               * @return Kaskade Operator
               */
        const auto getKaskOp(std::string type, int timeStep) const
        {
          if(!type.compare("My")){
            if(verbose)
              std::cout<<" Returning Y Block "<<std::endl;
            return My_.at(timeStep);
          }
          if(!type.compare("Mu")){
            if(verbose)
              std::cout<<" Returning U Block "<<std::endl;
            return Mu_.at(timeStep);
          }
          if(!type.compare("A")){
            if(verbose)
              std::cout<<" Returning A Block "<<std::endl;
            return A_.at(timeStep);
          }
          if(!type.compare("B")){
            if(verbose)
              std::cout<<" Returning B Block "<<std::endl;
            return B_.at(timeStep);
          }
          if(!type.compare("AT")){
            if(verbose)
              std::cout<<" Returning AT Block "<<std::endl;
            return A_t_.at(timeStep);
          }
          if(!type.compare("BT")){
            if(verbose)
              std::cout<<" Returning BT Block "<<std::endl;
            return B_t_.at(timeStep);
          }

          if(!type.compare("Mass_yy")){
            if(verbose)
              std::cout<<" Returning Mass_yy Block "<<std::endl;
            return Mass_yy_.at(timeStep);
          }
          if(!type.compare("Mass_sd")){
            if(verbose)
              std::cout<<" Returning Mass_sd Block "<<std::endl;
            return Mass_sd_.at(timeStep);
          }
          else std::cout<<" NO VALID TYPE GIVEN "<<std::endl;
          return A_.at(0);
        }

        /**
               * @brief getter for Kaskade Spaces
               * @return Kaskade Spaces for each timestep
               */
        const std::vector<std::shared_ptr<Spaces>>& getSpacesVec() const
        {
          return spacesVec_;
        }

        /**
               * @brief getter for temporal timesteps
               * @return timestep sizes
               */
        const std::vector<Real> getDtVec() const
        {
          return dtVec_;
        }

      private:
        bool verbose = false;
        std::vector<Mytype> My_{};
        std::vector<Mutype> Mu_{};
        std::vector<Atype> A_{};
        std::vector<Btype> B_{};
        std::vector<ATtype> A_t_{};
        std::vector<BTtype> B_t_{};

        std::vector<Masstype> Mass_yy_{};
        std::vector<Masstype> Mass_sd_{};

        std::vector<Real> dtVec_{};
        std::vector<std::shared_ptr<Spaces> > spacesVec_{};
        std::function<LinearSolver(const LinearBlockOperator&)> solverCreator_ = {};
      };
    }
  }
  /** @} */
}
