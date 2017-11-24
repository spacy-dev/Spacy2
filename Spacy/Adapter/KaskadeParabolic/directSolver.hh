
// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once


#include "fem/variables.hh"
#include "linalg/direct.hh"

#include "Spacy/linearSolver.hh"
#include "Spacy/operator.hh"
//#include "Spacy/c2Functional.hh"
#include "Spacy/vector.hh"
#include "Spacy/Util/Base/OperatorBase.hh"
#include "util.hh"
#include <Spacy/zeroVectorCreator.hh>

#include <iostream>

#include "linearBlockOperator.hh"

namespace Spacy
{
/// \cond
class VectorSpace;
/// \endcond

namespace KaskadeParabolic
{

    namespace PDE{

        /**
     * @brief A direct solver for time dependend problems
     *
     */
        template<class FunctionalDefinition>
        class DirectSolver : public OperatorBase {
            /// %Kaskade::VariableSetDescription
            using VariableSetDescription = typename FunctionalDefinition::AnsatzVars;
            /// Coefficient vector type.
            using CoefficientVector = typename VariableSetDescription::template CoefficientVectorRepresentation<>::type;
            /// boost::fusion::vector<const Space0*,const Space1*,...>
            using Spaces = typename VariableSetDescription::Spaces;
            /// Matrix type
            using Matrix = ::Kaskade::MatrixAsTriplet<double>;
            using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix, CoefficientVector, CoefficientVector>;

//            using VYSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<VariableSetDescription, 0>;
          using VYSetDescription = VariableSetDescription;
            using CoefficientVectorY = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
            using KaskadeOperatorYY = ::Kaskade::MatrixRepresentedOperator<Matrix, CoefficientVectorY, CoefficientVectorY>;

            using ImplVec = ::Spacy::KaskadeParabolic::Vector<VariableSetDescription>;

            /**
             * @brief Constructor.
             * @param domain domain space
             * @param range range space
             */

        public:
            DirectSolver() = delete;

            DirectSolver( const LinearBlockOperator<VariableSetDescription, VariableSetDescription> &H) :
                    OperatorBase(H.domain(), H.range()), H_(H), spacesVec_(H.getSpacesVec()), dtVec_(H.getDtVec()) {

                for (auto i = 0u; i < dtVec_.size(); i++) {
                    double dt = dtVec_.at(i);

                    if (i != 0) {
                        KaskadeOperatorYY currentKaskOp(H_.getKaskOp("A", i).get());
                        currentKaskOp *= dt;
                        currentKaskOp += H_.getKaskOp("MassY", i).get();
                        solDiag.emplace_back(std::make_shared
                                                     <::Kaskade::InverseLinearOperator <
                                                      ::Kaskade::DirectSolver <
                                                      CoefficientVectorY, CoefficientVectorY> >
                                             > (::Kaskade::directInverseOperator(currentKaskOp,
                                                                                 DirectType::UMFPACK3264,
                                                                                 MatrixProperties::GENERAL)));
                    } else {
                        KaskadeOperatorYY currentKaskOp(H_.getKaskOp("MassY", i));
                        solDiag.emplace_back(std::make_shared
                                                     <::Kaskade::InverseLinearOperator <
                                                      ::Kaskade::DirectSolver <
                                                      CoefficientVectorY, CoefficientVectorY> >
                                             > (::Kaskade::directInverseOperator(currentKaskOp,
                                                                                 DirectType::UMFPACK3264,
                                                                                 MatrixProperties::GENERAL)));
                    }
                }


            }

            /**
             * @brief Apply preconditioner \f$P\f$.
             * @param x argument
             * @return \f$P(x)\f$
             */
            ::Spacy::Vector operator()(const ::Spacy::Vector &b) const {

                using ::Spacy::cast_ref;

                auto b_impl = cast_ref<ImplVec>(b);

                std::vector<CoefficientVectorY> b_y;

                //domain
                std::vector<CoefficientVectorY> x_y;

                //range
                x_y.reserve(dtVec_.size());

                for (auto i = 0u; i < dtVec_.size(); i++) {
                    b_y.emplace_back(CoefficientVectorY(
                            VYSetDescription::template CoefficientVectorRepresentation<>::init(*(spacesVec_.at(i)))));

                    x_y.emplace_back(CoefficientVectorY(
                            VYSetDescription::template CoefficientVectorRepresentation<>::init(*(spacesVec_.at(i)))));
                    x_y.at(i) = 0;
                }

                // copy into CoefficientVectors
                for (auto i = 0u; i < dtVec_.size(); i++) {
                    boost::fusion::at_c<0>(b_y.at(i).data) = b_impl.getCoeffVec(i);
                }


                /// Solve State Equation
                for (auto i = 0u; i < dtVec_.size(); i++) {
                    if (verbose)
                        std::cout << "Solving the state equation for timestep " << i << std::endl;

                    double dt = dtVec_.at(i);

                    solDiag.at(i)->apply(b_y.at(i), x_y.at(i));

                    if (i != dtVec_.size() - 1)
                        H_.getKaskOp("MassY", i).applyscaleadd(1., x_y.at(i), b_y.at(i + 1));
                }

                ::Spacy::Vector result = zero(domain());
                auto& res_impl = cast_ref<ImplVec>(result);
                for (auto i = 0u; i < dtVec_.size(); i++) {
                    res_impl.getCoeffVec_nonconst(i) = boost::fusion::at_c<0>(x_y.at(i).data);
                }

                return result;

            }

        private:
            bool verbose = 0;
            std::vector<std::shared_ptr<Spaces> > spacesVec_;
            std::vector<double> dtVec_;
            LinearBlockOperator<VariableSetDescription, VariableSetDescription> H_;
            mutable std::vector<std::shared_ptr<
                    ::Kaskade::InverseLinearOperator <
                    ::Kaskade::DirectSolver < CoefficientVectorY, CoefficientVectorY> > > >

            solDiag{};
        };

        template <class OperatorDefinition,class VariableSetDescription>
        auto makeDirectSolver(LinearBlockOperator<VariableSetDescription, VariableSetDescription> &H)
        {
            return DirectSolver<OperatorDefinition>(std::move(H));
        }
    }

}
}

