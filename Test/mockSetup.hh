#ifndef SPACY_TEST_MOCK_SETUP_HH
#define SPACY_TEST_MOCK_SETUP_HH

#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>
#include <utility>

#include "Spacy/vectorSpace.hh"
#include "Spacy/Spaces/realSpace.hh"
#include "Spacy/Spaces/productSpace.hh"
#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Exceptions/incompatibleSpaceException.hh"

#include "Test/Mock/vector.hh"
#include "Test/Mock/vectorCreator.hh"
#include "Test/Mock/scalarProduct.hh"
#include "Test/Mock/norm.hh"


template <class Space, class Spaces, unsigned... indices>
inline auto makeTuple(Space&& space, const Spaces& spaces, std::integer_sequence<unsigned,indices...>)
{
  return std::make_tuple(std::forward<Space>(space), (spaces[indices]->index())...);
}

inline constexpr auto numberOfVariables()
{
  return 2u;
}

inline auto mockValue(const Spacy::Vector& x)
{
  return value(Spacy::cast_ref<Mock::Vector>(x));
}

inline auto createMockBanachSpace(bool defaultIndex=false)
{
  using namespace Spacy;
  return VectorSpace( Mock::VectorCreator() , Mock::Norm() ,  defaultIndex );
}

inline auto createMockHilbertSpace()
{
  return Spacy::makeHilbertSpace( Mock::VectorCreator() , Mock::ScalarProduct() );
}


inline auto firstGlobalId(){ return 2u; }
inline auto secondGlobalId(){ return 1u; }
inline auto thirdGlobalId(){ return 5u; }
inline auto notGlobalId(){ return 0u; }


inline auto createGlobalIds()
{
  std::vector<unsigned> globalIds = { firstGlobalId() , secondGlobalId() };
  return globalIds;
}


inline auto createProductSpaceCreatorWithSpaceIndices()
{
  using namespace Spacy;
  std::vector< std::shared_ptr<VectorSpace> > spaces;
  for(auto i=0u; i<numberOfVariables(); ++i)
    spaces.push_back( std::make_shared<VectorSpace>( createMockBanachSpace() ) );
  auto creator = ProductSpace::VectorCreator(spaces);
  return makeTuple( creator , spaces , std::make_integer_sequence<unsigned,numberOfVariables()>() );
}

inline auto createProductSpaceCreatorWithSpaceIndicesAndMap()
{
  using namespace Spacy;
  std::vector< std::shared_ptr<VectorSpace> > spaces;
  for(auto i=0u; i<numberOfVariables(); ++i)
    spaces.push_back( std::make_shared<VectorSpace>( createMockBanachSpace() ) );
  auto creator = ProductSpace::VectorCreator(spaces,createGlobalIds());
  return makeTuple( creator , spaces , std::make_integer_sequence<unsigned,numberOfVariables()>() );
}

inline auto makeProductHilbertSpace()
{
  using namespace Spacy;
  std::vector< std::shared_ptr<VectorSpace> > spaces;
  for(auto i=0u; i<numberOfVariables(); ++i)
    spaces.push_back( std::make_shared<VectorSpace>( createMockHilbertSpace() ) );
  return makeTuple( ProductSpace::makeHilbertSpace(spaces) , spaces , std::make_integer_sequence<unsigned,numberOfVariables()>() );
}

inline auto makeProductHilbertSpaceWithMap()
{
  using namespace Spacy;
  std::vector< std::shared_ptr<VectorSpace> > spaces;
  for(auto i=0u; i<numberOfVariables(); ++i)
    spaces.push_back( std::make_shared<VectorSpace>( createMockHilbertSpace() ) );
  return makeTuple( ProductSpace::makeHilbertSpace(spaces,createGlobalIds()) , spaces , std::make_integer_sequence<unsigned,numberOfVariables()>() );
}

inline auto makePrimalDualProductSpaceCreatorWithSpaceIndices()
{
  using namespace Spacy;

  auto primalSubSpace = std::make_shared<VectorSpace>(std::get<0>(makeProductHilbertSpaceWithMap()));
  auto dualSubSpace = std::make_shared<VectorSpace>( ProductSpace::makeHilbertSpace({std::make_shared<VectorSpace>(createMockHilbertSpace())},{thirdGlobalId()}) );
  auto creator = ProductSpace::VectorCreator( { primalSubSpace, dualSubSpace } );
  return std::make_tuple( creator ,
                          Spacy::creator<ProductSpace::VectorCreator>(*primalSubSpace).subSpace(0).index() ,
                          Spacy::creator<ProductSpace::VectorCreator>(*primalSubSpace).subSpace(1).index()  ,
                          Spacy::creator<ProductSpace::VectorCreator>(*dualSubSpace).subSpace(0).index() );
}

inline auto makePrimalDualSpaceCreatorWithSpaceIndices()
{
  using namespace Spacy;

  auto primalSubSpace = std::make_shared<VectorSpace>(createMockHilbertSpace());
  auto dualSubSpace = std::make_shared<VectorSpace>(createMockHilbertSpace());
  auto creator = ProductSpace::VectorCreator( { primalSubSpace, dualSubSpace } );
  return std::make_tuple( creator ,
                          primalSubSpace->index(),
                          dualSubSpace->index() );
}

inline auto makePrimalDualProductHilbertSpace()
{
  using namespace Spacy;

  auto primalSubSpace = std::make_shared<VectorSpace>(std::get<0>(makeProductHilbertSpaceWithMap()));
  auto dualSubSpace = std::make_shared<VectorSpace>( ProductSpace::makeHilbertSpace({std::make_shared<VectorSpace>(createMockHilbertSpace())},{thirdGlobalId()}) );

  return ProductSpace::makeHilbertSpace( { primalSubSpace , dualSubSpace } );
}

inline const auto& valueOfComponent(const Spacy::Vector& v, unsigned i)
{
  using namespace Spacy;
  return value(cast_ref<Mock::Vector>(cast_ref<ProductSpace::Vector>(v).component(i)));
}

inline auto& valueOfComponent(Spacy::Vector& v, unsigned i)
{
  using namespace Spacy;
  return value(cast_ref<Mock::Vector>(cast_ref<ProductSpace::Vector>(v).component(i)));
}


inline auto createFirstTestVector(const Spacy::VectorSpace& V)
{
  auto v = V.zeroVector();
  valueOfComponent(v,0) = 1;
  valueOfComponent(v,1) = 2;
  return v;
}

inline auto createSecondTestVector(const Spacy::VectorSpace& V)
{
  auto v = V.zeroVector();
  valueOfComponent(v,0) = 1;
  valueOfComponent(v,1) = 3;
  return v;
}

#endif // SPACY_TEST_MOCK_SETUP_HH
