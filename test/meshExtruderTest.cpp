#include <gtest/gtest.h>

#include "../point3.h"
#include "../meshExtruder.h"

using namespace libMesh;

TEST(MeshExtruder, BoxNormalsShouldPointOutwards) {
  // Arrange
  auto center = Point3(0, 0, 2.5f);
  // Act
  auto box = MeshExtruder::box(Point3(), Size2(3, 4), Vector3::XAxis(), Vector3::YAxis(), 5);
  // Assert
  for (int i = 0; i < box->trianglesCount(); i++)
  {
    auto p0 = box->getVertex(box->getIndex(3 * i));
    auto p1 = box->getVertex(box->getIndex(3 * i + 2));
    auto p2 = box->getVertex(box->getIndex(3 * i + 1));
    auto toCenter = Point3::betweenPoints(center, p0);
    auto normal = Point3::betweenPoints(p0, p1).cross(Point3::betweenPoints(p0, p2));
    float dotProduct = normal.dot(toCenter);
    EXPECT_GT(dotProduct, 0.0f) << "Triangle index: " << i;
  }
}

TEST(MeshExtruder, CylinderNormalsShouldPointOutwards) {
  // Arrange
  auto center = Point3(0, 0, 2.5f);
  // Act
  auto box = MeshExtruder::cylinder(Point3(), 4, Vector3::XAxis(), Vector3::YAxis(), 5);
  // Assert
  for (int i = 0; i < box->trianglesCount(); i++)
  {
    auto p0 = box->getVertex(box->getIndex(3 * i));
    auto p1 = box->getVertex(box->getIndex(3 * i + 2));
    auto p2 = box->getVertex(box->getIndex(3 * i + 1));
    auto toCenter = Point3::betweenPoints(center, p0);
    auto normal = Point3::betweenPoints(p0, p1).cross(Point3::betweenPoints(p0, p2));
    float dotProduct = normal.dot(toCenter);
    EXPECT_GT(dotProduct, 0.0f) << "Triangle index: " << i;
  }
}
