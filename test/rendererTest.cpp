#include <gtest/gtest.h>

#include "../renderer/renderer.h"
#include "../meshExtruder.h"
#include "../point3.h"
#include "../size2.h"
#include "../vector3.h"

using namespace libRenderer;
using namespace libMesh;

TEST(Renderer, Render) {
  // Arrange
  auto renderer = Renderer();
  auto size = Size2(0.6f, 0.5f);
  auto box = MeshExtruder::box(Point3(), size, Vector3::XAxis, Vector3::YAxis, 0.1f);
  // Act
  bool actual = renderer.start();
  renderer.addMesh(box);
  renderer.run();
  // Assert
  EXPECT_GT(box->trianglesCount(), 10);
  EXPECT_EQ(actual, true);
}
