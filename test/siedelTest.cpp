#include <gtest/gtest.h>

#include "../siedel.private.h"

TEST(SegmentStructure, AccessTest) {
  // Arrange
  auto seg = SegmentStructure(4);
  seg[0u].next = 1u;
  seg[1u].prev = 1u;
  // Act
  seg[1u].prev = 2u;
  // Assert
  EXPECT_EQ(seg[0].prev, 0u);
  EXPECT_EQ(seg[0].next, 1u);
  EXPECT_EQ(seg[1].prev, 2u);
}

TEST(SegmentStructure, InitializationTest) {
  // Arrange
  auto seg = SegmentStructure(5);
  auto polygon = libMesh::PolyLine();
  polygon.add(libMesh::Point2(0, 0));
  polygon.add(libMesh::Point2(0, 1));
  polygon.add(libMesh::Point2(1, 1));
  polygon.add(libMesh::Point2(1, 0));
  unsigned int contours = 4;
  // Act
  auto actual = siedel::initializeSegments(seg, polygon, &contours, 1);
  // Assert
  EXPECT_EQ(actual, 4u);

  EXPECT_EQ(seg[0].next, 0u);
  EXPECT_EQ(seg[0].prev, 0u);
  EXPECT_EQ(seg[0].root0, 0u);
  EXPECT_EQ(seg[0].root1, 0u);
  EXPECT_FALSE(seg[0].isInserted);

  EXPECT_EQ(seg[0].begin.x, 0);
  EXPECT_EQ(seg[0].begin.y, 0);
  EXPECT_EQ(seg[0].end.x, 0);
  EXPECT_EQ(seg[0].end.y, 0);

  EXPECT_EQ(seg[1].next, 2u);
  EXPECT_EQ(seg[2].next, 3u);
  EXPECT_EQ(seg[3].next, 4u);
  EXPECT_EQ(seg[4].next, 1u);
  
  EXPECT_EQ(seg[1].prev, 4u);
  EXPECT_EQ(seg[2].prev, 1u);
  EXPECT_EQ(seg[3].prev, 2u);
  EXPECT_EQ(seg[4].prev, 3u);
  
  EXPECT_EQ(seg[1].begin.x, 0);
  EXPECT_EQ(seg[1].begin.y, 0);
  EXPECT_EQ(seg[1].end.x, 0);
  EXPECT_EQ(seg[1].end.y, 1);

  EXPECT_EQ(seg[2].begin.x, 0);
  EXPECT_EQ(seg[2].begin.y, 1);
  EXPECT_EQ(seg[2].end.x, 1);
  EXPECT_EQ(seg[2].end.y, 1);

  EXPECT_EQ(seg[3].begin.x, 1);
  EXPECT_EQ(seg[3].begin.y, 1);
  EXPECT_EQ(seg[3].end.x, 1);
  EXPECT_EQ(seg[3].end.y, 0);

  EXPECT_EQ(seg[4].begin.x, 1);
  EXPECT_EQ(seg[4].begin.y, 0);
  EXPECT_EQ(seg[4].end.x, 0);
  EXPECT_EQ(seg[4].end.y, 0);

  EXPECT_FALSE(seg[1].isInserted);
  EXPECT_FALSE(seg[2].isInserted);
  EXPECT_FALSE(seg[3].isInserted);
  EXPECT_FALSE(seg[4].isInserted);
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
