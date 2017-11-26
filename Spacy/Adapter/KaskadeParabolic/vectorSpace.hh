#pragma once

#include <type_traits>

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/productSpace.hh>
#include <Spacy/Util/Mixins/Get.hh>

#include "l2Product.hh"
#include "vector.hh"
#include "util.hh"
#include "gridManager.hh"

#include "../Kaskade/vectorSpace.hh"
#include "boost/signals2.hpp"


namespace Spacy
{
  /** @addtogroup KaskadeGroup @{ */
  namespace KaskadeParabolic
  {
    /// Creator for vector space elements for %Kaskade 7
    template <class VariableSetDescription>
    class VectorCreator
    {
    public:
      using Spaces = typename VariableSetDescription::Spaces;

      typedef ::boost::signals2::signal<void (unsigned)> Signal;
      typedef Signal::slot_type OnRefimenentSlotType;
      /**
       * @ingroup VectorSpaceGroup
       * @brief Create from %Kaskade 7 function space.
       * @param space single %Kaskade 7 function space (no product space)
       */
      VectorCreator(GridManager<VariableSetDescription>& gm): gm_(gm)
      {
        auto descriptions = gm.getDescriptions();
        for(auto i = 0u ; i < descriptions.size(); i++)
          creators_.push_back(std::make_shared<::Spacy::Kaskade::VectorCreator<VariableSetDescription> >(Spacy::Kaskade::VectorCreator<VariableSetDescription>(descriptions.at(i))));

        S_ = std::make_shared<Signal> (Signal());
      }

      /// Generate vector for %Kaskade 7.
      Vector<VariableSetDescription> operator()(const VectorSpace* space) const
      {
        return Vector<VariableSetDescription>{*space};
      }

      const ::Spacy::Kaskade::VectorCreator<VariableSetDescription>& getSubCreator(const unsigned i) const
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
      const GridManager<VariableSetDescription>& getGridMan() const
      {
        return gm_;
      }

      void refine(unsigned k)
      {
//        std::cout<< "in VC refine function :"<<k<<std::endl;
        //refining the grid

//        std::cout<<"--------Handing over refinfo to Grids"<<std::endl;
        auto insertedDesc = gm_.refine(k);
        auto toinsertCreator = std::make_shared<::Spacy::Kaskade::VectorCreator<VariableSetDescription> >(::Spacy::Kaskade::VectorCreator<VariableSetDescription>( insertedDesc));
        this->creators_.insert(creators_.begin()+k,toinsertCreator);
//        std::cout<<"--------returning from the Grids refine function"<<std::endl;

        //tell the vectors
//        std::cout<<"--------Handing over refinfo to vectors"<<std::endl;
        this->S_->operator()(k);
//        std::cout<<"--------returning from the VC refine function"<<std::endl;

      }
      std::shared_ptr<Signal> S_;

    private:
      std::vector<std::shared_ptr<::Spacy::Kaskade::VectorCreator<VariableSetDescription> > > creators_;
      GridManager<VariableSetDescription>& gm_;
    };

    /**
     * @ingroup VectorSpaceGroup
     * @brief Create single space with hilbert space structure for %Kaskade 7.
     * @param space single %Kaskade 7 function space (no product space)
     */
    template<class VariableSetDescription>
    auto makeHilbertSpace(GridManager<VariableSetDescription>& gm)
    {
      return ::Spacy::makeHilbertSpace( KaskadeParabolic::VectorCreator<VariableSetDescription> (gm) , l2Product<VariableSetDescription>{} );
    }

  }
  /** @} */
}
