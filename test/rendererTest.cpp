#include <gtest/gtest.h>

#include "../renderer/renderer.h"
#include "../meshExtruder.h"
#include "../point3.h"
#include "../size2.h"
#include "../vector3.h"

using namespace libRenderer;

TEST(Renderer, Render) {
  // Arrange
  auto renderer = Renderer();
  auto box = libMesh::MeshExtruder::box(libMesh::Point3(), libMesh::Size2(0.5f, 0.5f), libMesh::Vector3::XAxis(), libMesh::Vector3::YAxis(), 0.8f);
  // Act
  bool actual = renderer.start();
  renderer.addMesh(box);
  renderer.run();
  // Assert
  EXPECT_GT(box->trianglesCount(), 10);
  EXPECT_EQ(actual, true);
}
