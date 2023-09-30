#include "vector.h"

libMesh::Vector::Vector() : x(0.0f), y(0.0f), z(0.0f)
{
}

libMesh::Vector::Vector(float x, float y, float z) : x(x), y(y), z(z)
{
}

float libMesh::Vector::dot(Vector &other)
{
    return x * other.x + y * other.y + z * other.z;
}

libMesh::Vector libMesh::Vector::cross(Vector &other)
{
    auto result = Vector();
    result.x = (y * other.z) - (z * other.y);
    result.y = (z * other.x) - (x * other.z);
    result.z = (x * other.y) - (y * other.x);
    return result;
}
