#pragma once

#include "point2.h"
#include "point3.h"
#include "vector3.h"

namespace libMesh {
    class Math {
        public:
            static float Abs(float a) {
                return std::abs(a);
            }

            static bool Equals(float a, float b) {
                return Abs(b - a) < epsilon;
            }

            static bool GreaterThan(float a, float b) {
                return a > b + epsilon;
            }

            static bool LessThan(float a, float b) {
                return a < b - epsilon;
            }

            static float Max(float a, float b) {
                return std::max(a, b);
            }

            static float Min(float a, float b) {
                return std::min(a, b);
            }
        private:
            static constexpr float epsilon = 1.0e-7f;
    };
}