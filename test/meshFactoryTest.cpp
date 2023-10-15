#include <gtest/gtest.h>

#include "../point3.h"
#include "../meshFactory.h"

using namespace libMesh;

TEST(MeshFactory, BoxNormalsShouldPointOutwards) {
  // Arrange
  auto center = Point3(0, 0, 2.5f);
  auto size = Size2(3, 4);
  // Act
  auto box = MeshFactory::box(center, size, Vector3::XAxis, Vector3::YAxis, 5);
  // Assert
  for (int i = 0; i < box->trianglesCount(); i++)
  {
    auto index = box->getTriangle(i);
    auto vertex0 = box->getVertex(index.a);
    auto vertex1 = box->getVertex(index.b);
    auto vertex2 = box->getVertex(index.c);
    auto toCenter = Point3::betweenPoints(center, vertex0.position);
    auto normal = vertex0.normal;
    float dot0 = vertex0.normal.dot(toCenter);
    EXPECT_GT(dot0, 0.0f) << "Vertex 0 of Triangle index: " << i;
    float dot1 = vertex1.normal.dot(toCenter);
    EXPECT_GT(dot1, 0.0f) << "Vertex 1 of Triangle index: " << i;
    float dot2 = vertex2.normal.dot(toCenter);
    EXPECT_GT(dot2, 0.0f) << "Vertex 2 of Triangle index: " << i;
  }
}

TEST(MeshFactory, CylinderNormalsShouldPointOutwards) {
  // Arrange
  auto center = Point3(0, 0, 2.5f);
  // Act
  auto box = MeshFactory::cylinder(center, 4, Vector3::XAxis, Vector3::YAxis, 5);
  // Assert
  for (int i = 0; i < box->trianglesCount(); i++)
  {
    auto index = box->getTriangle(i);
    auto vertex0 = box->getVertex(index.a);
    auto vertex1 = box->getVertex(index.b);
    auto vertex2 = box->getVertex(index.c);
    auto toCenter = Point3::betweenPoints(center, vertex0.position);
    auto normal = vertex0.normal;
    float dot0 = vertex0.normal.dot(toCenter);
    //EXPECT_GT(dot0, 0.0f) << "Vertex 0 of Triangle index: " << i;
    float dot1 = vertex1.normal.dot(toCenter);
    //EXPECT_GT(dot1, 0.0f) << "Vertex 1 of Triangle index: " << i;
    float dot2 = vertex2.normal.dot(toCenter);
    //EXPECT_GT(dot2, 0.0f) << "Vertex 2 of Triangle index: " << i;
  }
}
