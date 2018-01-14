#pragma once

#include <type_traits>

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/productSpace.hh>
#include <Spacy/Util/Mixins/Get.hh>

#include "gridManager.hh"
#include "vector.hh"
#include "subCreator.hh"

#include "boost/signals2.hpp"

#include "fem/variables.hh"
#include <dune/grid/uggrid.hh>


namespace Spacy
{

  /** @addtogroup KaskadeParabolicGroup @{ */
  namespace KaskadeParabolic
  {
    /// Creator for time dependent (piecewise constant in time) vector space elements implemented with %Kaskade 7
    template <class Description>
    class VectorCreator
    {
    public:
      using Spaces = typename Description::Spaces;
      using Signal = ::boost::signals2::signal<void (unsigned)>;
      using OnRefimenentSlotType = Signal::slot_type;

      /**
       * @ingroup VectorSpaceGroup
       * @brief Create from Gridmanager
       * @param gm Spacy Gridmanager holding grid information (temporal and spatial)
       * @param name name of variable (needed for writing paraview files)
       */
      VectorCreator(::Spacy::KaskadeParabolic::GridManager<Spaces>& gm,std::string name): gm_(gm)
      {
        name_.push_back(name);
        auto spaces = gm.getSpacesVec();

        for(auto i = 0u ; i < spaces.size(); i++)
          creators_.push_back(std::make_shared<SubCreator<Description> >(SubCreator<Description>(Description(*spaces.at(i),name_))));

        S_ = std::make_shared<Signal> (Signal());
      }

      /// Generate vector for %Kaskade 7.
      Vector<Description> operator()(const VectorSpace* space) const
      {
        return Vector<Description>{*space};
      }

      /**
           * @brief getter for creators of each timestep
           * @param i index of requested creator
           * @return SubCreator for time interval i
           */
      const SubCreator<Description>& getSubCreator(const unsigned i) const
      {
        return *(creators_.at(i));
      }

      /// get number of Creators
      unsigned numberOfCreators() const
      {
        return creators_.size();
      }

      /// get Kaskade Space of time interval i
      const Spaces& getSpace(unsigned i) const
      {
        auto spacesVec = gm_.getSpacesVec();
        return *(spacesVec.at(i));
      }

      /**
           * @brief getter for Spacy Gridmanager
           * @return Spacy GridManager
           */
      const ::Spacy::KaskadeParabolic::GridManager<Spaces>& getGridMan() const
      {
        return gm_;
      }

      /**
           * @brief Refine the VectorSpace in time, calls the refine on GridManager and the vectors (via boost::signal)
           * @param k index of interval to be refined
           * @return Kaskade Space that was inserted
           */
      std::shared_ptr<Spaces> refine(unsigned k)
      {
        //refining the grid
        const std::shared_ptr<Spaces> insertedSpace = gm_.refine(k);

        ///insert a subcreator
        auto toinsertCreator =
            std::make_shared<SubCreator<Description> >(SubCreator<Description>(Description(*insertedSpace,name_)));
        this->creators_.insert(creators_.begin()+k,toinsertCreator);

        //Invoke refinement on the vectors
        this->S_->operator()(k);
        return insertedSpace;
      }

      /**
           * @brief Refine the VectorSpace in time, calls the refine the vectors (via boost::signal)
           * NOT ON THE GRID!! This function assumes the grid was refined (e.g. y space was refined via refine(k), now u space refinement with this function)
           * @param k index of interval to be refined
           * @param insertedSpace Kaskade space that was inserted when calling the above refine(k)
           */
      void refine_noGridRef(unsigned k,std::shared_ptr<Spaces> insertedSpace)
      {
        ///insert a subcreator
        auto toinsertCreator = std::make_shared<SubCreator<Description> >(SubCreator<Description>( Description(*insertedSpace,name_)));
        this->creators_.insert(creators_.begin()+k,toinsertCreator);

        //Invoke refinement on the vectors
        this->S_->operator()(k);
      }
      std::shared_ptr<Signal> S_;

    private:
      std::vector<std::shared_ptr<SubCreator<Description> > > creators_;
      ::Spacy::KaskadeParabolic::GridManager<Spaces>& gm_;
      std::vector<std::string> name_{};
    };

    /**
     * @ingroup VectorSpaceGroup
     * @brief Create single space of piecewise constant in time functions with hilbert space structure with %Kaskade 7.
     * @param gm Spacy Gridmanager holding grid information (temporal and spatial)
     */
    template<class Spaces>
    auto makeHilbertSpace(GridManager<Spaces>& gm)
    {

      using VD = boost::fusion::vector<::Kaskade::VariableDescription<0,1,0> >;
      using VariableSetDescription = ::Kaskade::VariableSetDescription<Spaces,VD>;

      return ::Spacy::makeHilbertSpace( KaskadeParabolic::VectorCreator<VariableSetDescription> (gm,"y") , l2Product{} );
    }

    /**
     * @ingroup VectorSpaceGroup
     * @brief Create product space piecewise constant in time functions with hilbert space structure with %Kaskade 7.
     * @param gm Spacy Gridmanager holding grid information (temporal and spatial)
     * @param primalIds ids of primal variables
     * @param dualIds ids of dual variables
     */
    template<class Spaces>
    auto makeHilbertSpace(GridManager<Spaces>& gm, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds)
    {
      //assume optimal control context
      assert(primalIds.size() == 2 && dualIds.size() == 1);

      std::vector<std::shared_ptr< VectorSpace > > newSpaces;

      //State
      using VD = boost::fusion::vector<::Kaskade::VariableDescription<0,1,0> >;
      using VariableSetDescription = ::Kaskade::VariableSetDescription<Spaces,VD>;
      newSpaces.push_back(std::make_shared<VectorSpace>(::Spacy::makeHilbertSpace(
                                                          ::Spacy::KaskadeParabolic::VectorCreator<VariableSetDescription>(gm,"y"),::Spacy::KaskadeParabolic::l2Product() ) ));
      //Control
      using VD2 = boost::fusion::vector<::Kaskade::VariableDescription<0,1,1> >;
      using VariableSetDescription2 = ::Kaskade::VariableSetDescription<Spaces,VD2>;
      newSpaces.push_back(std::make_shared<VectorSpace>(::Spacy::makeHilbertSpace(
                                                          ::Spacy::KaskadeParabolic::VectorCreator<VariableSetDescription2>(gm,"u"),::Spacy::KaskadeParabolic::l2Product() ) ));
      //Adjoint
      using VD3 = boost::fusion::vector<::Kaskade::VariableDescription<0,1,2> >;
      using VariableSetDescription3 = ::Kaskade::VariableSetDescription<Spaces,VD3>;
      newSpaces.push_back(std::make_shared<VectorSpace>(::Spacy::makeHilbertSpace(
                                                          ::Spacy::KaskadeParabolic::VectorCreator<VariableSetDescription3>(gm,"p"),::Spacy::KaskadeParabolic::l2Product() ) ));

      std::cout << "create space with " << newSpaces.size() << " variables." << std::endl;

      return ::Spacy::ProductSpace::makeHilbertSpace( newSpaces , primalIds , dualIds );
    }

  }
  /** @} */
}
