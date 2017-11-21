#pragma once

#include <type_traits>

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/productSpace.hh>
#include <Spacy/Util/Mixins/Get.hh>

#include "l2Product.hh"
#include "vector.hh"
#include "util.hh"

#include "../Kaskade/vectorSpace.hh"
#include "boost/signals2.hpp"


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
      typedef ::boost::signals2::signal<void (unsigned,VectorSpace&)> Signal;
      typedef Signal::slot_type OnRefimenentSlotType;
      /**
       * @ingroup VectorSpaceGroup
       * @brief Create from %Kaskade 7 function space.
       * @param space single %Kaskade 7 function space (no product space)
       */
      VectorCreator(const std::vector<Description> descriptions, const std::vector<std::shared_ptr<Spaces> > spaces): spaces_(spaces)
      {
        for(auto i = 0u ; i < descriptions.size(); i++)
          creators_.push_back(::Spacy::Kaskade::VectorCreator<Description>(descriptions.at(i)));

        S_ = std::make_shared<Signal> (Signal());
      }

      /// Generate vector for %Kaskade 7.
      Vector<Description> operator()(const VectorSpace* space) const
      {
        return Vector<Description>{*space};
      }

      ::Spacy::Kaskade::VectorCreator<Description> subcreator(const unsigned i) const
      {
        return creators_.at(i);
      }

      unsigned numberOfCreators() const
      {
        return creators_.size();
      }

      Spaces getSpace(unsigned i) const
      {
        return *spaces_.at(i);
      }

      void refine(unsigned k)
      {
        std::cout<< "in VC refine function :"<<k<<std::endl;
        //refining the grid
        //        auto insertedVSptr = DP_.refine(k);

        //        this->creators_.insert(k,creator< VectorCreator<Description> >(*insertedVSptr));

        //tell the vectors
        auto vsdum = VectorSpace();
        std::cout<<"--------Handing over refinfo to vectors"<<std::endl;
        this->S_->operator()(k,vsdum);
        std::cout<<"--------returning from the VC refine function"<<std::endl;
      }
      std::shared_ptr<Signal> S_;

    private:
      std::vector<::Spacy::Kaskade::VectorCreator<Description> > creators_;
      std::vector< std::shared_ptr<Spaces> > spaces_;
    };

    /**
     * @ingroup VectorSpaceGroup
     * @brief Create single space with hilbert space structure for %Kaskade 7.
     * @param space single %Kaskade 7 function space (no product space)
     */
    template <class Description>
    auto makeHilbertSpace(const std::vector<Description> descriptions, const std::vector<std::shared_ptr< typename Description::Spaces> > spaces)
    {
      return ::Spacy::makeHilbertSpace( KaskadeParabolic::VectorCreator<Description>(descriptions, spaces) , l2Product<Description>{} );
    }

  }
  /** @} */
}
