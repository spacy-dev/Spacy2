#include "c2Functional.hh"
#include "vector.hh"
#include "vectorSpace.hh"
#include "Spacy/Spaces/productSpace.hh"


namespace Spacy {
  namespace KaskadeParabolic {
    namespace OCP{
      class ErrorEstimator
      {
      public:

        ErrorEstimator(::Spacy::Real tolerance):tolerance_(tolerance) {}


        template<class FunctionalDefinition>
        void estimateErr(const ::Spacy::Vector& x,const ::Spacy::C2Functional& c2func)
        {
          if(Spacy::is<::Spacy::KaskadeParabolic::C2Functional<FunctionalDefinition> >(c2func))
          {
            auto c2func_impl = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::C2Functional<FunctionalDefinition> >(c2func);

            std::cout<<"computing error indicators"<<std::endl;
            c2func_impl.computeErrorDistribution(x);

            std::cout<<"requesting error indicators from c2functional"<<std::endl;
            std::vector<::Spacy::Real> rho_u_vec = c2func_impl.getStateEqError();
            std::vector<::Spacy::Real> rho_z_vec = c2func_impl.getAdjEqError();
            std::vector<::Spacy::Real> rho_q_vec = c2func_impl.getStatCondError();

            auto no_time_steps = c2func_impl.getGridMan().getTempGrid().getDtVec().size();
            errorIndicators = std::vector<::Spacy::Real>(no_time_steps,::Spacy::Real{0.});
            for(auto i = 0u;i<no_time_steps;i++)
            {
              errorIndicators.at(i) = rho_u_vec.at(i) /*+ rho_z_vec.at(i) + rho_q_vec.at(i)*/;
              std::cout<<rho_u_vec.at(i)<<"            "<<rho_z_vec.at(i)<<"            "<<rho_q_vec.at(i)<<std::endl;
            }
          }
          else
            std::cout<<"No valid c2 func given, needs implementation"<<std::endl;
          return;
        }

        template<class FunctionalDefinition>
        void refine(::Spacy::VectorSpace& domain, ::Spacy::C2Functional& c2func)
        {

          if(Spacy::is<::Spacy::KaskadeParabolic::C2Functional<FunctionalDefinition> >(c2func))
          {
            using VYSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<typename FunctionalDefinition::AnsatzVars,0>;
            using VUSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<typename FunctionalDefinition::AnsatzVars,1>;
            using VPSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<typename FunctionalDefinition::AnsatzVars,2>;

            std::cout<<"its a parab c2 func-> casting"<<std::endl;
            auto& c2func_impl = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::C2Functional<FunctionalDefinition> >(c2func);
            unsigned refctr = 0;
            auto& domain_ps = ::Spacy::cast_ref<::Spacy::ProductSpace::VectorCreator> (domain.creator());

            auto& Y = (::Spacy::cast_ref<::Spacy::ProductSpace::VectorCreator>(domain_ps.subSpace(0).creator())).subSpace(0);
            auto& U = (::Spacy::cast_ref<::Spacy::ProductSpace::VectorCreator>(domain_ps.subSpace(0).creator())).subSpace(1);
            auto& P = (::Spacy::cast_ref<::Spacy::ProductSpace::VectorCreator>(domain_ps.subSpace(1).creator())).subSpace(0);
            if(::Spacy::is<::Spacy::KaskadeParabolic::VectorCreator<VYSetDescription> >(Y.creator()));
            std::cout<<"Y Creator is right"<<std::endl;
            if(::Spacy::is<::Spacy::KaskadeParabolic::VectorCreator<VUSetDescription> >(U.creator()));
            std::cout<<"U Creator is right"<<std::endl;
            if(::Spacy::is<::Spacy::KaskadeParabolic::VectorCreator<VPSetDescription> >(P.creator()));
            std::cout<<"P Creator is right"<<std::endl;
            //            assert(::Spacy::is<::Spacy::KaskadeParabolic::VectorCreator<typename FunctionalDefinition::AnsatzVars> >(U.creator()));
            //            assert(::Spacy::is<::Spacy::KaskadeParabolic::VectorCreator<typename FunctionalDefinition::AnsatzVars> >(P.creator()));
            //            assert(false);

            for(auto i = 0u;i<errorIndicators.size();i++)
            {
              auto tolerance = 1e-3;
              if(abs(errorIndicators.at(i)) > tolerance_)
              {
//                std::cout<<"Refining index "<<i+refctr<<std::endl;
                auto insertedSpace = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::VectorCreator<VYSetDescription> >(Y.creator()).refine(i+refctr);
                ::Spacy::cast_ref<::Spacy::KaskadeParabolic::VectorCreator<VUSetDescription> >(U.creator()).refine_noGridRef(i+refctr,insertedSpace);
                ::Spacy::cast_ref<::Spacy::KaskadeParabolic::VectorCreator<VPSetDescription> >(P.creator()).refine_noGridRef(i+refctr,insertedSpace);

                c2func_impl.informAboutRefinement(i+refctr);
                refctr++;
              }

            }
          }
        }
      private:


        ::Spacy::Real tolerance_;
        std::vector<::Spacy::Real> errorIndicators;
      };

    }

  }

}

