#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/productSpace.hh>
#include <Spacy/Adapter/KaskadeParabolic/gridManager.hh>
#include <Spacy/Adapter/KaskadeParabolic/l2Product.hh>

#include <fem/variables.hh>

namespace Spacy
{
    /** @addtogroup KaskadeParabolicGroup
   * @{
   */

    namespace KaskadeParabolic
    {
        /// \cond
        template <class> class Vector;
        template <class> class VectorCreator;

        namespace Detail
        {
            template <class Description, int i>
            struct ExtractDescription
            {
                using Variables = typename Description::Variables;
                using Spaces = typename Description::Spaces;
                using Variable = std::decay_t<typename boost::fusion::result_of::value_at_c<Variables,i>::type>;
                using type = ::Kaskade::VariableSetDescription< Spaces , boost::fusion::vector< Variable > >;
            };

            template <class Description, int i>
            using ExtractDescription_t = typename ExtractDescription<Description,i>::type;
        }
        /// \endcond

        template <int n>
        struct CoefficientsToVariableSet
        {
            template <class CoefficientVector, class VariableSet>
            static void apply(const CoefficientVector& x, VariableSet& y)
            {
                boost::fusion::at_c<n>(y.data).coefficients() = boost::fusion::at_c<n>(x.data);
            }
        };

        template <>
        struct CoefficientsToVariableSet<-1>
        {
            template <class CoefficientVector, class VariableSet>
            static void apply(const CoefficientVector&, VariableSet&){}
        };

        template <int n>
        struct VariableSetToCoefficients
        {
            template <class VariableSet, class CoefficientVector>
            static void apply(const VariableSet& x, CoefficientVector& y)
            {
                boost::fusion::at_c<n>(y.data) = boost::fusion::at_c<n>(x.data).coefficients();
            }
        };

        template <>
        struct VariableSetToCoefficients<-1>
        {
            template <class VariableSet, class CoefficientVector>
            static void apply(const VariableSet& x, CoefficientVector& y){}
        };

        template <class CoefficientVector, class VariableSet>
        void coefficientsToVariableSet(const CoefficientVector& x, VariableSet& y)
        {
            CoefficientsToVariableSet<VariableSet::Descriptions::noOfVariables-1>::apply(x,y);
        }

        template <class VariableSet, class CoefficientVector>
        void variableSetToCoefficients(const VariableSet& x, CoefficientVector& y)
        {
            VariableSetToCoefficients<VariableSet::Descriptions::noOfVariables-1>::apply(x,y);
        }

        /**
               * @brief get Implementation triple of product space vector
               * @tparam Descriptions Kaskade VariableSetDescription
               * @param x Spacy Vector of interest
               * @return reference of implementation (Spacy::KaskadeParabolic::Vector<Descriptions> of state, control and adjoint
               */
        template<class Descriptions>
        auto getImpl(::Spacy::Vector& x)
        {
          using VYSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,0>;
          using VUSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,1>;
          using VPSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,2>;
          using PSV = ::Spacy::ProductSpace::Vector;

          auto& x_ps = ::Spacy::cast_ref<PSV>(x);

          //subvectors as Spacy::Vector
          auto& x_y = (::Spacy::cast_ref<PSV>(x_ps.component(0))).component(0);
          auto& x_u = (::Spacy::cast_ref<PSV>(x_ps.component(0))).component(1);
          auto& x_p = (::Spacy::cast_ref<PSV>(x_ps.component(1))).component(0);


          //Implementation on as Spacy::KaskadeParabolic::Vector
          assert(Spacy::is<::Spacy::KaskadeParabolic::Vector<VYSetDescription> >(x_y));
          auto& x_y_impl = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::Vector<VYSetDescription>>(x_y);
          assert(::Spacy::is<::Spacy::KaskadeParabolic::Vector<VUSetDescription>>(x_u));
          auto& x_u_impl = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::Vector<VUSetDescription>>(x_u);
          assert(::Spacy::is<::Spacy::KaskadeParabolic::Vector<VPSetDescription>>(x_p));
          auto& x_p_impl = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::Vector<VPSetDescription>>(x_p);

          return std::tie(x_y_impl,x_u_impl,x_p_impl);
        }

    }
    /** @} */
}
