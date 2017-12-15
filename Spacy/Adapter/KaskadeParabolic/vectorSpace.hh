#pragma once

#include <type_traits>

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/productSpace.hh>
#include <Spacy/Util/Mixins/Get.hh>

//#include "Spacy/Adapter/KaskadeParabolic/l2Product.hh"
#include "Spacy/Adapter/KaskadeParabolic/gridManager.hh"
#include "Spacy/Adapter/KaskadeParabolic/vector.hh"
//#include "Spacy/Adapter/KaskadeParabolic/util.hh"

//#include "../Kaskade/vectorSpace.hh"
#include "subCreator.hh"
#include "boost/signals2.hpp"

#include "fem/variables.hh"
#include <dune/grid/uggrid.hh>


namespace Spacy
{

  /** @addtogroup KaskadeGroup @{ */
  namespace KaskadeParabolic
  {
    /// Creator for vector space elements for %Kaskade 7
    template <class Description>
    class VectorCreator
    {
    public:
      using Spaces = typename Description::Spaces;

      typedef ::boost::signals2::signal<void (unsigned)> Signal;
      typedef Signal::slot_type OnRefimenentSlotType;
      /**
       * @ingroup VectorSpaceGroup
       * @brief Create from %Kaskade 7 function space.
       * @param space single %Kaskade 7 function space (no product space)
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
        std::cout<<"in the creator"<<std::endl;
        return Vector<Description>{*space};
      }

      const SubCreator<Description>& getSubCreator(const unsigned i) const
      {
        return *(creators_.at(i));
      }

      unsigned numberOfCreators() const
      {
        return creators_.size();
      }

      const Spaces& getSpace(unsigned i) const
      {
        auto spacesVec = gm_.getSpacesVec();
        return *(spacesVec.at(i));
      }
      const ::Spacy::KaskadeParabolic::GridManager<Spaces>& getGridMan() const
      {
        return gm_;
      }

      void refine(unsigned k)
      {
//        std::cout<< "in VC refine function :"<<k<<std::endl;
        //refining the grid

//        std::cout<<"--------Handing over refinfo to Grids"<<std::endl;
          const std::shared_ptr<Spaces> insertedSpace = gm_.refine(k);
            auto toinsertCreator =
                std::make_shared<SubCreator<Description> >(SubCreator<Description>(Description(*insertedSpace,name_)));
        this->creators_.insert(creators_.begin()+k,toinsertCreator);
//        std::cout<<"--------returning from the Grids refine function"<<std::endl;

        //tell the vectors
//        std::cout<<"--------Handing over refinfo to vectors"<<std::endl;
        this->S_->operator()(k);
//        std::cout<<"--------returning from the VC refine function"<<std::endl;
      }

      void refine_noGridRef(unsigned k,Spaces& insertedSpace)
      {
//        std::cout<< "in VC refine function :"<<k<<std::endl;
        auto toinsertCreator = std::make_shared<SubCreator<Description> >(SubCreator<Description>( Description(*insertedSpace,name_)));
        this->creators_.insert(creators_.begin()+k,toinsertCreator);
//        std::cout<<"--------returning from the Grids refine function"<<std::endl;

        //tell the vectors
//        std::cout<<"--------Handing over refinfo to vectors"<<std::endl;
        this->S_->operator()(k);
//        std::cout<<"--------returning from the VC refine function"<<std::endl;
      }
      std::shared_ptr<Signal> S_;

    private:
      std::vector<std::shared_ptr<SubCreator<Description> > > creators_;
      ::Spacy::KaskadeParabolic::GridManager<Spaces>& gm_;
      std::vector<std::string> name_{};
    };

    /**
     * @ingroup VectorSpaceGroup
     * @brief Create single space with hilbert space structure for %Kaskade 7.
     * @param space single %Kaskade 7 function space (no product space)
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
     * @brief Create product space with hilbert space structure for %Kaskade 7.
     * @param spaces boost fusion forward sequence of const pointers to %Kaskade 7 function spaces
     * @param primalIds ids of primal variables
     * @param dualIds ids of dual variables
     */
    template<class Spaces>
    auto makeHilbertSpace(GridManager<Spaces>& gm, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds = {})
    {
      //assume optimal control context
      assert(primalIds.size() == 2 && dualIds.size() == 1);

      unsigned n = 3;
      std::vector<std::shared_ptr< VectorSpace > > newSpaces( n );
      std::cout << "create space with " << n << " variables." << std::endl;

      using VD = boost::fusion::vector<::Kaskade::VariableDescription<0,1,0> >;
      using VariableSetDescription = ::Kaskade::VariableSetDescription<Spaces,VD>;

      //State
      newSpaces.at(0) = std::make_shared<VectorSpace>(::Spacy::makeHilbertSpace(
                   ::Spacy::KaskadeParabolic::VectorCreator<VariableSetDescription>(gm,"y"),::Spacy::KaskadeParabolic::l2Product() ) );
      //Cotnrol
      newSpaces.at(1) = std::make_shared<VectorSpace>(::Spacy::makeHilbertSpace(
          ::Spacy::KaskadeParabolic::VectorCreator<VariableSetDescription>(gm,"u"),::Spacy::KaskadeParabolic::l2Product() ) );
      //Adjoint
      newSpaces.at(2) = std::make_shared<VectorSpace>(::Spacy::makeHilbertSpace(
          ::Spacy::KaskadeParabolic::VectorCreator<VariableSetDescription>(gm,"p"),::Spacy::KaskadeParabolic::l2Product() ) );

       return ::Spacy::ProductSpace::makeHilbertSpace( newSpaces , primalIds , dualIds );
    }

  }
  /** @} */
}
