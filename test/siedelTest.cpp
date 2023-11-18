#include <gtest/gtest.h>

#include "../siedel.private.h"

using namespace libMesh;

TEST(SegmentStructure, AccessTest) {
  // Arrange
  auto seg = SegmentStructure(4);
  auto firstId = seg.nextIndex();
  auto secondId = seg.nextIndex();
  seg[firstId].next = 1u;
  seg[secondId].prev = 1u;
  // Act
  auto actualId = seg.nextIndex();
  seg[secondId].prev = actualId;
  // Assert
  EXPECT_EQ(seg[0].prev, firstId);
  EXPECT_EQ(seg[0].next, secondId);
  EXPECT_EQ(seg[1].prev, actualId);
}

TEST(QueryStructure, AccessTest) {
  // Arrange
  auto qs = QueryStructure(4);
  auto firstId = qs.nextIndex();
  auto secondId = qs.nextIndex();
  qs[firstId].right = secondId;
  qs[secondId].left = secondId;
  // Act
  auto thirdId = qs.nextIndex();
  qs[secondId].left = thirdId;
  // Assert
  EXPECT_EQ(qs[firstId].left, firstId);
  EXPECT_EQ(qs[firstId].right, secondId);
  EXPECT_EQ(qs[secondId].left, thirdId);
}

TEST(TrapezoidStructure, AccessTest) {
  // Arrange
  auto tr = TrapezoidStructure(4);
  auto firstId = tr.nextIndex();
  auto secondId = tr.nextIndex();
  tr[firstId].up0 = secondId;
  tr[secondId].down0 = secondId;
  // Act
  auto thirdId = tr.nextIndex();
  tr[secondId].down0 = thirdId;
  // Assert
  EXPECT_EQ(tr[firstId].down0, firstId);
  EXPECT_EQ(tr[firstId].up0, secondId);
  EXPECT_EQ(tr[secondId].down0, thirdId);
}
