#pragma once

#include "point2.h"
#include "point3.h"
#include "vector3.h"

namespace libMesh {
    class Math {
        public:
            static float Abs(float a) {
                return fabsf32(a);
            }

            static bool Equals(float a, float b) {
                return Abs(b - a) < epsilon;
            }
        private:
            static constexpr float epsilon = 1.0e-7f;
    };
}