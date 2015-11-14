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
//#include "Spacy/Spaces/PrimalDualProductSpace/vector.hh"
//#include "Spacy/Spaces/PrimalDualProductSpace/vectorSpace.hh"
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

inline auto createProductSpaceCreatorWithSpaceIndices()
{
  using namespace Spacy;
  std::vector< std::shared_ptr<VectorSpace> > spaces;
  for(auto i=0u; i<numberOfVariables(); ++i)
    spaces.push_back( std::make_shared<VectorSpace>( createMockBanachSpace() ) );
  auto creator = ProductSpace::VectorCreator(spaces);
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

//inline auto makePrimalDualProductSpaceCreatorWithSpaceIndices()
//{
//  using namespace Spacy;
//  auto V = std::make_shared<VectorSpace>(createMockBanachSpace());
//  auto W = std::make_shared<VectorSpace>(createMockBanachSpace());
//  auto creator = PrimalDualProductSpace::VectorCreator( V , W );
//  return std::make_tuple( creator , V->index() , W->index() );
//}

//inline auto makePrimalDualProductHilbertSpace()
//{
//  using namespace Spacy;
//  auto V = std::make_shared<VectorSpace>(createMockBanachSpace());
//  auto W = std::make_shared<VectorSpace>(createMockBanachSpace());
//  return std::make_tuple( PrimalDualProductSpace::makeHilbertSpace(V,W) , V->index() , W->index() );
//}

inline const auto& valueOfComponent(const Spacy::Vector& v, unsigned i)
{
  using namespace Spacy;
  return value(cast_ref<Mock::Vector>(cast_ref<ProductSpace::Vector>(v).variable(i)));
}

inline auto& valueOfComponent(Spacy::Vector& v, unsigned i)
{
  using namespace Spacy;
  return value(cast_ref<Mock::Vector>(cast_ref<ProductSpace::Vector>(v).variable(i)));
}


inline auto createFirstTestVector(const Spacy::VectorSpace& V)
{
  auto v = V.vector();
  valueOfComponent(v,0) = 1;
  valueOfComponent(v,1) = 2;
  return v;
}

inline auto createSecondTestVector(const Spacy::VectorSpace& V)
{
  auto v = V.vector();
  valueOfComponent(v,0) = 1;
  valueOfComponent(v,1) = 3;
  return v;
}

#endif // SPACY_TEST_MOCK_SETUP_HH
