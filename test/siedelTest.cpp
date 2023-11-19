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
  polygon.add(libMesh::Point2(1, 1));
  polygon.add(libMesh::Point2(1, 2));
  polygon.add(libMesh::Point2(2, 2));
  polygon.add(libMesh::Point2(2, 1));
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
  
  EXPECT_EQ(seg[1].begin.x, 1);
  EXPECT_EQ(seg[1].begin.y, 1);
  EXPECT_EQ(seg[1].end.x, 1);
  EXPECT_EQ(seg[1].end.y, 2);

  EXPECT_EQ(seg[2].begin.x, 1);
  EXPECT_EQ(seg[2].begin.y, 2);
  EXPECT_EQ(seg[2].end.x, 2);
  EXPECT_EQ(seg[2].end.y, 2);

  EXPECT_EQ(seg[3].begin.x, 2);
  EXPECT_EQ(seg[3].begin.y, 2);
  EXPECT_EQ(seg[3].end.x, 2);
  EXPECT_EQ(seg[3].end.y, 1);

  EXPECT_EQ(seg[4].begin.x, 2);
  EXPECT_EQ(seg[4].begin.y, 1);
  EXPECT_EQ(seg[4].end.x, 1);
  EXPECT_EQ(seg[4].end.y, 1);

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

TEST(QueryStructure, InitializationTest) {
  // Arrange
  auto seg = SegmentStructure(5);
  auto polygon = libMesh::PolyLine();
  polygon.add(libMesh::Point2(1, 1));
  polygon.add(libMesh::Point2(1, 2));
  polygon.add(libMesh::Point2(2, 2));
  polygon.add(libMesh::Point2(2, 1));
  unsigned int contours = 4;
  auto numSegments = siedel::initializeSegments(seg, polygon, &contours, 1);
  auto qs = QueryStructure(8 * 5);
  auto trap = TrapezoidStructure(4 * 5);
  unsigned int segmentIndex = 1u;
  // Act
  auto root = siedel::initQueryStructure(seg, segmentIndex, qs, trap);
  // Assert
  EXPECT_EQ(root, 0u);
  EXPECT_EQ(qs.size(), 7u);
  EXPECT_TRUE(seg[segmentIndex].isInserted);

  EXPECT_EQ(qs[0].type, NodeType::Y);
  EXPECT_EQ(qs[0].left, 2u);
  EXPECT_EQ(qs[0].right, 1u);
  EXPECT_EQ(qs[0].yValue, libMesh::Point2(1, 2));
  EXPECT_EQ(qs[0].parent, 0u);
  EXPECT_EQ(qs[0].segmentIndex, 0u);
  EXPECT_EQ(qs[0].trapezoidIndex, 0u);

  EXPECT_EQ(qs[1].type, NodeType::Sink);
  EXPECT_EQ(qs[1].left, 0u);
  EXPECT_EQ(qs[1].right, 0u);
  EXPECT_EQ(qs[1].yValue, libMesh::Point2(0, 0));
  EXPECT_EQ(qs[1].parent, 0u);
  EXPECT_EQ(qs[1].segmentIndex, 0u);
  EXPECT_EQ(qs[1].trapezoidIndex, 3u);

  EXPECT_EQ(qs[2].type, NodeType::Y);
  EXPECT_EQ(qs[2].left, 3u);
  EXPECT_EQ(qs[2].right, 4u);
  EXPECT_EQ(qs[2].yValue, libMesh::Point2(1, 1));
  EXPECT_EQ(qs[2].parent, 0u);
  EXPECT_EQ(qs[2].segmentIndex, 0u);
  EXPECT_EQ(qs[2].trapezoidIndex, 0u);

  EXPECT_EQ(qs[3].type, NodeType::Sink);
  EXPECT_EQ(qs[3].left, 0u);
  EXPECT_EQ(qs[3].right, 0u);
  EXPECT_EQ(qs[3].yValue, libMesh::Point2(0, 0));
  EXPECT_EQ(qs[3].parent, 2u);
  EXPECT_EQ(qs[3].segmentIndex, 0u);
  EXPECT_EQ(qs[3].trapezoidIndex, 2u);

  EXPECT_EQ(qs[4].type, NodeType::X);
  EXPECT_EQ(qs[4].left, 5u);
  EXPECT_EQ(qs[4].right, 6u);
  EXPECT_EQ(qs[4].yValue, libMesh::Point2(0, 0));
  EXPECT_EQ(qs[4].parent, 2u);
  EXPECT_EQ(qs[4].segmentIndex, 1u);
  EXPECT_EQ(qs[4].trapezoidIndex, 0u);

  EXPECT_EQ(qs[5].type, NodeType::Sink);
  EXPECT_EQ(qs[5].left, 0u);
  EXPECT_EQ(qs[5].right, 0u);
  EXPECT_EQ(qs[5].yValue, libMesh::Point2(0, 0));
  EXPECT_EQ(qs[5].parent, 4u);
  EXPECT_EQ(qs[5].segmentIndex, 0u);
  EXPECT_EQ(qs[5].trapezoidIndex, 0u);

  EXPECT_EQ(qs[6].type, NodeType::Sink);
  EXPECT_EQ(qs[6].left, 0u);
  EXPECT_EQ(qs[6].right, 0u);
  EXPECT_EQ(qs[6].yValue, libMesh::Point2(0, 0));
  EXPECT_EQ(qs[6].parent, 4u);
  EXPECT_EQ(qs[6].segmentIndex, 0u);
  EXPECT_EQ(qs[6].trapezoidIndex, 1u);
}

