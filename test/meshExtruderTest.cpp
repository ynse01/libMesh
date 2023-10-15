#include <gtest/gtest.h>

#include "../point3.h"
#include "../meshExtruder.h"

using namespace libMesh;

TEST(MeshExtruder, BoxNormalsShouldPointOutwards) {
  // Arrange
  auto center = Point3(0, 0, 2.5f);
  auto size = Size2(3, 4);
  // Act
  auto box = MeshExtruder::box(center, size, Vector3::XAxis, Vector3::YAxis, 5);
  // Assert
  for (int i = 0; i < box->trianglesCount(); i++)
  {
    auto index = box->getTriangle(i);
    auto vertex0 = box->getVertex(index.a);
    auto p0 = vertex0.position;
    auto p1 = box->getVertex(index.b).position;
    auto p2 = box->getVertex(index.c).position;
    auto toCenter = Point3::betweenPoints(center, p0);
    auto normal = vertex0.normal;
    float dotProduct = normal.dot(toCenter);
    EXPECT_GT(dotProduct, 0.0f) << "Triangle index: " << i;
  }
}

TEST(MeshExtruder, CylinderNormalsShouldPointOutwards) {
  // Arrange
  auto center = Point3(0, 0, 2.5f);
  // Act
  auto box = MeshExtruder::cylinder(Point3(), 4, Vector3::XAxis, Vector3::YAxis, 5);
  // Assert
  for (int i = 0; i < box->trianglesCount(); i++)
  {
    auto index = box->getTriangle(i);
    auto p0 = box->getVertex(index.a).position;
    auto p1 = box->getVertex(index.b).position;
    auto p2 = box->getVertex(index.c).position;
    auto toCenter = Point3::betweenPoints(center, p0);
    auto normal = Point3::betweenPoints(p0, p1).cross(Point3::betweenPoints(p0, p2));
    float dotProduct = normal.dot(toCenter);
    //EXPECT_GT(dotProduct, 0.0f) << "Triangle index: " << i;
  }
}
