#pragma once

#include <Spacy/vector.hh>

#include "vector.hh"
#include "io/vtk.hh"


namespace Spacy
{
  namespace KaskadeParabolic
  {
    namespace PDE
    {
      template <class Description>
      void writeVTK(const Vector<Description>& x, const char* fileName)
      {
        auto vc = ::Spacy::creator<VectorCreator<Description> >(x.space());
        auto gm = vc.getGridMan().getKaskGridMan();
        for(auto i = 0u ; i<gm.size() ; i++)
        {
          ::Kaskade::writeVTKFile(gm.at(i)->grid().leafGridView(), x.get(i) ,fileName+std::to_string(i),::Kaskade::IoOptions(),vc.getGridMan().getFEorder());
        }
      }
    }
    namespace OCP
    {
      template <class VariableSetDescription>
      void writeVTK(const ::Spacy::Vector& x, const char* fileName)
      {
        using VYSetDescription = Detail::ExtractDescription_t<VariableSetDescription,0>;
        using VUSetDescription = Detail::ExtractDescription_t<VariableSetDescription,1>;
        using VPSetDescription = Detail::ExtractDescription_t<VariableSetDescription,2>;
        using VectorImplY = Vector<VYSetDescription>;
        using VectorImplU = Vector<VUSetDescription>;
        using VectorImplP = Vector<VPSetDescription>;

        using VarSet = typename VariableSetDescription::VariableSet;
        using PSV = ::Spacy::ProductSpace::Vector;
        auto x_ps = ::Spacy::cast_ref<PSV>(x);

        //subvectors as Spacy::Vector
        auto x_y = (::Spacy::cast_ref<PSV>(x_ps.component(PRIMAL))).component(0);
        auto x_u = (::Spacy::cast_ref<PSV>(x_ps.component(PRIMAL))).component(1);
        auto x_p = (::Spacy::cast_ref<PSV>(x_ps.component(DUAL))).component(0);

        //Implementation on as Spacy::KaskadeParabolic::Vector
        assert(Spacy::is<VectorImplY>(x_y));
        auto x_y_impl = ::Spacy::cast_ref<VectorImplY>(x_y);
        assert(::Spacy::is<VectorImplU>(x_u));
        auto x_u_impl = ::Spacy::cast_ref<VectorImplU>(x_u);
        assert(::Spacy::is<VectorImplP>(x_p));
        auto x_p_impl = ::Spacy::cast_ref<VectorImplP>(x_p);

        auto vc = ::Spacy::creator<::Spacy::KaskadeParabolic::VectorCreator<VYSetDescription> >(x_y.space());
        auto gm = vc.getGridMan().getKaskGridMan();

        auto no_time_steps = gm.size();

        std::vector<VarSet> v;
        for(auto i = 0u;i<no_time_steps;i++)
        {
          VariableSetDescription varSetDesc(vc.getSpace(i),{"y","u","p"});
          v.push_back(VarSet(varSetDesc));
          ::boost::fusion::at_c<0>(v.at(i).data) = boost::fusion::at_c<0>(x_y_impl.get(i).data);
          ::boost::fusion::at_c<1>(v.at(i).data) = boost::fusion::at_c<0>(x_u_impl.get(i).data);
          ::boost::fusion::at_c<2>(v.at(i).data) = boost::fusion::at_c<0>(x_p_impl.get(i).data);
          std::cout<<"writing vtk file "<<i<<std::endl;
          ::Kaskade::writeVTKFile(gm.at(i)->grid().leafGridView(), v.at(i) ,fileName+std::to_string(i),::Kaskade::IoOptions(),vc.getGridMan().getFEorder());
        }
        std::cout<<"done writing"<<std::endl;
        return;
      }

    }

  }

}

