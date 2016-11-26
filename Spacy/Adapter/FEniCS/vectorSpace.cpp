#include "vectorSpace.hh"

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Spaces/ProductSpace/vectorSpace.hh>
#include <Spacy/Spaces/ScalarSpace/real.hh>

#include "l2Product.hh"
#include "vector.hh"

namespace Spacy
{
    namespace FEniCS
    {
        VectorCreator::VectorCreator(std::shared_ptr<dolfin::FunctionSpace> space)
            : Mixin::Get< std::shared_ptr<dolfin::FunctionSpace> >(space)
        {}

        VectorCreator::VectorCreator(std::shared_ptr<dolfin::FunctionSpace> space, const std::unordered_map<std::size_t,std::size_t>& dofmap)
            : Mixin::Get< std::shared_ptr<dolfin::FunctionSpace> >(space),
              dofmap_(dofmap)
        {
            if(!dofmap_.empty())
                for(auto entry = begin(dofmap_); entry != end(dofmap_); ++entry)
                    inverse_dofmap_[entry->second] = entry->first;
        }

        std::size_t VectorCreator::dofmap(std::size_t i) const
        {
            if(dofmap_.empty()) return i;
            auto found = dofmap_.find(i);
            assert(found != end(dofmap_));
            return found->second;
        }

        std::size_t VectorCreator::inverseDofmap(std::size_t i) const
        {
            if(inverse_dofmap_.empty()) return i;
            auto found = inverse_dofmap_.find(i);
            assert(found != end(inverse_dofmap_));
            return found->second;
        }


        ::Spacy::Vector VectorCreator::operator()(const VectorSpace* space) const
        {
            return Vector{*space};
        }

        unsigned VectorCreator::size() const
        {
            return get()->dim();
        }

        VectorSpace makeHilbertSpace(std::shared_ptr<dolfin::FunctionSpace> space)
        {
            return ::Spacy::makeHilbertSpace( VectorCreator{space} , l2Product{} );
        }

        VectorSpace makeHilbertSpace(std::shared_ptr<dolfin::FunctionSpace> space, const std::unordered_map<std::size_t,std::size_t>& dofmap)
        {
            return ::Spacy::makeHilbertSpace( VectorCreator{space,dofmap} , l2Product{} );
        }

        VectorSpace makeHilbertSpace(std::shared_ptr<dolfin::FunctionSpace> space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds)
        {
            unsigned maxPrimalId = primalIds.empty() ? 0 : *std::max_element(begin(primalIds),end(primalIds));
            unsigned maxDualId   = dualIds.empty()   ? 0 : *std::max_element(begin(dualIds),end(dualIds));
            std::vector<std::shared_ptr< VectorSpace > > spaces( 1 + std::max(maxPrimalId, maxDualId) );

            for(auto i : primalIds)
            {
                std::unordered_map<std::size_t,std::size_t> dofmap;
                auto subSpace = (*space)[i]->collapse(dofmap);
                spaces[i] = std::make_shared<VectorSpace>( makeHilbertSpace(subSpace, std::move(dofmap)) );
            }
            for(auto i : dualIds)
            {
                std::unordered_map<std::size_t,std::size_t> dofmap;
                auto subSpace = (*space)[i]->collapse(dofmap);
                spaces[i] = std::make_shared<VectorSpace>( makeHilbertSpace(subSpace, std::move(dofmap)) );
            }

            if( dualIds.empty() )
                return ProductSpace::makeHilbertSpace(spaces, std::move(primalIds));

            return ProductSpace::makeHilbertSpace(spaces, std::move(primalIds), std::move(dualIds));
        }
    }
}
