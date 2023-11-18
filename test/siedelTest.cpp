#include <gtest/gtest.h>

#include "../siedel.private.h"

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
  EXPECT_EQ(seg[1].next, 2u);
  EXPECT_EQ(seg[2].next, 1u);
  EXPECT_EQ(seg[3].next, 2u);
  EXPECT_EQ(seg[4].next, 1u);
  EXPECT_EQ(seg[1].prev, 4u);
  EXPECT_EQ(seg[2].prev, 3u);
  EXPECT_EQ(seg[3].prev, 4u);
  EXPECT_EQ(seg[4].prev, 3u);
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
