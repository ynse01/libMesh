#include "vector3.h"
#include "point3.h"

libMesh::Vector3::Vector3() : Vector3(0.0f, 0.0f, 0.0f)
{
}

libMesh::Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}

float libMesh::Vector3::dot(Vector3 other)
{
    return x * other.x + y * other.y + z * other.z;
}

libMesh::Vector3 libMesh::Vector3::cross(Vector3 other)
{
    auto result = Vector3();
    result.x = (y * other.z) - (z * other.y);
    result.y = (z * other.x) - (x * other.z);
    result.z = (x * other.y) - (y * other.x);
    return result;
}

libMesh::Vector3 libMesh::Vector3::transform(Transformation t)
{
    float newX = (x * t.m00 + y * t.m10 + z * t.m20) / t.m33;
    float newY = (x * t.m01 + y * t.m11 + z * t.m21) / t.m33;
    float newZ = (x * t.m02 + y * t.m12 + z * t.m22) / t.m33;
    return Vector3(newX, newY, newZ);
}

libMesh::Vector3 libMesh::Vector3::scale(float value)
{
    return Vector3(x * value, y * value, z * value);
}
