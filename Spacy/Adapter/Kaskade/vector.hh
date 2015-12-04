// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_KASKADE_VECTOR_SPACE_ELEMENT_HH
#define SPACY_ADAPTER_KASKADE_VECTOR_SPACE_ELEMENT_HH

#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/Util/Base/addArithmeticOperators.hh"
#include "Spacy/Util/Base/vectorBase.hh"
#include "Spacy/vectorSpace.hh"

#include "util.hh"

#include "io/vtk.hh"

namespace Spacy
{  
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace Kaskade
  {
    /// \cond
    template <class> class VectorCreator;
    /// \endcond

    /**
     * @ingroup KaskadeGroup VectorSapceGroup
     * @brief Coefficient vector implementation for %Kaskade 7 (single space).
     * @tparam Description %Kaskade::VariableSetDescription
     */
    template <class Description>
    class Vector :
        public VectorBase ,
        public AddArithmeticOperators< Vector<Description> >
    {
      using VectorImpl = typename Description::template CoefficientVectorRepresentation<>::type;
      using Variable = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Variables,0>::type> >;
      using Space = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Spaces,Variable::spaceIndex>::type> >;

    public:
      /**
       * @brief Construct vector \f$x\f$ from underlying vector space.
       * @param space underlying vector space
       */
      Vector(const VectorSpace& space)
        : VectorBase(space),
          description_( std::make_shared<Description>( creator< VectorCreator<Description> >(space).get()) ),
          v_( Description::template CoefficientVectorRepresentation<>::init( description_->spaces ))
      {}

      /// Assign from coefficient vector of %Kaskade 7.
      Vector& operator=(const typename Description::template CoefficientVectorRepresentation<>::type& v)
      {
        v_ = v;
        return *this;
      }

      /// Access coefficient vector.
      VectorImpl& get()
      {
        return v_;
      }

      /// Access coefficient vector.
      const VectorImpl& get() const
      {
        return v_;
      }


      /**
       * @brief Apply as dual element.
       * @param y primal vector
       * @return \f$x(y)\f$
       */
      Real operator()(const Vector& y) const
      {
        return get() * y.get();
      }

      //      Vector& axpy(double a, const AbstractVector& y)
      //      {
      //        v_.axpy(a,castTo< Vector<Description> >(y).v_);
      //        return *this;
      //      }

      void toFile(const std::string& filename) const
      {
//        typename Description::VariableSet x(*description_);
//        copy(*this,x);
//        ::Kaskade::writeVTKFile(boost::fusion::at_c<0>(description_->spaces)->gridManager().grid().leafGridView(),
//                                x,
//                                filename,::Kaskade::IoOptions{},
//                                1);//at_c<0>(spaces_)->mapper().getOrder());
      }

    private:
      std::shared_ptr<Description> description_;
      VectorImpl v_;
    };
  }
}

#endif // SPACY_ADAPTER_KASKADE_VECTOR_SPACE_ELEMENT_HH
