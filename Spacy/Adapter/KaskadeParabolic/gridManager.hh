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

    template< class Spaces>
    class GridManager
    {
    public:

      using H1Space_ptr  =  typename ::boost::fusion::result_of::at_c<Spaces,0>::type;
      using H1Space = typename std::remove_reference<decltype(*std::declval<H1Space_ptr>())>::type;
      using Grid = typename H1Space::Grid;
      //      using SPFunc =  ScalarProdFunctional<double,::Kaskade::VariableSetDescription<Spaces,boost::fusion::vector<::Kaskade::VariableDescription<0,1,0> >>>;
      static constexpr int dim = Grid::dimension;

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

      GridManager() = delete;

      GridManager(unsigned N, Real T, unsigned initialRefinements = 1, unsigned FEorder = 1 , ::std::string gridType = "uniform", Real lambda = Real{-0.1}):
        initialRefinements_(initialRefinements),FEorder_(FEorder)
      {
        /// temporal grid generation done in init list
        if(gridType == "uniform")
        {
          std::cout << "generating Uniform Grid with " << N <<" points on [0,"<<T<<"]" << std::endl;
          tgptr_ = std::make_shared<TempGrid> (TempGrid(Real{0},T,N));
        }
        if(gridType == "exponential")
        {
          std::cout << "generating exponential Grid with " << N <<" points on [0,"<<T<<"]" << std::endl;
          tgptr_ = std::make_shared<TempGrid> (TempGrid(Real{0},T,N,lambda));

        }
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
        spacesVecHelper_.reserve(N);

        for (unsigned i = 0; i < N; i++) {
          spacesVecHelper_.emplace_back(std::make_shared<H1Space>(H1Space(*gm_.at(i), gm_.at(i)->grid().leafGridView(), FEorder_)));
          spacesVec_.emplace_back(std::make_shared<Spaces>(Spaces(&(*spacesVecHelper_.at(i)))));
        }
      }

      /// Move constructor.
      GridManager(GridManager&&) = default;

      /// Copy constructor.
      GridManager(const GridManager&) = default;

      /// Move assignment.
      GridManager& operator=(GridManager&& B) = default;


      /// Move assignment.
      GridManager& operator=(const GridManager& B) = default;

      const TempGrid& getTempGrid() const
      {
        return *tgptr_;
      }

      const std::vector<std::shared_ptr<Spaces> >& getSpacesVec() const
      {
        return spacesVec_;
      }

      //      const SPFunc& getScalProd() const
      //      {
      //        return SP_;
      //      }

      const std::vector<std::shared_ptr<::Kaskade::GridManager < Grid> > >& getKaskGridMan() const
      {
        return gm_;
      }

      const unsigned getFEorder() const
      {
        return FEorder_;
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

      //      const VariableSetDescription& refine(unsigned k)
      //      {
      //        // refine the grid
      //        tgptr_->refine(k);


      //        {
      //          // insert new Grid
      //          //                typedef Grid::LeafGridView GridView;
      //          //                using Mapper = ::Dune::MultipleCodimMultipleGeomTypeMapper<GridView,CellData>;

      //          //                auto GV = gm_.at(k)->grid().leafGridView();
      //          //                ::Dune::GridFactory<Grid> gf;
      //          //                Mapper mapper(GV);

      //          //                auto ctr = 0u;
      //          //                unsigned id0,id1,id2;

      //          //                for( auto&& vert : ::Dune::vertices(gm_.at(k)->grid().leafGridView()))
      //          //                    gf.insertVertex(vert.gl);
      //          //                for( auto&& ele : ::Dune::elements(gm_.at(k)->grid().leafGridView()))
      //          //                 {
      //          //                    for(auto i = 0;i<ele.geometry.corners();i++)
      //          //                        if(!gf.wasInserted())
      //          //                        gf.insertVertex(gf.insert);
      //          //                    gf.insertElement(ele.geometry().type(),{id0,id1,id2});
      //          //                }
      //          //                auto newgrid = gf.createGrid();



      //          //                ::Dune::VTKWriter<GridView> mywriter(newgrid->leafGridView());
      //          //                std::cout<<"Size of created grid "<<newgrid->leafGridView().size(dim)<<std::endl;
      //          //                mywriter.write("createdGrid");
      //        }

      //        gm_.insert(gm_.begin()+k,std::move(std::make_shared<::Kaskade::GridManager < Grid> > (::Kaskade::createUnitSquare<Grid>(1., false))));
      //        gm_.at(k)->globalRefine(initialRefinements_);
      //        gm_.at(k)->enforceConcurrentReads(true);
      //        std::cout << "Size of inserted grid at timestep " << k << ": " << gm_.at(k)->grid().leafGridView().size(dim) << std::endl;

      //        // insert new Spaces and Descriptions

      //        spacesVecHelper_.emplace_back(std::make_shared<H1Space> (H1Space(*gm_.at(k), gm_.at(k)->grid().leafGridView(), FEorder_)));
      //        spacesVec_.emplace_back(std::make_shared<Spaces>(Spaces(&(*(spacesVecHelper_.back())))));
      //        descriptionsVec_.emplace_back(VariableSetDescription(*spacesVec_.back(), {"u"}));

      //        return descriptionsVec_.back();
      //      }

      const std::shared_ptr<Spaces> refine(unsigned k)
      {
        // refine the grid
        tgptr_->refine(k);

        gm_.insert(gm_.begin()+k,std::move(std::make_shared<::Kaskade::GridManager < Grid> > (::Kaskade::createUnitSquare<Grid>(1., false))));
        gm_.at(k)->globalRefine(initialRefinements_);
        gm_.at(k)->enforceConcurrentReads(true);
        std::cout << "Size of inserted grid at timestep " << k << ": " << gm_.at(k)->grid().leafGridView().size(dim) << std::endl;

        // insert new Spaces and Descriptions

        spacesVecHelper_.emplace_back(std::make_shared<H1Space> (H1Space(*gm_.at(k), gm_.at(k)->grid().leafGridView(), FEorder_)));
        spacesVec_.emplace_back(std::make_shared<Spaces>(Spaces(&(*(spacesVecHelper_.back())))));

        return spacesVec_.back();
      }

    private:

      std::shared_ptr<TempGrid> tgptr_ = nullptr;
      std::vector<std::shared_ptr<::Kaskade::GridManager < Grid> > > gm_{};


      std::vector<std::shared_ptr<H1Space> > spacesVecHelper_{};
      std::vector<std::shared_ptr<Spaces> > spacesVec_{};


      //      SPFunc SP_ = SPFunc();
      unsigned initialRefinements_,FEorder_;

    };
  }
}
