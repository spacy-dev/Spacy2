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


namespace Spacy
{
  /** @addtogroup KaskadeGroup
   * @{
   */

  namespace KaskadeParabolic
  {
    namespace PDE
    {
      /**
     * @brief Linear operator interface for operators in %Kaskade 7.
     * @tparam OperatorImpl %Kaskade 7 operator, i.e. %Kaskade::AssembledGalerkinOperator or %Kaskade::MatrixRepresentedOperator.
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
       * @param My ... vectors of Kaskade Operators for each timestep
       * @param space OperatorSpace of this linear Operator
       */
        LinearBlockOperator(std::vector<KaskadeOperator> MassY,std::vector<KaskadeOperator> A,const std::vector<double> dtVec,
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

        LinearBlockOperator(std::vector<KaskadeOperator> MassY,std::vector<KaskadeOperator> A,const std::vector<double> dtVec,
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
        ::Spacy::Vector operator()(const ::Spacy::Vector &x) const {

          if (verbose)
            std::cout << "In evaluation of Block Op PDE " << std::endl;
          using ::Spacy::cast_ref;

          auto x_impl = cast_ref<ImplVec_domain>(x);


          std::vector<Domain> y_d;

          y_d.reserve(dtVec_.size());

          std::vector<Range> y_r;

          y_r.reserve(dtVec_.size());

          for (auto i = 0u; i < dtVec_.size(); i++) {
            y_d.emplace_back(CoefficientVectorY(
                               VYSetDescription::template CoefficientVectorRepresentation<>::init(*(spacesVec_.at(i)))));
            y_r.emplace_back(CoefficientVectorY(
                               VYSetDescription::template CoefficientVectorRepresentation<>::init(*(spacesVec_.at(i)))));
            y_r.at(i) *= 0;
          }

          // copy into CoefficientVectors
          for (auto i = 0u; i < dtVec_.size(); i++) {
            boost::fusion::at_c<0>(y_d.at(i).data) = x_impl.getCoeffVec(i);

          }
          // Apply the assembled KaskadeOperators
          for (auto i = 0u; i < dtVec_.size(); i++) {
            double dt = dtVec_.at(i);

            // State Equation
            // perform
            // | M(0)                               |   |y_d(0) |
            // | -M(0)   dt*A(1) + M(1)             | * |  :    |
            // |        ...         ...             |   |y_d(N) |
            // |          -M(N-1)    dt*A(N) + M(N) |

            if (verbose)
              std::cout << "State Equation of timestep " << i << std::endl;

            MassY_.at(i).applyscaleadd(1., y_d.at(i), y_r.at(i));
            if (i != 0) {
              MassY_.at(i - 1).applyscaleadd(-1., y_d.at(i - 1), y_r.at(i));
              A_.at(i).applyscaleadd(dt, y_d.at(i), y_r.at(i));
            }

          }

          // cast result into Spacy Vector

          ::Spacy::Vector result = zero(range());
          ImplVec_range& res_impl = cast_ref<ImplVec_range>(result);



          for (auto i = 0u; i < dtVec_.size(); i++) {
            res_impl.getCoeffVec_nonconst(i)= boost::fusion::at_c<0>(y_r.at(i).data);
          }

          return result;
        }

        ::Spacy::Real operator()(const LinearBlockOperator &) const {
          return Real(0);
        }

        /// Access solver representing \f$A^{-1}\f$.
        auto solver() const {
          return solverCreator_(*this);
        }

        /// This needs to be removed
        auto &get() {
          std::cout << "This method returns basically nonsense " << std::endl;
          return A_.at(0).get_non_const();
        }

        const auto &get() const {
          std::cout << "This method returns basically nonsense " << std::endl;

          return A_.at(0).template get<Matrix>();
        }

        const auto &A() const {
          std::cout << "This method returns basically nonsense " << std::endl;

          return A_.at(0);
        }

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

        const std::vector<std::shared_ptr<Spaces> > &getSpacesVec() const {
          return spacesVec_;
        }

        const std::vector<double> getDtVec() const {
          return dtVec_;
        }

      private:
        bool verbose = false;
        std::vector<KaskadeOperator> MassY_{};
        std::vector<KaskadeOperator> A_{};

        std::vector<double> dtVec_{};
        std::vector<std::shared_ptr<Spaces> > spacesVec_{};
        std::function<LinearSolver( const LinearBlockOperator&)> solverCreator_ = {};
      };
    }
  }
  /** @} */
}
