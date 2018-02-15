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
      /**
           * @brief Write #timesteps VTK files for time dependent function
           * @tparam Description Kaskade VariableSetDescription associated with x
           * @param x KaskadeParabolic Vector that holds the vector(i.e. function) to be written
           * @param fileName name of file to be written
           */
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

      /**
           * @brief Write #timesteps VTK files for time dependent function
           * @tparam Description Kaskade VariableSetDescription associated with x
           * @param x Spacy Vector that holds the vector(i.e. function) to be written
           * @param fileName name of file to be written
           */
      template <class Description>
      void writeVTK(const Spacy::Vector& x, const char* fileName)
      {
        auto x_impl = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::Vector<Description> > (x);
        auto vc = ::Spacy::creator<VectorCreator<Description> >(x.space());
        auto gm = vc.getGridMan().getKaskGridMan();
        for(auto i = 0u ; i<gm.size() ; i++)
        {
          ::Kaskade::writeVTKFile(gm.at(i)->grid().leafGridView(), x_impl.get(i) ,fileName+std::to_string(i),::Kaskade::IoOptions(),vc.getGridMan().getFEorder());
        }
      }

      /**
           * @brief write text files of the norm of the solution
           * @tparam Descriptions Kaskade VariableSetDescription associated with x
           * @param sol Spacy Vector that holds the productspace vector to be written
           * @param Blockoperator Functional the norm will be computed with
           * @param gm GridManager of exact solution
           * @param name output filename
           */
      template<class Descriptions>
      auto printNormSolution(::Spacy::Vector sol,::Spacy::KaskadeParabolic::PDE::LinearBlockOperator<Descriptions,Descriptions> Blockoperator,
                             ::Spacy::KaskadeParabolic::GridManager<typename Descriptions::Spaces> gm, std::string name)
      {

        using CoefficientVectorY = typename Descriptions::template CoefficientVectorRepresentation<>::type;

        // compute norm of exact solution
        auto timesteps = gm.getSpacesVec().size();
        auto sol_impl = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::Vector<Descriptions> > (sol);
        std::vector<double> norm_y;

        for(auto i = 0u; i< gm.getSpacesVec().size(); i++)
        {
          CoefficientVectorY ydum(
                Descriptions::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
          boost::fusion::at_c<0>(ydum.data) = sol_impl.getCoeffVec(i);
          auto My = ydum;
          Blockoperator.getKaskOp("MassY", i).apply(ydum, My);
          norm_y.push_back(std::sqrt(ydum*My));
        }

        // Print into files
        std::ofstream norm_y_os;
        norm_y_os.open("./data/norm_y_"+name+".txt");

        auto tempgrid = gm.getTempGrid().getVertexVec();

        for(auto i = 0u; i < timesteps; i++)
        {
          norm_y_os << tempgrid.at(i) << std::setprecision(std::numeric_limits<long double>::digits10 + 1)  << " " <<  norm_y.at(i) << std::endl;
        }
        norm_y_os.close();

      }

      /**
           * @brief write text files of the norm of the solution
           * @tparam Descriptions Kaskade VariableSetDescription associated with x
           * @param sol Spacy Vector that holds the productspace vector to be written
           * @param Blockoperator Functional the norm will be computed with
           * @param gm GridManager of exact solution
           * @param name output filename
           */
      template<class Descriptions>
      auto printNormSolution(::Spacy::Vector sol,::Spacy::KaskadeParabolic::PDE::NormOperator<Descriptions,Descriptions> NormOperator,
                             ::Spacy::KaskadeParabolic::GridManager<typename Descriptions::Spaces> gm, std::string name)
      {

        using CoefficientVectorY = typename Descriptions::template CoefficientVectorRepresentation<>::type;

        // compute norm of exact solution
        auto timesteps = gm.getSpacesVec().size();
        auto sol_impl = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::Vector<Descriptions> > (sol);
        std::vector<double> norm_y;

        for(auto i = 0u; i< gm.getSpacesVec().size(); i++)
        {
          CoefficientVectorY ydum(
                Descriptions::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
          boost::fusion::at_c<0>(ydum.data) = sol_impl.getCoeffVec(i);
          auto My = ydum;
          NormOperator.getKaskOp("MassY", i).apply(ydum, My);
          norm_y.push_back(std::sqrt(ydum*My));
        }

        // Print into files
        std::ofstream norm_y_os;
        norm_y_os.open("./data/norm_y_"+name+".txt");

        auto tempgrid = gm.getTempGrid().getVertexVec();

        for(auto i = 0u; i < timesteps; i++)
        {
          norm_y_os << tempgrid.at(i) << std::setprecision(std::numeric_limits<long double>::digits10 + 1)  << " " <<  norm_y.at(i) << std::endl;
        }
        norm_y_os.close();

      }

    }
    namespace OCP
    {

      /**
           * @brief Write #timesteps VTK files for time dependent triple
           * @tparam VariableSetDescription Kaskade VariableSetDescription associated with x
           * @param x Spacy Vector that holds the productspace vector to be written
           * @param fileName name of file to be written
           */
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
          ::Kaskade::writeVTKFile(gm.at(i)->grid().leafGridView(), v.at(i) ,fileName+std::to_string(i),::Kaskade::IoOptions(),vc.getGridMan().getFEorder());
        }
        return;
      }

      /**
           * @brief Write text files of relative and absolute error
           * @tparam Descriptions Kaskade VariableSetDescription associated with x
           * @param ex Spacy Vector of a finer("exact") solution
           * @param inex Spacy vector of perturbed("inexact") solution
           * @param NormFunctional Functional the norm will be computed with
           * @param gm GridManager of exact solution
           * @param name output filename
           */
      template<class Descriptions>
      auto printErrorDistribution(::Spacy::Vector ex,::Spacy::Vector inex, ::Spacy::KaskadeParabolic::OCP::LinearBlockOperator<Descriptions,Descriptions> NormFunctional,
                                  ::Spacy::KaskadeParabolic::GridManager<typename Descriptions::Spaces> gm, std::string name)
      {
        using VYSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,0>;
        using VUSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,1>;
        using VPSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,2>;

        using CoefficientVectorY = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
        using CoefficientVectorU = typename VUSetDescription::template CoefficientVectorRepresentation<>::type;
        using CoefficientVectorP = typename VPSetDescription::template CoefficientVectorRepresentation<>::type;

        // diff = inex-ex
        auto ex_copy = ex;
        auto diff_tuple = getImpl<Descriptions>(ex_copy);
        auto inex_tuple = getImpl<Descriptions>(inex);
        // exact -= inexact
        //        std::get<0>(diff_tuple).subtractOtherTempGrid(std::get<0>(inex_tuple));
        //        std::get<1>(diff_tuple).subtractOtherTempGrid(std::get<1>(inex_tuple));
        //        std::get<2>(diff_tuple).subtractOtherTempGrid(std::get<2>(inex_tuple));

        std::get<0>(diff_tuple).subtractOtherTempGrid_linearInterpolated(std::get<0>(inex_tuple));
        std::get<1>(diff_tuple).subtractOtherTempGrid_linearInterpolated(std::get<1>(inex_tuple));
        std::get<2>(diff_tuple).subtractOtherTempGrid_linearInterpolated(std::get<2>(inex_tuple));

        auto timesteps = gm.getSpacesVec().size();
        std::vector<double> diffnorm_y,diffnorm_u,diffnorm_p;

        // compute norm of diff
        for(auto i = 0u; i<gm.getSpacesVec().size(); i++)
        {
          CoefficientVectorY ydum(
                VYSetDescription::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
          boost::fusion::at_c<0>(ydum.data) = std::get<0>(diff_tuple).getCoeffVec(i);
          auto My = ydum;
          NormFunctional.getKaskOp("My", i).apply(ydum, My);
          diffnorm_y.push_back(std::sqrt(ydum*My));

          CoefficientVectorU udum(
                VUSetDescription::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
          boost::fusion::at_c<0>(udum.data) = std::get<1>(diff_tuple).getCoeffVec(i);
          auto Mu = udum;
          NormFunctional.getKaskOp("Mu", i).apply(udum, Mu);
          diffnorm_u.push_back(std::sqrt(udum*Mu));

          CoefficientVectorP pdum(
                VPSetDescription::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
          boost::fusion::at_c<0>(pdum.data) = std::get<2>(diff_tuple).getCoeffVec(i);
          auto Mp = pdum;
          NormFunctional.getKaskOp("My", i).apply(pdum, Mp);
          diffnorm_p.push_back(std::sqrt(pdum*Mp));
        }

        // compute norm of exact solution
        auto ex_tuple = getImpl<Descriptions>(ex);
        std::vector<double> exnorm_y, exnorm_u, exnorm_p;
        for(auto i = 0u; i< gm.getSpacesVec().size(); i++)
        {
          CoefficientVectorY ydum(
                VYSetDescription::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
          boost::fusion::at_c<0>(ydum.data) = std::get<0>(ex_tuple).getCoeffVec(i);
          auto My = ydum;
          NormFunctional.getKaskOp("My", i).apply(ydum, My);
          exnorm_y.push_back(std::sqrt(ydum*My));

          CoefficientVectorU udum(
                VUSetDescription::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
          boost::fusion::at_c<0>(udum.data) = std::get<1>(ex_tuple).getCoeffVec(i);
          auto Mu = udum;
          NormFunctional.getKaskOp("Mu", i).apply(udum, Mu);
          exnorm_u.push_back(std::sqrt(udum*Mu));

          CoefficientVectorP pdum(
                VPSetDescription::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
          boost::fusion::at_c<0>(pdum.data) = std::get<2>(ex_tuple).getCoeffVec(i);
          auto Mp = pdum;
          NormFunctional.getKaskOp("My", i).apply(pdum, Mp);
          exnorm_p.push_back(std::sqrt(pdum*Mp));
        }

        // Print into files
        std::ofstream relerr_y, relerr_u, relerr_p,
            abserr_y, abserr_u, abserr_p;
        relerr_y.open("./data/relerr_y_"+name+".txt");
        relerr_u.open("./data/relerr_u_"+name+".txt");
        relerr_p.open("./data/relerr_p_"+name+".txt");

        abserr_y.open("./data/abserr_y_"+name+".txt");
        abserr_u.open("./data/abserr_u_"+name+".txt");
        abserr_p.open("./data/abserr_p_"+name+".txt");
        auto tempgrid = gm.getTempGrid().getVertexVec();

        for(auto i = 0u; i < timesteps; i++)
        {
          relerr_y  << tempgrid.at(i) << " "<< std::setprecision(std::numeric_limits<long double>::digits10 + 1) << diffnorm_y.at(i) / exnorm_y.at(i) << std::endl;
          relerr_u  << tempgrid.at(i) << " " << std::setprecision(std::numeric_limits<long double>::digits10 + 1)<< diffnorm_u.at(i) / exnorm_u.at(i) << std::endl;
          relerr_p << tempgrid.at(i) << " "<< std::setprecision(std::numeric_limits<long double>::digits10 + 1)  << diffnorm_p.at(i) / exnorm_p.at(i) << std::endl;

          abserr_y  << tempgrid.at(i) << " "<< std::setprecision(std::numeric_limits<long double>::digits10 + 1) << diffnorm_y.at(i) << std::endl;
          abserr_u  << tempgrid.at(i) << " " << std::setprecision(std::numeric_limits<long double>::digits10 + 1)<< diffnorm_u.at(i) << std::endl;
          abserr_p << tempgrid.at(i) << " "<< std::setprecision(std::numeric_limits<long double>::digits10 + 1)  << diffnorm_p.at(i) << std::endl;
        }
        relerr_y.close();
        relerr_u.close();
        relerr_p.close();
        abserr_y.close();
        abserr_u.close();
        abserr_p.close();
      }

      /**
           * @brief write text files of the norm of the solution
           * @tparam Descriptions Kaskade VariableSetDescription associated with x
           * @param sol Spacy Vector that holds the productspace vector to be written
           * @param NormFunctional Functional the norm will be computed with
           * @param gm GridManager of exact solution
           * @param name output filename
           */
      template<class Descriptions>
      auto printNormSolution(::Spacy::Vector sol,::Spacy::KaskadeParabolic::OCP::LinearBlockOperator<Descriptions,Descriptions> NormFunctional,
                             ::Spacy::KaskadeParabolic::GridManager<typename Descriptions::Spaces> gm, std::string name)
      {
        using VYSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,0>;
        using VUSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,1>;
        using VPSetDescription = ::Spacy::KaskadeParabolic::Detail::ExtractDescription_t<Descriptions,2>;

        using CoefficientVectorY = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
        using CoefficientVectorU = typename VUSetDescription::template CoefficientVectorRepresentation<>::type;
        using CoefficientVectorP = typename VPSetDescription::template CoefficientVectorRepresentation<>::type;


        // compute norm of exact solution
        auto timesteps = gm.getSpacesVec().size();
        auto sol_tuple = getImpl<Descriptions>(sol);
        std::vector<double> norm_y, norm_u, norm_p;

        for(auto i = 0u; i< gm.getSpacesVec().size(); i++)
        {
          CoefficientVectorY ydum(
                VYSetDescription::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
          boost::fusion::at_c<0>(ydum.data) = std::get<0>(sol_tuple).getCoeffVec(i);
          auto My = ydum;
          NormFunctional.getKaskOp("My", i).apply(ydum, My);
          norm_y.push_back(std::sqrt(ydum*My));

          CoefficientVectorU udum(
                VUSetDescription::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
          boost::fusion::at_c<0>(udum.data) = std::get<1>(sol_tuple).getCoeffVec(i);
          auto Mu = udum;
          NormFunctional.getKaskOp("Mu", i).apply(udum, Mu);
          norm_u.push_back(std::sqrt(udum*Mu));

          CoefficientVectorP pdum(
                VPSetDescription::template CoefficientVectorRepresentation<>::init(*gm.getSpacesVec().at(i)));
          boost::fusion::at_c<0>(pdum.data) = std::get<2>(sol_tuple).getCoeffVec(i);
          auto Mp = pdum;
          NormFunctional.getKaskOp("My", i).applyscaleaddTransposed(1.,pdum, Mp);
          norm_p.push_back(std::sqrt(pdum*Mp));
        }

        // Print into files
        std::ofstream relerr_y, relerr_u, relerr_p;
        relerr_y.open("./data/norm_y_"+name+".txt");
        relerr_u.open("./data/norm_u_"+name+".txt");
        relerr_p.open("./data/norm_p_"+name+".txt");
        auto tempgrid = gm.getTempGrid().getVertexVec();

        for(auto i = 0u; i < timesteps; i++)
        {
          relerr_y << tempgrid.at(i) << std::setprecision(std::numeric_limits<long double>::digits10 + 1)  << " " <<  norm_y.at(i) << std::endl;
          relerr_u << tempgrid.at(i) << std::setprecision(std::numeric_limits<long double>::digits10 + 1)  << " " <<  norm_u.at(i) << std::endl;
          relerr_p << tempgrid.at(i) << std::setprecision(std::numeric_limits<long double>::digits10 + 1)  << " " <<  norm_p.at(i) << std::endl;
        }

      }

    }

  }

}

