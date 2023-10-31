#include <gtest/gtest.h>

#include "../yaui/renderer.h"
#include "../yaui/meshRenderable.h"
#include "../meshFactory.h"
#include "../point3.h"
#include "../size2.h"
#include "../vector3.h"

using namespace YetAnotherUI;
using namespace libMesh;

TEST(Renderer, Render) {
  // Arrange
  auto renderer = Renderer();
  auto size = Size2(0.6f, 0.5f);
  auto box = MeshFactory::box(Point3(), size, Vector3::XAxis, Vector3::YAxis, 0.1f);
  auto boxVisual = MeshRenderable(*box);
  // Act
  bool actual = renderer.start();
  renderer.add(&boxVisual);
  renderer.run();
  renderer.close();
  // Assert
  EXPECT_GT(box->trianglesCount(), 10);
  EXPECT_EQ(actual, true);
}
