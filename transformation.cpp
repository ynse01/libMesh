#include "transformation.h"

libMesh::Transformation::Transformation() : Transformation(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f)
{
}

libMesh::Transformation::Transformation(const Transformation &other)
{
    m00 = other.m00;
    m01 = other.m01;
    m02 = other.m02;
    m03 = other.m03;
    m10 = other.m10;
    m11 = other.m11;
    m12 = other.m12;
    m13 = other.m13;
    m20 = other.m20;
    m21 = other.m21;
    m22 = other.m22;
    m23 = other.m23;
    m33 = other.m33;
}

libMesh::Transformation::Transformation(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m33)
    : m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11), m12(m12), m13(m13), m20(m20), m21(m21), m22(m22), m23(m23), m33(m33)
{
}