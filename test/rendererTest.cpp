#include <gtest/gtest.h>

#include <string>

#include "../yaui/renderer.h"
#include "../yaui/meshRenderable.h"
#include "../yaui/label.h"
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
  auto fontPath = std::string("/usr/share/fonts/truetype/Sarai/Sarai.ttf");
  auto font = Font(fontPath);
  auto text = std::string("Hello world!");
  auto textBrush = SolidTextBrush(Color());
  auto label = Label(font, text, textBrush, 0.005f);
  // Act
  bool actual = renderer.start();
  renderer.add(&boxVisual);
  //renderer.add(&label);
  renderer.run();
  renderer.close();
  // Assert
  EXPECT_GT(box->trianglesCount(), 10);
  EXPECT_EQ(actual, true);
}
