#include <gtest/gtest.h>

#include "../point.h"
#include "../meshExtruder.h"

using namespace libMesh;

TEST(MeshExtruder, BoxNormalsShouldPointOutwards) {
  // Arrange
  Point center = Point(0, 0, 2.5f);
  // Act
  auto box = MeshExtruder::box(Point(), Size2(3, 4), Vector::XAxis(), Vector::YAxis(), 5);
  // Assert
  for (int i = 0; i < box->trianglesCount(); i++)
  {
    Point p0 = box->getVertex(box->getIndex(3 * i));
    Point p1 = box->getVertex(box->getIndex(3 * i + 2));
    Point p2 = box->getVertex(box->getIndex(3 * i + 1));
    Vector toCenter = Point::betweenPoints(center, p0);
    Vector normal = Point::betweenPoints(p0, p1).cross(Point::betweenPoints(p0, p2));
    float dotProduct = normal.dot(toCenter);
    EXPECT_GT(dotProduct, 0.0f) << "Triangle index: " << i;
  }
}
