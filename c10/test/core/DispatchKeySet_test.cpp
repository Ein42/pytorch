#include <gtest/gtest.h>

#include <c10/core/DispatchKeySet.h>

using namespace c10;

TEST(DispatchKeySet, Empty) {
  DispatchKeySet empty_set;
  for (uint8_t i = 1; i < static_cast<uint8_t>(DispatchKey::NumDispatchKeys); i++) {
    auto tid = static_cast<DispatchKey>(i);
    ASSERT_FALSE(empty_set.has(tid));
  }
  ASSERT_TRUE(empty_set.empty());
  DispatchKeySet empty_set2;
  ASSERT_TRUE(empty_set == empty_set2);
  ASSERT_EQ(empty_set.highestPriorityTypeId(), DispatchKey::Undefined);
}

TEST(DispatchKeySet, Singleton) {
  for (uint8_t i = 1; i < static_cast<uint8_t>(DispatchKey::NumDispatchKeys); i++) {
    auto tid = static_cast<DispatchKey>(i);
    DispatchKeySet sing(tid);
    ASSERT_EQ(sing, sing);
    ASSERT_EQ(sing, DispatchKeySet().add(tid));
    ASSERT_EQ(sing, sing.add(tid));
    ASSERT_EQ(sing, sing | sing);
    ASSERT_FALSE(sing.empty());
    ASSERT_TRUE(sing.has(tid));
    ASSERT_EQ(sing.highestPriorityTypeId(), tid);
    ASSERT_EQ(sing.remove(tid), DispatchKeySet());
  }
}

TEST(DispatchKeySet, Doubleton) {
  for (uint8_t i = 1; i < static_cast<uint8_t>(DispatchKey::NumDispatchKeys); i++) {
    for (uint8_t j = i + 1; j < static_cast<uint8_t>(DispatchKey::NumDispatchKeys); j++) {
      ASSERT_LT(i, j);
      auto tid1 = static_cast<DispatchKey>(i);
      auto tid2 = static_cast<DispatchKey>(j);
      auto doub = DispatchKeySet(tid1).add(tid2);
      ASSERT_EQ(doub, DispatchKeySet(tid1) | DispatchKeySet(tid2));
      ASSERT_TRUE(doub.has(tid1));
      ASSERT_TRUE(doub.has(tid2));
      ASSERT_EQ(doub.highestPriorityTypeId(), tid2);  // relies on i < j
    }
  }
}

TEST(DispatchKeySet, Full) {
  DispatchKeySet full(DispatchKeySet::FULL);
  for (uint8_t i = 1; i < static_cast<uint8_t>(DispatchKey::NumDispatchKeys); i++) {
    auto tid = static_cast<DispatchKey>(i);
    ASSERT_TRUE(full.has(tid));
  }
}

TEST(DispatchKeySet, IteratorBasicOps) {
  DispatchKeySet empty_set;
  DispatchKeySet full_set(DispatchKeySet::FULL);
  DispatchKeySet mutated_set = empty_set.add(static_cast<DispatchKey>(1));
  auto it_begin = empty_set.begin();
  auto it_end = empty_set.end();

  // Dereference Op
  ASSERT_EQ(*it_begin, false);
  ASSERT_TRUE(*full_set.begin());
  ASSERT_TRUE(*mutated_set.begin());

  // Comparison Ops
  ASSERT_EQ(it_begin, empty_set.begin());
  ASSERT_EQ(it_end, empty_set.end());
  ASSERT_FALSE(it_begin == it_end);
  ASSERT_TRUE(it_begin != it_end);

  // Increment Ops
  ASSERT_TRUE(empty_set.begin() == empty_set.begin()++);
  ASSERT_TRUE(empty_set.begin() != ++empty_set.begin());
}

TEST(DispatchKeySet, IteratorBasic) {
  DispatchKeySet empty_set;
  uint8_t i = 0;

  for (auto it = empty_set.begin(); it != empty_set.end(); ++it) {
    i++;
  }

  ASSERT_EQ(i, static_cast<uint8_t>(DispatchKey::NumDispatchKeys) - 1);
}

TEST(DispatchKeySet, IteratorRangeBased) {
  DispatchKeySet empty_set;
  uint8_t i = 0;

  for (bool dispatch_key: empty_set) {
    i++;
  }

  ASSERT_EQ(i, static_cast<uint8_t>(DispatchKey::NumDispatchKeys) - 1);
}
