// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_KASKADE_PRECONDITIONER_HH
#define SPACY_ADAPTER_KASKADE_PRECONDITIONER_HH

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
      
 template  <  class Range,  class Domain, class Prec>
 class Apply
 {
     
 public:
     static void wrap(Prec & prec, Range & y, Domain & x) 
     {
         prec.apply(y,x);
     }
     
     
     
 };
 
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
     * @brief Direct solver interface for %Kaskade 7.
     */
    template <class KaskadeOperator, class AnsatzVariableDescription, class TestVariableDescription, class Prec>
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
       * @param A %Kaskade operator (i.e., %AssembledGalerkinOperator or %MatrixRepresentedOperator)
       * @param spaces boost fusion forward sequence of space pointers required to initialize temporary storage
       * @param domain domain space of the solver
       * @param range range space of the solver
       * @param directSolver solver type (DirectType::MUMPS (default), DirectType::UMFPACK, DirectType::UMFPACK3264 or DirectType::SUPERLU)
       * @param property matrix property (MatrixProperties::GENERAL (default) or MatrixProperties::SYMMETRIC)
       */
      Preconditioner(const KaskadeOperator& A, const VectorSpace& domain, const VectorSpace& range, Prec& prec)
        : OperatorBase(domain,range),
          prec_(prec),
          A_(A),
          spaces_( extractSpaces<AnsatzVariableDescription>(domain) )
      {}

      /// Compute \f$A^{-1}x\f$.
      ::Spacy::Vector operator()(const ::Spacy::Vector& x) const
      {
        
        // use range and domain defined by the preconditioner
        Range y_(TestVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        Domain x_(AnsatzVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        copyToCoefficientVector<AnsatzVariableDescription>(x,x_);

//         static const bool check_type_equal = std::is_same<Prec, KaskadeOperator>::value;
        
        // Note: apply arguments in correct order !!!!!!!!!!!!
        
       
//         prec_.apply( ::Kaskade::component<0>(y_),  ::Kaskade::component<0>(x_));
//         else
//          prec_.apply(y_,x_);
        

        Apply<Range, Domain, Prec>::wrap(prec_, y_, x_);
//         auto y = ::Spacy
         auto y = ::Spacy::zero(range());
        copyFromCoefficientVector<TestVariableDescription>(y_,y);

        return y;
      }

    private:
      Prec&  prec_;
      const KaskadeOperator& A_;
      Spaces spaces_;
      
    };

    /**
     * @brief Convenient generation of direct solver for %Kaskade 7.
     * @param A %Kaskade operator (i.e., AssembledGalerkinOperator or MatrixRepresentedOperator)
     * @param spaces boost fusion forward sequence of space pointers required to initialize temporary storage
     * @param domain domain space of the solver
     * @param range range space of the solver
     * @param directSolver solver type (DirectType::MUMPS (default), DirectType::UMFPACK, DirectType::UMFPACK3264 or DirectType::SUPERLU)
     * @param property matrix property (MatrixProperties::GENERAL (default) or MatrixProperties::SYMMETRIC)
     * @return Preconditioner<KaskadeOperator,AnsatzVariableSetDescription,TestVariableSetDescription>( A , spaces , domain , range , directSolver , property )
     */
    template <class AnsatzVariableSetDescription, class TestVariableSetDescription, class KaskadeOperator, class Prec>
    auto makePreconditioner(const KaskadeOperator& A, const VectorSpace& domain, const VectorSpace& range, Prec&  prec)
    {
      return Preconditioner<KaskadeOperator,AnsatzVariableSetDescription,TestVariableSetDescription, Prec>
          (  A, domain, range, prec);
    }

  }
}

#endif // SPACY_ADAPTER_KASKADE_PRECONDITIONER_HH
