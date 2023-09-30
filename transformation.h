#pragma once

namespace libMesh {
    /// @brief A spatial transformation, consisting of rotation, translation and scaling.
    struct Transformation {
        public:
            Transformation();
            Transformation(Transformation& other);
            Transformation(
                float m00, float m01, float m02, float m03,
                float m10, float m11, float m12, float m13,
                float m20, float m21, float m22, float m23,
                float m33 );
            float m00;
            float m01;
            float m02;
            float m03;
            float m10;
            float m11;
            float m12;
            float m13;
            float m20;
            float m21;
            float m22;
            float m23;
            float m33;
    };
}