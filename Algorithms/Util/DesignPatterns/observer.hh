#ifndef ALGORITHM_UTIL_DESIGN_PATTERNS_OBSERVER_HH
#define ALGORITHM_UTIL_DESIGN_PATTERNS_OBSERVER_HH

#include <vector>

namespace Algorithm
{
  namespace DesignPattern
  {
    namespace Observer
    {
      /// \cond
      class Subject;
      /// \endcond

      /**
       * @brief Abstract observer class for the observer design pattern (GOF, Ch. 5.7).
       */
      class Observer
      {
      public:
        virtual ~Observer() = default;

      protected:
        friend class Subject;
        /**
         * @brief Update observer when one of its subjects has changed.
         */
        virtual void update(Subject* changedSubjekt) = 0;
      };

      /**
       * @brief Subject base class for the observer design pattern (GOF, Ch. 5.7).
       */
      class Subject
      {
      public:
        virtual ~Subject() = default;

      protected:
        /**
         * @brief Attach observers.
         */
        virtual void attach(Observer* observer);

        /**
         * @brief Detach observers.
         */
        virtual void detach(Observer* observer);

        /**
         * @brief Notify attached observers.
         */
        virtual void notify();

      private:
        std::vector< Observer* > observers_;
      };
    }
  }
}

#endif // ALGORITHM_UTIL_DESIGN_PATTERNS_OBSERVER_HH

