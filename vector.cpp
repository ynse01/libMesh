#include "vector.h"
#include "point.h"

libMesh::Vector::Vector() : Vector(0.0f, 0.0f, 0.0f)
{
}

libMesh::Vector::Vector(float x, float y, float z) : x(x), y(y), z(z)
{
}

float libMesh::Vector::dot(Vector other)
{
    return x * other.x + y * other.y + z * other.z;
}

libMesh::Vector libMesh::Vector::cross(Vector other)
{
    auto result = Vector();
    result.x = (y * other.z) - (z * other.y);
    result.y = (z * other.x) - (x * other.z);
    result.z = (x * other.y) - (y * other.x);
    return result;
}

libMesh::Vector libMesh::Vector::transform(Transformation t)
{
    float newX = (x * t.m00 + y * t.m10 + z * t.m20) / t.m33;
    float newY = (x * t.m01 + y * t.m11 + z * t.m21) / t.m33;
    float newZ = (x * t.m02 + y * t.m12 + z * t.m22) / t.m33;
    return Vector(newX, newY, newZ);
}

libMesh::Vector libMesh::Vector::scale(float value)
{
    return Vector(x * value, y * value, z * value);
}
