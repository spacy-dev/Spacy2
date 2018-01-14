#pragma once

#include <type_traits>

#include <Spacy/Util/Mixins/Get.hh>



namespace Spacy
{
  /** @addtogroup KaskadeParabolicGroup @{ */
  namespace KaskadeParabolic
  {
    /// Creator forvector space elements for %Kaskade 7
    /// this function is used for each timestep
    template <class Description>
    class SubCreator :
        public Mixin::Get<Description>
    {
    public:
      /**
       * @ingroup VectorSpaceGroup
       * @brief Create from %Kaskade 7 function space.
       * @param space single %Kaskade 7 function space (no product space)
       */
      template <class... Args,
                class = std::enable_if_t<std::is_constructible<Description,Args...>::value> >
      SubCreator(Args&&... args)
        : Mixin::Get<Description>( std::forward<Args>(args)... )
      {}

    };

  }
  /** @} */
}

