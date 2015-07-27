#ifndef ALGORITHM_UTILCREATE_HH
#define ALGORITHM_UTILCREATE_HH

#include <utility>

namespace Algorithm
{
  /**
   * @brief Convenient generation of Object from implementation arguments.
   * @return Object(std::make_shared<Implementation>(std::forward<Args>(args)...))
   */
  template <class Object, class Implementation, class... Args>
  auto createFromSharedImpl(Args&&... args)
  {
    return Object( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }

  /**
   * @brief Convenient generation of Object from implementation arguments.
   * @return Object(std::make_unique<Implementation>(std::forward<Args>(args)...))
   */
  template <class Object, class Implementation, class... Args>
  auto createFromUniqueImpl(Args&&... args)
  {
    return Object( std::make_unique<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_UTILCREATE_HH

