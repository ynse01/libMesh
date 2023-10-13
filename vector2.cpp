#include "vector2.h"

libMesh::Vector2::Vector2() : Vector2(0.0f, 0.0f)
{
}

libMesh::Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

float libMesh::Vector2::dot(Vector2 other)
{
    return x * other.x + y * other.y;
}

libMesh::Vector2 libMesh::Vector2::scale(float value)
{
    return Vector2(x * value, y * value);
}
