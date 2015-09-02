#include <gtest/gtest.h>

#include "Spacy/norm.hh"
#include "Spacy/scalarProduct.hh"
#include "Spacy/Spaces/realSpace.hh"
#include "Spacy/Spaces/productSpace.hh"
#include "Spacy/Util/cast.hh"

TEST(ProductSpaceTest,PurePrimalElementTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } );

  auto x = R2.vector();
  const auto& x_ = cast_ref<ProductSpace::Vector>(x);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(x_.variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(x_.variable(1)) , 0. );
}

TEST(ProductSpaceTest,PureDualElementTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } , {} , {0,1} );

  auto x = R2.vector();
  const auto& x_ = cast_ref<ProductSpace::Vector>(x);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(x_.variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(x_.variable(1)) , 0. );
}

TEST(ProductSpaceTest,MixedElementTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } , {0} , {1} );

  auto x = R2.vector();
  const auto& x_ = cast_ref<ProductSpace::Vector>(x);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(x_.variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(x_.variable(1)) , 0. );
}

TEST(ProductSpaceTest,PurePrimalElementSumTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } , {0,1} );

  auto x = R2.vector();
  auto y = R2.vector();
  auto& x_ = cast_ref<ProductSpace::Vector>(x);
  auto& y_ = cast_ref<ProductSpace::Vector>(y);
  cast_ref<Real::Vector>(x_.variable(0)) = cast_ref<Real::Vector>(y_.variable(0)) = 1;
  cast_ref<Real::Vector>(y_.variable(1)) = 2;

  auto ax = primal(x);
  const auto& ax_ = cast_ref<ProductSpace::Vector>(ax);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(ax_.variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(ax_.variable(1)) , 0. );
  auto ay = primal(y);
  const auto& ay_ = cast_ref<ProductSpace::Vector>(ay);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(ay_.variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(ay_.variable(1)) , 2. );

  auto bx = dual(x);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(bx).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(bx).variable(1)) , 0. );
  auto by = dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(by).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(by).variable(1)) , 0. );

  auto z = x + y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = x + primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = primal(x) + y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = primal(x) + primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );

  z = x + dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 0. );
  z = y + dual(x);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = dual(x) + y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = dual(x) + dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 0. );

  x += y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 2. );
  x += primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 3. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 4. );
  x += dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 3. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 4. );

  primal(x) += y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 4. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 6. );
  dual(x) += y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 4. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 6. );
  primal(x) += primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 5. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 8. );
  primal(x) += dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 5. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 8. );
  dual(x) += primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 5. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 8. );
  dual(x) += dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 5. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 8. );
}

TEST(ProductSpaceTest,PureDualElementSumTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } , {} , {0,1} );

  auto x = R2.vector();
  auto y = R2.vector();
  auto& x_ = cast_ref<ProductSpace::Vector>(x);
  auto& y_ = cast_ref<ProductSpace::Vector>(y);
  cast_ref<Real::Vector>(x_.variable(0)) = cast_ref<Real::Vector>(y_.variable(0)) = 1;
  cast_ref<Real::Vector>(y_.variable(1)) = 2;

  auto ax = primal(x);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(ax).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(ax).variable(1)) , 0. );
  auto ay = primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(ay).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(ay).variable(1)) , 0. );

  auto bx = dual(x);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(bx).variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(bx).variable(1)) , 0. );

  auto by = dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(by).variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(by).variable(1)) , 2. );

  auto z = x + y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = x + primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 0. );
  z = primal(x) + y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = primal(x) + primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 0. );

  z = x + dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = dual(x) + y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = dual(x) + dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );

  x += y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 2. );
  x += primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 2. );
  x += dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 3. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 4. );

  primal(x) += y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 3. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 4. );
  dual(x) += y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 4. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 6. );
  primal(x) += primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 4. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 6. );
  primal(x) += dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 4. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 6. );
  dual(x) += primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 4. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 6. );
  dual(x) += dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 5. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 8. );
}

TEST(ProductSpaceTest,MixedElementSumTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } , {0} , {1} );

  auto x = R2.vector();
  auto y = R2.vector();
  auto& x_ = cast_ref<ProductSpace::Vector>(x);
  auto& y_ = cast_ref<ProductSpace::Vector>(y);
  cast_ref<Real::Vector>(x_.variable(0)) = cast_ref<Real::Vector>(y_.variable(0)) = 1;
  cast_ref<Real::Vector>(y_.variable(1)) = 2;

  auto ax = primal(x);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(ax).variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(ax).variable(1)) , 0. );
  auto ay = primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(ay).variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(ay).variable(1)) , 0. );
  auto bx = dual(x);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(bx).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(bx).variable(1)) , 0. );
  auto by = dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(by).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(by).variable(1)) , 2. );

  auto z = x + y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = x + primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 0. );
  z = primal(x) + y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = primal(x) + primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 0. );

  z = x + dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = dual(x) + y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );
  z = dual(x) + dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 2. );

  x += y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 2. );
  x += primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 3. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 2. );
  x += dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 3. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 4. );

  primal(x) += y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 4. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 4. );
  dual(x) += y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 4. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 6. );
  primal(x) += primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 5. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 6. );
  primal(x) += dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 5. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 6. );
  dual(x) += primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 5. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 6. );
  dual(x) += dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(0)) , 5. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(x).variable(1)) , 8. );
}

