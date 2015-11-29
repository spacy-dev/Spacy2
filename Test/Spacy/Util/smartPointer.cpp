// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "Spacy/Util/smartPointer.hh"

using namespace Spacy;

namespace
{
  struct Base
  {
    virtual ~Base(){}

    virtual int foo() { return 1; }

    virtual std::unique_ptr<Base> clone() const { return std::make_unique<Base>(); }
  };

  struct Derived : Base
  {
    Derived() = default;

    Derived(int i, int j)
      : value(i+j)
    {}

    int foo() override { return value; }

    std::unique_ptr<Base> clone() const override { return std::make_unique<Derived>(value,0); }

    int value = 2;
  };
}

TEST(CopyViaClonePtr,Empty)
{
  CopyViaClonePtr<Base> ptr;

  bool empty = !ptr;
  ASSERT_TRUE(empty);
  ASSERT_TRUE( ptr == nullptr );
}

TEST(CopyViaClonePtr,ExplicitMoveFromDerivedUniquePtr)
{
  CopyViaClonePtr<Base> ptr ( std::make_unique<Derived>(1,3) );

  bool empty = !ptr;
  ASSERT_FALSE(empty);
  ASSERT_TRUE( ptr != nullptr );
  ASSERT_EQ( ptr->foo() , 4 );
}



TEST(CopyViaClonePtr,ExplicitMoveFromDerived)
{
  CopyViaClonePtr<Derived> ptr0 ( std::make_unique<Derived>(1,3) );
  CopyViaClonePtr<Base> ptr( std::move(ptr0) );

  bool empty = !ptr;
  ASSERT_FALSE(empty);
  ASSERT_TRUE( ptr != nullptr );
  ASSERT_EQ( ptr->foo() , 4 );
}

TEST(CopyViaClonePtr,ImplicitMoveFromDerivedUniquePtr)
{
  CopyViaClonePtr<Base> ptr = std::make_unique<Derived>(1,3);

  bool empty = !ptr;
  ASSERT_FALSE(empty);
  ASSERT_TRUE( ptr != nullptr );
  ASSERT_EQ( ptr->foo() , 4 );
}

TEST(CopyViaClonePtr,ImplicitMoveFromDerived)
{
  CopyViaClonePtr<Derived> ptr0( std::make_unique<Derived>(1,3) );
  CopyViaClonePtr<Base> ptr = std::move(ptr0);

  bool empty0 = !ptr0;
  bool empty = !ptr;
  ASSERT_TRUE(empty0);
  ASSERT_FALSE(empty);
  ASSERT_FALSE( ptr0 != nullptr );
  ASSERT_TRUE( ptr != nullptr );
  ASSERT_EQ( ptr->foo() , 4 );
}

TEST(CopyViaClonePtr,Copy)
{
  CopyViaClonePtr<Base> ptr = std::make_unique<Derived>(1,3);
  auto ptr2 = ptr;

  bool empty = !ptr;
  ASSERT_FALSE(empty);
  ASSERT_TRUE( ptr != nullptr );
  ASSERT_EQ( ptr->foo() , 4 );
  ASSERT_EQ( ptr2->foo() , 4 );
  ASSERT_FALSE( ptr == ptr2 );
  ASSERT_TRUE( ptr == ptr );
}

TEST(CopyViaClonePtr,Move)
{
  CopyViaClonePtr<Base> ptr = std::make_unique<Derived>(1,3);
  auto ptr2 = std::move(ptr);

  bool empty = !ptr;
  ASSERT_TRUE(empty);
  ASSERT_TRUE( ptr == nullptr );
  ASSERT_EQ( ptr2->foo() , 4 );
}

TEST(CopyViaClonePtr,MoveFromDerived)
{
  CopyViaClonePtr<Derived> ptr = std::make_unique<Derived>(1,3);
  auto ptr2 = std::move(ptr);

  bool empty = !ptr;
  ASSERT_TRUE(empty);
  ASSERT_TRUE( ptr == nullptr );
  ASSERT_EQ( ptr2->foo() , 4 );
}

TEST(CopyViaClonePtr,MoveAssignFromDerived)
{
  CopyViaClonePtr<Base> ptr;
  ptr = std::make_unique<Derived>(1,3);

  bool empty = !ptr;
  ASSERT_FALSE(empty);
  ASSERT_TRUE( ptr != nullptr );
  ASSERT_EQ( ptr->foo() , 4 );
}

TEST(CopyViaClonePtr,CopyEmpty)
{
  CopyViaClonePtr<Base> ptr2;
  CopyViaClonePtr<Base> ptr = ptr2;

  bool empty = !ptr;
  ASSERT_TRUE(empty);
  ASSERT_TRUE( ptr == nullptr );
  ASSERT_TRUE( ptr == ptr2 );
  ASSERT_TRUE( ptr == ptr );
}


TEST(CopyViaClonePtr,ConstructFromArguments)
{
  CopyViaClonePtr<Derived> ptr(1,2);
  ASSERT_EQ( ptr->foo() , 3 );
}

