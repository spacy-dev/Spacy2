// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include "linalg/direct.hh"
#include <Spacy/zeroVectorCreator.hh>
#include "Spacy/vector.hh"
#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Base/operatorBase.hh"
#include "util.hh"
#include "vectorSpace.hh"

namespace Spacy
{
  namespace Kaskade
  {
      
    /**
     * @ingroup KaskadeGroup
     * @brief Wrapper class to call apply-routines from different Kaskade preconditioners the same way (is to be unified in Kaskade)
     * @param Prec  %Kaskade Preconditioner (i.e., %Kaskade::AdditiveMultiGrid < > )
     * @param range range space of the preconditioner
     * @param domain domain space of the preconditioner
     */
      
 template  <  class Range,  class Domain, class Prec>
 class Apply
 {
     
 public:
     static void wrap(Prec & prec, Range & y, Domain & x) 
     {
         prec.apply(y,x);
     }
     
 };
  /**
     * @ingroup KaskadeGroup
     * @brief Wrapper class to call apply-routines from different Kaskade preconditioners the same way (is to be unified in Kaskade)
     * @param Prec  %Kaskade Preconditioner (i.e., %Kaskade::AdditiveMultiGrid < > )
     * @param range range space of the preconditioner
     * @param domain domain space of the preconditioner
     */
 template <class Range,  class Domain,  class T,  class U,  class V>
 class Apply< Range,  Domain, ::Kaskade::AdditiveMultiGrid<T, U, V> >
 {
 public:
     static void wrap(::Kaskade::AdditiveMultiGrid<T, U, V> & prec, Range & y, Domain & x) 
     {
         prec.apply( ::Kaskade::component<0>(y),  ::Kaskade::component<0>(x));
     }
     
 };
    /**
     * @ingroup KaskadeGroup
     * @brief Preconditioner interface for %Kaskade 7.
     */
    template < class AnsatzVariableDescription, class TestVariableDescription, class Prec>
    class Preconditioner :
        public OperatorBase
    {
      using Spaces = typename AnsatzVariableDescription::Spaces;
      using Domain = typename AnsatzVariableDescription::template CoefficientVectorRepresentation<>::type;
      using Range  = typename TestVariableDescription::template CoefficientVectorRepresentation<>::type;
    public:
      Preconditioner() = delete;
      /**
       * @brief Constructor.
       * @param spaces boost fusion forward sequence of space pointers required to initialize temporary storage
       * @param domain domain space of the solver
       * @param range range space of the solver
       */
      Preconditioner( const VectorSpace& domain, const VectorSpace& range, Prec& prec)
        : OperatorBase(domain,range),
          prec_(prec),
          spaces_( extractSpaces<AnsatzVariableDescription>(domain) )
      {}

      /// Compute \f$A^{-1}x\f$.
      ::Spacy::Vector operator()(const ::Spacy::Vector& x) const
      {
        
        // use range and domain defined by the preconditioner
        Range y_(TestVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        Domain x_(AnsatzVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        copyToCoefficientVector<AnsatzVariableDescription>(x,x_);

        Apply<Range, Domain, Prec>::wrap(prec_, y_, x_);

        auto y = ::Spacy::zero(range());
        copyFromCoefficientVector<TestVariableDescription>(y_,y);

        return y;
      }

    private:
      Prec&  prec_;
      Spaces spaces_;
      
    };

    /**
     * @brief Convenient generation of a Spacy preconditioner from a Kaskade preconditioner
     * @param A %Kaskade operator (i.e., AssembledGalerkinOperator or MatrixRepresentedOperator)
     * @param spaces boost fusion forward sequence of space pointers required to initialize temporary storage
     * @param domain domain space of the solver
     * @param range range space of the solver
     * @param Prec Kaskade preconditioner (e.g. Kaskade::AdditiveMultiGrid < > )
     * @return Preconditioner<AnsatzVariableSetDescription, TestVariableSetDescription, Preconditioner  >( domain , range , preconditioner )
     */
    template <class AnsatzVariableSetDescription, class TestVariableSetDescription, class Prec>
    auto makePreconditioner(const VectorSpace& domain, const VectorSpace& range, Prec&  prec)
    {
      return Preconditioner<AnsatzVariableSetDescription,TestVariableSetDescription, Prec>
          ( domain, range, prec);
    }

  }
}
