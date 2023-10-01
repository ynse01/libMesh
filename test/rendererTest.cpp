#include <gtest/gtest.h>

#include "../renderer/renderer.h"

using namespace libRenderer;

TEST(Renderer, Render) {
  // Arrange
  auto renderer = Renderer();
  // Act
  bool actual = renderer.start();
  renderer.render();
  // Assert
  EXPECT_EQ(actual, true);
}