TEST(TrapezoidStructure, InitializationTest) {
  // Arrange
  auto seg = SegmentStructure(5);
  auto polygon = libMesh::PolyLine();
  polygon.add(libMesh::Point2(1, 1));
  polygon.add(libMesh::Point2(1, 2));
  polygon.add(libMesh::Point2(2, 2));
  polygon.add(libMesh::Point2(2, 1));
  unsigned int contours = 4;
  auto numSegments = siedel::initializeSegments(seg, polygon, &contours, 1);
  auto qs = QueryStructure(8 * 5);
  auto trap = TrapezoidStructure(4 * 5);
  unsigned int segmentIndex = 1u;
  // Act
  auto root = siedel::initQueryStructure(seg, segmentIndex, qs, trap);
  // Assert
  EXPECT_EQ(root, 0u);
  EXPECT_EQ(qs.size(), 7u);
  EXPECT_TRUE(seg[segmentIndex].isInserted);

  EXPECT_EQ(trap[0].high, libMesh::Point2(1, 2));
  EXPECT_EQ(trap[0].low, libMesh::Point2(1, 1));
  EXPECT_EQ(trap[0].up0, 3u);
  EXPECT_EQ(trap[0].up1, 0u);
  EXPECT_EQ(trap[0].up2, 0u);
  EXPECT_EQ(trap[0].down0, 2u);
  EXPECT_EQ(trap[0].down1, 0u);
  EXPECT_EQ(trap[0].leftSegment, 0u);
  EXPECT_EQ(trap[0].rightSegment, 1u);
  EXPECT_EQ(trap[0].sink, 5u);
  EXPECT_TRUE(trap[0].state);

  EXPECT_EQ(trap[1].high, libMesh::Point2(1, 2));
  EXPECT_EQ(trap[1].low, libMesh::Point2(1, 1));
  EXPECT_EQ(trap[1].up0, 3u);
  EXPECT_EQ(trap[1].up1, 0u);
  EXPECT_EQ(trap[1].up2, 0u);
  EXPECT_EQ(trap[1].down0, 2u);
  EXPECT_EQ(trap[1].down1, 0u);
  EXPECT_EQ(trap[1].leftSegment, 1u);
  EXPECT_EQ(trap[1].rightSegment, 0u);
  EXPECT_EQ(trap[1].sink, 6u);
  EXPECT_TRUE(trap[1].state);

  EXPECT_EQ(trap[2].high, libMesh::Point2(1, 1));
  //EXPECT_EQ(trap[2].low, libMesh::Point2(-inf, -inf));
  EXPECT_EQ(trap[2].up0, 0u);
  EXPECT_EQ(trap[2].up1, 1u);
  EXPECT_EQ(trap[2].up2, 0u);
  EXPECT_EQ(trap[2].down0, 0u);
  EXPECT_EQ(trap[2].down1, 0u);
  EXPECT_EQ(trap[2].leftSegment, 0u);
  EXPECT_EQ(trap[2].rightSegment, 0u);
  EXPECT_EQ(trap[2].sink, 3u);
  EXPECT_TRUE(trap[2].state);

  //EXPECT_EQ(trap[3].high, libMesh::Point2(inf, inf));
  EXPECT_EQ(trap[3].low, libMesh::Point2(1, 2));
  EXPECT_EQ(trap[3].up0, 0u);
  EXPECT_EQ(trap[3].up1, 0u);
  EXPECT_EQ(trap[3].up2, 0u);
  EXPECT_EQ(trap[3].down0, 0u);
  EXPECT_EQ(trap[3].down1, 1u);
  EXPECT_EQ(trap[3].leftSegment, 0u);
  EXPECT_EQ(trap[3].rightSegment, 0u);
  EXPECT_EQ(trap[3].sink, 1u);
  EXPECT_TRUE(trap[3].state);
}