#include <gtest/gtest.h>

#include "../point3.h"

using namespace libMesh;

TEST(Point3, TransformOnlyTranslation) {
  // Arrange
  auto p = Point3(3, 4, 5);
  auto t = Transformation(1.0f, 0, 0, 17, 0, 1, 0, 27, 0, 0, 1, 37, 1);
  // Act
  auto actual = p.transformed(t);
  // Assert
  EXPECT_EQ(actual.x, 20);
  EXPECT_EQ(actual.y, 31);
  EXPECT_EQ(actual.z, 42);
}
