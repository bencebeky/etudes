#include "median.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(FindMedianSortedArrays, Empty) {
  EXPECT_EQ(1.0, findMedianSortedArrays({}, {1}));
  EXPECT_EQ(1.5, findMedianSortedArrays({}, {1, 2}));
  EXPECT_EQ(2.0, findMedianSortedArrays({}, {1, 2, 3}));

  EXPECT_EQ(1.0, findMedianSortedArrays({1}, {}));
  EXPECT_EQ(1.5, findMedianSortedArrays({1, 2}, {}));
  EXPECT_EQ(2.0, findMedianSortedArrays({1, 2, 3}, {}));
}

TEST(FindMedianSortedArrays, OneLarge) {
  EXPECT_EQ(0.5, findMedianSortedArrays({0}, {1}));
  EXPECT_EQ(1.0, findMedianSortedArrays({0, 1}, {2}));
  EXPECT_EQ(1.5, findMedianSortedArrays({0, 1, 2}, {3}));
  EXPECT_EQ(2.0, findMedianSortedArrays({0, 1, 2, 3}, {4}));
  EXPECT_EQ(2.5, findMedianSortedArrays({0, 1, 2, 3, 4}, {5}));

  EXPECT_EQ(0.5, findMedianSortedArrays({1}, {0}));
  EXPECT_EQ(1.0, findMedianSortedArrays({2}, {0, 1}));
  EXPECT_EQ(1.5, findMedianSortedArrays({3}, {0, 1, 2}));
  EXPECT_EQ(2.0, findMedianSortedArrays({4}, {0, 1, 2, 3}));
  EXPECT_EQ(2.5, findMedianSortedArrays({5}, {0, 1, 2, 3, 4}));
}

TEST(FindMedianSortedArrays, OneSmall) {
  EXPECT_EQ(0.5, findMedianSortedArrays({1}, {0}));
  EXPECT_EQ(1.0, findMedianSortedArrays({1, 2}, {0}));
  EXPECT_EQ(1.5, findMedianSortedArrays({1, 2, 3}, {0}));
  EXPECT_EQ(2.0, findMedianSortedArrays({1, 2, 3, 4}, {0}));
  EXPECT_EQ(2.5, findMedianSortedArrays({1, 2, 3, 4, 5}, {0}));

  EXPECT_EQ(0.5, findMedianSortedArrays({0}, {1}));
  EXPECT_EQ(1.0, findMedianSortedArrays({0}, {1, 2}));
  EXPECT_EQ(1.5, findMedianSortedArrays({0}, {1, 2, 3}));
  EXPECT_EQ(2.0, findMedianSortedArrays({0}, {1, 2, 3, 4}));
  EXPECT_EQ(2.5, findMedianSortedArrays({0}, {1, 2, 3, 4, 5}));
}

TEST(FindMedianSortedArrays, OneInTheMiddle) {
  EXPECT_EQ(1.0, findMedianSortedArrays({1}, {1}));

  EXPECT_EQ(1.0, findMedianSortedArrays({1, 2}, {1}));
  EXPECT_EQ(2.0, findMedianSortedArrays({1, 2}, {2}));
  EXPECT_EQ(2.0, findMedianSortedArrays({1, 3}, {2}));
  EXPECT_EQ(2.0, findMedianSortedArrays({1, 2, 3}, {2}));

  EXPECT_EQ(1.0, findMedianSortedArrays({1}, {1, 2}));
  EXPECT_EQ(2.0, findMedianSortedArrays({2}, {1, 2}));
  EXPECT_EQ(2.0, findMedianSortedArrays({2}, {1, 3}));
  EXPECT_EQ(2.0, findMedianSortedArrays({2}, {1, 2, 3}));

  EXPECT_EQ(3.0, findMedianSortedArrays({1, 3, 4, 5}, {2}));
  EXPECT_EQ(3.5, findMedianSortedArrays({1, 3, 4, 5, 6}, {2}));
  EXPECT_EQ(4.0, findMedianSortedArrays({1, 3, 4, 5, 6, 7}, {2}));

  EXPECT_EQ(3.0, findMedianSortedArrays({2}, {1, 3, 4, 5}));
  EXPECT_EQ(3.5, findMedianSortedArrays({2}, {1, 3, 4, 5, 6}));
  EXPECT_EQ(4.0, findMedianSortedArrays({2}, {1, 3, 4, 5, 6, 7}));

  EXPECT_EQ(3.0, findMedianSortedArrays({1, 2, 3, 5}, {4}));
  EXPECT_EQ(3.5, findMedianSortedArrays({1, 2, 3, 4, 6}, {5}));
  EXPECT_EQ(4.0, findMedianSortedArrays({1, 2, 3, 4, 5, 7}, {6}));

  EXPECT_EQ(3.0, findMedianSortedArrays({4}, {1, 2, 3, 5}));
  EXPECT_EQ(3.5, findMedianSortedArrays({5}, {1, 2, 3, 4, 6}));
  EXPECT_EQ(4.0, findMedianSortedArrays({6}, {1, 2, 3, 4, 5, 7}));
}

TEST(FindMedianSortedArrays, MostlyDisjoint) {
  EXPECT_EQ(3.0, findMedianSortedArrays({1, 2, 3}, {4, 5}));
  // EXPECT_EQ(3.0, findMedianSortedArrays({1, 2}, {3, 4, 5}));

  // EXPECT_EQ(2.5, findMedianSortedArrays({1, 2}, {3, 4}));
  // EXPECT_EQ(3.5, findMedianSortedArrays({1, 2}, {3, 4, 5, 6}));
  // EXPECT_EQ(3.5, findMedianSortedArrays({1, 2, 3, 4}, {5, 6}));

  /*
    EXPECT_EQ(3.0, findMedianSortedArrays({1, 3}, {2, 3, 4}));
    EXPECT_EQ(3.0, findMedianSortedArrays({1, 3}, {2, 3, 4, 5}));
    EXPECT_EQ(3.5, findMedianSortedArrays({1, 3}, {1, 2, 4, 5}));
    EXPECT_EQ(4.5, findMedianSortedArrays({1, 3}, {2, 4, 5, 6}));

    EXPECT_EQ(2.0, findMedianSortedArrays({1, 2, 3}, {1, 4}));
    EXPECT_EQ(3.0, findMedianSortedArrays({1, 2, 3, 4}, {3, 5}));
    EXPECT_EQ(4.5, findMedianSortedArrays({1, 2, 5, 6}, {4, 6}));
    EXPECT_EQ(3.5, findMedianSortedArrays({1, 2, 3, 5}, {4, 6}));
    */
}

TEST(FindMedianSortedArrays, Interleaved) {
  EXPECT_EQ(2.5, findMedianSortedArrays({1, 4}, {2, 3}));
  EXPECT_EQ(2.5, findMedianSortedArrays({2, 3}, {1, 4}));
  EXPECT_EQ(2.5, findMedianSortedArrays({1, 3}, {2, 4}));
  EXPECT_EQ(2.5, findMedianSortedArrays({2, 4}, {1, 3}));

  EXPECT_EQ(3.0, findMedianSortedArrays({1, 4, 5}, {2, 3}));
  EXPECT_EQ(3.0, findMedianSortedArrays({2, 3}, {1, 4, 5}));
  EXPECT_EQ(3.0, findMedianSortedArrays({1, 3, 5}, {2, 4}));
  EXPECT_EQ(3.0, findMedianSortedArrays({2, 4}, {1, 3, 5}));
}