TEST(ProductSpaceTest,PurePrimalElementProductTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } , {0,1} );

  auto x = R2.vector();
  auto y = R2.vector();
  auto& x_ = cast_ref<ProductSpace::Vector>(x);
  auto& y_ = cast_ref<ProductSpace::Vector>(y);
  cast_ref<Real::Vector>(x_.variable(0)) = cast_ref<Real::Vector>(x_.variable(1)) = cast_ref<Real::Vector>(y_.variable(0)) = 1;
  cast_ref<Real::Vector>(y_.variable(1)) = 3;

  EXPECT_DOUBLE_EQ( x*y , 4. );
  EXPECT_DOUBLE_EQ( primal(x)*y , 4.);
  EXPECT_DOUBLE_EQ( x*primal(y) , 4.);
  EXPECT_DOUBLE_EQ( primal(x)*primal(y) , 4.);
  EXPECT_DOUBLE_EQ( dual(x)*y , 0.);
  EXPECT_DOUBLE_EQ( x*dual(y) , 0.);
  EXPECT_DOUBLE_EQ( dual(x)*dual(y) , 0.);
}

TEST(ProductSpaceTest,PureDualElementProductTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } , {} , {0,1} );

  auto x = R2.vector();
  auto y = R2.vector();
  auto& x_ = cast_ref<ProductSpace::Vector>(x);
  auto& y_ = cast_ref<ProductSpace::Vector>(y);
  cast_ref<Real::Vector>(x_.variable(0)) = cast_ref<Real::Vector>(x_.variable(1)) = cast_ref<Real::Vector>(y_.variable(0)) = 1;
  cast_ref<Real::Vector>(y_.variable(1)) = 3;

  EXPECT_DOUBLE_EQ( x*y , 4. );
  EXPECT_DOUBLE_EQ( primal(x)*y , 0.);
  EXPECT_DOUBLE_EQ( x*primal(y) , 0.);
  EXPECT_DOUBLE_EQ( primal(x)*primal(y) , 0.);
  EXPECT_DOUBLE_EQ( dual(x)*y , 4.);
  EXPECT_DOUBLE_EQ( x*dual(y) , 4.);
  EXPECT_DOUBLE_EQ( dual(x)*dual(y) , 4.);
}

TEST(ProductSpaceTest,MixedElementProductTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } , {0} , {1} );

  auto x = R2.vector();
  auto y = R2.vector();
  auto& x_ = cast_ref<ProductSpace::Vector>(x);
  auto& y_ = cast_ref<ProductSpace::Vector>(y);
  cast_ref<Real::Vector>(x_.variable(0)) = cast_ref<Real::Vector>(x_.variable(1)) = cast_ref<Real::Vector>(y_.variable(0)) = 1;
  cast_ref<Real::Vector>(y_.variable(1)) = 3;

  EXPECT_DOUBLE_EQ( x*y , 4. );
  EXPECT_DOUBLE_EQ( primal(x)*y , 1.);
  EXPECT_DOUBLE_EQ( x*primal(y) , 1.);
  EXPECT_DOUBLE_EQ( primal(x)*primal(y) , 1.);
  EXPECT_DOUBLE_EQ( dual(x)*y , 3.);
  EXPECT_DOUBLE_EQ( x*dual(y) , 3.);
  EXPECT_DOUBLE_EQ( dual(x)*dual(y) , 3.);
}

TEST(ProductSpaceTest,MixedElementArithmeticProductTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } , {0} , {1} );

  auto x = R2.vector();
  auto y = R2.vector();
  auto& x_ = cast_ref<ProductSpace::Vector>(x);
  auto& y_ = cast_ref<ProductSpace::Vector>(y);
  cast_ref<Real::Vector>(x_.variable(0)) = cast_ref<Real::Vector>(x_.variable(1)) = cast_ref<Real::Vector>(y_.variable(0)) = 1;
  cast_ref<Real::Vector>(y_.variable(1)) = 3;
  Vector z = 2*y;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 6. );

  z = 2*primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 2. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 0. );

  z = 2*dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 6. );

  primal(z) = 3*primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 3. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 6. );

  primal(z) = 3*dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 6. );

  dual(z) = 3*primal(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 0. );

  dual(z) = 3*dual(y);
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(0)) , 0. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(cast_ref<ProductSpace::Vector>(z).variable(1)) , 9. );
}

TEST(ProductSpaceTest,ScalarProductTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } , {0} , {1} );

  auto x = R2.vector();
  auto y = R2.vector();
  auto& x_ = cast_ref<ProductSpace::Vector>(x);
  auto& y_ = cast_ref<ProductSpace::Vector>(y);
  cast_ref<Real::Vector>(x_.variable(0)) = cast_ref<Real::Vector>(y_.variable(0)) = 1;
  cast_ref<Real::Vector>(y_.variable(1)) = 2;
  EXPECT_DOUBLE_EQ( x*y, 1. );
  EXPECT_DOUBLE_EQ( x*y, R2.scalarProduct()(x,y) );
}

TEST(ProductSpaceTest,NormTest)
{
  using namespace Spacy;
  auto R = std::make_shared<VectorSpace>( Real::makeHilbertSpace() );
  auto R2 = ProductSpace::makeHilbertSpace( { R , R } , {0} , {1} );

  auto x = R2.vector();
  auto y = R2.vector();
  auto& x_ = cast_ref<ProductSpace::Vector>(x);
  auto& y_ = cast_ref<ProductSpace::Vector>(y);
  cast_ref<Real::Vector>(x_.variable(0)) = cast_ref<Real::Vector>(y_.variable(0)) = 3;
  cast_ref<Real::Vector>(y_.variable(1)) = 4;
  EXPECT_DOUBLE_EQ( R2.norm()(x) , 3. );
  EXPECT_DOUBLE_EQ( R2.norm()(y) , 5. );
}

