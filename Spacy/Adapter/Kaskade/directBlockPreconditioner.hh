

// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_KASKADE_DIRECTBLOCKPRECONDITIONER_HH
#define SPACY_KASKADE_DIRECTBLOCKPRECONDITIONER_HH

#include "fem/variables.hh"

#include "Spacy/linearSolver.hh"
#include "Spacy/operator.hh"
#include "Spacy/c2Functional.hh"
#include "Spacy/vector.hh"
#include "Spacy/Spaces/ProductSpace/vector.hh"
#include "Spacy/Util/Base/operatorBase.hh"
#include "Spacy/Adapter/Kaskade/util.hh"
#include <Spacy/zeroVectorCreator.hh>

#include <iostream>

namespace Spacy
{
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace Kaskade
  {
    /**
     * @brief A Preconditioner that uses a direct solver
     *
     */
    template<class FunctionalDefinition> 
    class DirectBlockPreconditioner
        : public OperatorBase
    {
    public:
    
       using Description = typename FunctionalDefinition::AnsatzVars;
       using SpacyFunctional = typename ::Spacy::Kaskade::C2Functional<FunctionalDefinition>;
       using AVD = typename SpacyFunctional::VariableSetDescription;
       using VariableSet = typename Description::VariableSet;
       using Spaces = typename AVD::Spaces;

       using KaskadeOperator = typename SpacyFunctional::KaskadeOperator;
       using Domain = typename AVD::template CoefficientVectorRepresentation<>::type;
       using Range = typename AVD::template CoefficientVectorRepresentation<>::type;
       template <class X, class Y>
          using M = ::Kaskade::MatrixRepresentedOperator<MatrixAsTriplet<double>,X,Y>;

       
          using VYSetDescription = Detail::ExtractDescription_t<AVD,0>;
          using VUSetDescription = Detail::ExtractDescription_t<AVD,1>;
          using VPSetDescription = Detail::ExtractDescription_t<AVD,2>;
          using DomainY = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
          using DomainU = typename VUSetDescription::template CoefficientVectorRepresentation<>::type;
          using DomainP = typename VPSetDescription::template CoefficientVectorRepresentation<>::type;
          using Atype = M<DomainY,DomainP>;
          using ATtype = M<DomainP,DomainY>;
          using Btype = M<DomainU,DomainP>;
          using BTtype = M<DomainP,DomainU>;
          using Mutype = M<DomainU,DomainU>;
       
      /**
       * @brief Constructor.
       * @param domain domain space
       * @param range range space
       */
      DirectBlockPreconditioner(const KaskadeOperator& K,
                          /*  const ::Spacy::Vector& origin, */ 
                            const Description& desc,
                            const VectorSpace& domain,
                            const VectorSpace& range) : 
                            OperatorBase(domain,range), 
                            desc_(desc),
                            spaces_( ::Spacy::Kaskade::extractSpaces<AVD>(domain) )
                       /*     origin_(origin)          */                  
                            {			
								blockbounds.push_back(0);
								{
								typename Description::VariableSet originK(desc);
// 								copy(origin_,originK);
								for(int i=0; i<originK.descriptions.noOfVariables;i++)
								{
									blockbounds.push_back(originK.descriptions.degreesOfFreedom(i,i+1)+blockbounds[i]);
								}
							    }        

								//~ ATtype AT;
								//~ AT=getBlock<ATtype>(K,0,2);
								//~ AT.get_non_const().setStartToZero();
								//~ solAT=std::make_shared< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<DomainP,DomainY> > >
                                //~ ( ::Kaskade::directInverseOperator(AT, DirectType::UMFPACK3264, MatrixProperties::GENERAL) );
                                { 
								std::cout << "  Factorization .." << std::flush;	
								Atype A;
								A=getBlock<Atype>(K,2,0);
								A.get_non_const().setStartToZero();
								solA =
								std::make_shared< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<DomainY,DomainP> > >
                                ( ::Kaskade::directInverseOperator(A, DirectType::UMFPACK3264, MatrixProperties::GENERAL) );
                                std::cout << "." << std::endl;
							    }

								Mutype Mu;
								Mu=getBlockDiag<Mutype>(K,1,1);		
								Mu.get_non_const().setStartToZero();
					

								solMu=std::make_shared< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<DomainU,DomainU> > >
                                ( ::Kaskade::directInverseOperator(Mu, DirectType::UMFPACK3264, MatrixProperties::GENERAL) );

								B=getBlock<Btype>(K,2,1);
								B.get_non_const().setStartToZero();

							}

      /**
       * @brief Apply preconditioner \f$P\f$.
       * @param x argument
       * @return \f$P(x)\f$
       */
      ::Spacy::Vector operator()(const ::Spacy::Vector& b) const { 

		using namespace boost::fusion;

        Domain b_(AVD::template CoefficientVectorRepresentation<>::init(spaces_) );
        Domain x_(AVD::template CoefficientVectorRepresentation<>::init(spaces_) );


        copyToCoefficientVector<AVD>(b,b_);


        DomainY b0_(at_c<0>(b_.data));           
        DomainU b1_(at_c<1>(b_.data));
        DomainP b2_(at_c<2>(b_.data));

        
        DomainY xy_(at_c<0>(x_.data));
        DomainU xu_(at_c<1>(x_.data));
        DomainP xp_(at_c<2>(x_.data));
        
        //solAT->apply(b0_,xp_);
        
        std::vector<double> p(xp_.dim());
        std::vector<double> b0(b0_.dim());
        
        b0_.write(b0.begin());
        
        // Solve A^T p=b0
        solA->op.solver->solve(b0,p,true);   // true means transposed 
        //solAT->apply(b0_,xp_);
        xp_.read(p.begin());
        
        
        B.applyscaleaddTransposed(-1.0,xp_,b1_); // b1_ -> b1_ +(-1.0)*(-B^T xp_)        
        
        // Solve M_u xu_ = b1_
        solMu->apply(b1_,xu_);
        
        
        B.applyscaleadd(-1.0,xu_,b2_); // b2_ -> b2_ +(-1.0)*(-B xu_)
        
        // Solve Axy_ = b2_
        solA->apply(b2_,xy_);
        
        at_c<0>(x_.data)=at_c<0>(xy_.data);
        at_c<1>(x_.data)=at_c<0>(xu_.data);
        at_c<2>(x_.data)=at_c<0>(xp_.data);

        //~ b2_ *= 0.0;
        //~ 
        //~ A.applyscaleadd(1.0,xy_,b2_);
        //~ B.applyscaleadd(1.0,xu_,b2_);
//~ 
//~ 
        //~ DomainY b0__(at_c<0>(b_.data));           
        //~ DomainU b1__(at_c<1>(b_.data));
        //~ DomainP b2__(at_c<2>(b_.data));
//~ 
        //~ std::cout << "Scalar products:" << xy_*b0__ << " " << xu_*b1__ << " " << xp_*b2__ << std::endl;        
        //~ std::cout << "U Norms:" << xy_.two_norm() << " " << xu_.two_norm() << " " << xp_.two_norm() << std::endl;        
        //~ std::cout << "R Norms:" << b0__.two_norm() << " " << b1__.two_norm() << " " << b2__.two_norm() << std::endl;        

//         auto x  = return space.creator()( &space )
        auto x = zero( domain() );
        
        copyFromCoefficientVector<AVD>(x_,x);

       return x;
	 };

      
    private:
        const Description& desc_;
        Spaces spaces_;
//         const ::Spacy::Vector& origin_;
        std::vector<int> blockbounds = {};
        Btype B;
        mutable std::shared_ptr< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<DomainY,DomainP> > > solA = nullptr;
        //~ mutable std::shared_ptr< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<DomainP,DomainY> > > solAT = nullptr;
        mutable std::shared_ptr< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<DomainU,DomainU> > > solMu = nullptr;
 
        template<class Result>
        Result getBlock(const KaskadeOperator& K, unsigned int row, unsigned int col)
        {
			const MatrixAsTriplet<double>& A(K.get());
			Result B;
			for(int i=0; i<A.nnz();++i)
			{
				if(A.ridx[i]>=blockbounds[row] && A.ridx[i]<blockbounds[row+1] 
				&& A.cidx[i]>=blockbounds[col] && A.cidx[i]<blockbounds[col+1])
				  B.get_non_const().addEntry(A.ridx[i],A.cidx[i],A.data[i]);
			}
        return B;

		}
        template<class Result>
        Result getBlockDiag(const KaskadeOperator& K, unsigned int row, unsigned int col)
        {
			const MatrixAsTriplet<double>& A(K.get());
			Result B;
			for(int i=0; i<A.nnz();++i)
			{
				if(A.ridx[i]>=blockbounds[row] && A.ridx[i]<blockbounds[row+1] 
				&& A.cidx[i]>=blockbounds[col] && A.cidx[i]<blockbounds[col+1] && A.ridx[i]==A.cidx[i])
				  B.get_non_const().addEntry(A.ridx[i],A.cidx[i],A.data[i]);
			}
        return B;

		}
    };
  }
}

#endif // SPACY_KASKADE_DIRECTBLOCKPRECONDITIONER_HH
