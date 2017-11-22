#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <dune/grid/config.h>
#include <dune/grid/uggrid.hh>

#include "fem/gridmanager.hh"
#include "fem/fixdune.hh"
#include "fem/lagrangespace.hh"
#include "fem/variables.hh"
#include "utilities/kaskopt.hh"
#include "fem/spaces.hh"
#include "utilities/gridGeneration.hh" //  createUnitSquare

#include "scalprod.hh"
#include "tempGrid.hh"

#include <boost/fusion/tuple.hpp>

#include <type_traits>

#include <dune/grid/io/file/vtk/vtkwriter.hh>
#include <dune/grid/uggrid/uggridfactory.hh>

namespace Spacy
{
  namespace KaskadeParabolic
  {

    template <int dim >
    struct  CellData {
      bool  contains (::Dune::GeometryType  gt) {
        return  gt.dim() == dim;
      }
    };

    class DynamicProblem
    {
    public:
      using Grid = Dune::UGGrid<2>;

      using H1Space_K = typename ::Kaskade::FEFunctionSpace < ::Kaskade::ContinuousLagrangeMapper<double, Grid::LeafGridView> >;
      using Spaces = ::boost::fusion::vector<H1Space_K const *>;

      using VariableDescriptions = ::boost::fusion::vector<::Kaskade::Variable < ::Kaskade::SpaceIndex < 0>,::Kaskade::Components < 1 > , ::Kaskade::VariableId < 0 > > >;
      using VariableSetDescription = ::Kaskade::VariableSetDescription<Spaces, VariableDescriptions>;

      using SPFunc =  ScalarProdFunctional<double,VariableSetDescription>;
      static constexpr int dim = VariableSetDescription::Grid::dimension;

      /**
             * @brief Construct dynamicProblem for %Kaskade 7 on the interval [0,T].
             * @param P definition of the non-time-derivative part of the pde from %Kaskade 7
             * @param N number of desired timesteps
             * @param T endtime
             * @param initialRefinements refinements of spatial grid
             * @param Finite element order
             *
             * Time grid will always start at 0
             */
    public:

      DynamicProblem() = delete;

      DynamicProblem(unsigned N, double T, unsigned initialRefinements = 1, unsigned FEorder = 1):
        initialRefinements_(initialRefinements),FEorder_(FEorder)
      {
        /// temporal grid generation done in init list
        tgptr_ = std::make_shared<TempGrid> (TempGrid(0,T,N));

        /// Spatial grid generation
        gm_.reserve(N);
        for (auto i = 0u; i < N; i++) {
          gm_.push_back(std::move(std::make_shared<::Kaskade::GridManager < Grid> > (::Kaskade::createUnitSquare<Grid>(1., false))));
          gm_.at(i)->globalRefine(initialRefinements);
          gm_.at(i)->enforceConcurrentReads(true);
          std::cout << "Size of grid at timestep " << i << ": " << gm_.at(i)->grid().leafGridView().size(dim) << std::endl;
        }

        /// construction of function spaces
        spacesVec_.reserve(N);
        descriptionsVec_.reserve(N);
        spacesVecHelper_.reserve(N);

        for (unsigned i = 0; i < N; i++) {
          spacesVecHelper_.emplace_back(std::make_shared<H1Space_K>(H1Space_K(*gm_.at(i), gm_.at(i)->grid().leafGridView(), FEorder_)));
          spacesVec_.emplace_back(std::make_shared<Spaces>(Spaces(&(*spacesVecHelper_.at(i)))));
          descriptionsVec_.emplace_back(VariableSetDescription(*spacesVec_.at(i), {"u"}));
        }
      }

      /// Move constructor.
      DynamicProblem(DynamicProblem&&) = default;

      /// Copy constructor.
      DynamicProblem(const DynamicProblem&) = default;

      /// Move assignment.
      DynamicProblem& operator=(DynamicProblem&& B) = default;


      /// Move assignment.
      DynamicProblem& operator=(const DynamicProblem& B) = default;

      const std::vector<VariableSetDescription>& getDescriptions() const
      {
        return descriptionsVec_;
      }

      const TempGrid& getTempGrid() const
      {
        return *tgptr_;
      }

      const std::vector<std::shared_ptr<Spaces> >& getSpacesVec() const
      {
        return spacesVec_;
      }

      const SPFunc& getScalProd() const
      {
        return SP_;
      }

      const std::vector<std::shared_ptr<::Kaskade::GridManager < Grid> > >& getGridMan() const
      {
        return gm_;
      }

      void refine(const std::vector<unsigned>& indizes)
      {
        assert(std::is_sorted(indizes.begin(),indizes.end()));
        unsigned refctr = 0;
        for(auto ind : indizes)
        {
          this->refine(ind+refctr);
          refctr++;
        }

      }

      const VariableSetDescription& refine(unsigned k)
      {
        // refine the grid
        tgptr_->refine(k);


        {
          // insert new Grid
          //                typedef Grid::LeafGridView GridView;
          //                using Mapper = ::Dune::MultipleCodimMultipleGeomTypeMapper<GridView,CellData>;

          //                auto GV = gm_.at(k)->grid().leafGridView();
          //                ::Dune::GridFactory<Grid> gf;
          //                Mapper mapper(GV);

          //                auto ctr = 0u;
          //                unsigned id0,id1,id2;

          //                for( auto&& vert : ::Dune::vertices(gm_.at(k)->grid().leafGridView()))
          //                    gf.insertVertex(vert.gl);
          //                for( auto&& ele : ::Dune::elements(gm_.at(k)->grid().leafGridView()))
          //                 {
          //                    for(auto i = 0;i<ele.geometry.corners();i++)
          //                        if(!gf.wasInserted())
          //                        gf.insertVertex(gf.insert);
          //                    gf.insertElement(ele.geometry().type(),{id0,id1,id2});
          //                }
          //                auto newgrid = gf.createGrid();



          //                ::Dune::VTKWriter<GridView> mywriter(newgrid->leafGridView());
          //                std::cout<<"Size of created grid "<<newgrid->leafGridView().size(dim)<<std::endl;
          //                mywriter.write("createdGrid");
        }

        gm_.insert(gm_.begin()+k,std::move(std::make_shared<::Kaskade::GridManager < Grid> > (::Kaskade::createUnitSquare<Grid>(1., false))));
        gm_.at(k)->globalRefine(initialRefinements_);
        gm_.at(k)->enforceConcurrentReads(true);
        std::cout << "Size of inserted grid at timestep " << k << ": " << gm_.at(k)->grid().leafGridView().size(dim) << std::endl;

        // insert new Spaces and Descriptions

        spacesVecHelper_.emplace_back(std::make_shared<H1Space_K> (H1Space_K(*gm_.at(k), gm_.at(k)->grid().leafGridView(), FEorder_)));
        spacesVec_.emplace_back(std::make_shared<Spaces>(Spaces(&(*(spacesVecHelper_.back())))));
        descriptionsVec_.emplace_back(VariableSetDescription(*spacesVec_.back(), {"u"}));

        return descriptionsVec_.back();
      }

    private:

      std::shared_ptr<TempGrid> tgptr_ = nullptr;
      std::vector<std::shared_ptr<::Kaskade::GridManager < Grid> > > gm_{};


      std::vector<VariableSetDescription> descriptionsVec_{};
      std::vector<std::shared_ptr<H1Space_K> > spacesVecHelper_{};
      std::vector<std::shared_ptr<Spaces> > spacesVec_{};


      SPFunc SP_ = SPFunc();
      unsigned initialRefinements_,FEorder_;

    };
  }
}
