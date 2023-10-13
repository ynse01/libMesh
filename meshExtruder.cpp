#define _USE_MATH_DEFINES
#include <math.h>

#include "meshExtruder.h"
#include "plane.h"

libMesh::Mesh *libMesh::MeshExtruder::box(Point3 center, Size2 size, Vector3 xAxis, Vector3 yAxis, float length)
{
    std::vector<Point3> vertices;
    std::vector<unsigned int> indices;
    auto plane = Plane(center, xAxis, yAxis);
    auto halfSize = size.scale(0.5f);
    // First the in-plane rectangle.
    vertices.push_back(plane.get(halfSize));
    halfSize.height = -halfSize.height;
    vertices.push_back(plane.get(halfSize));
    halfSize.width = -halfSize.width;
    vertices.push_back(plane.get(halfSize));
    halfSize.height = -halfSize.height;
    vertices.push_back(plane.get(halfSize));
    // Secondly, the oposite rectangle.
    auto extruded = xAxis.cross(yAxis).scale(length);
    vertices.push_back(vertices[0].add(extruded));
    vertices.push_back(vertices[1].add(extruded));
    vertices.push_back(vertices[2].add(extruded));
    vertices.push_back(vertices[3].add(extruded));
    //     7 ===  4
    //   //    // ||
    //  3 === 0   5
    //  ||  6 ||//
    //  2 === 1
    // Indices of front face
    indices.push_back(0);indices.push_back(2);indices.push_back(1);
    indices.push_back(0);indices.push_back(3);indices.push_back(2);
    // Indices of top face
    indices.push_back(3);indices.push_back(0);indices.push_back(7);
    indices.push_back(4);indices.push_back(7);indices.push_back(0);
    // Indices of left face
    indices.push_back(2);indices.push_back(3);indices.push_back(6);
    indices.push_back(3);indices.push_back(7);indices.push_back(6);
    // Indices of bottom face
    indices.push_back(1);indices.push_back(2);indices.push_back(6);
    indices.push_back(1);indices.push_back(6);indices.push_back(5);
    // Indices of right face
    indices.push_back(0);indices.push_back(5);indices.push_back(4);
    indices.push_back(0);indices.push_back(1);indices.push_back(5);
    // Indices of back face
    indices.push_back(4);indices.push_back(5);indices.push_back(6);
    indices.push_back(4);indices.push_back(6);indices.push_back(7);
    return new Mesh(vertices, indices);
}

libMesh::Mesh *libMesh::MeshExtruder::cylinder(Point3 center, float diameter, Vector3 xAxis, Vector3 yAxis, float length, int numFaces)
{
    std::vector<Point3> vertices;
    std::vector<unsigned int> indices;
    auto plane = Plane(center, xAxis, yAxis);
    auto radius = diameter / 2;
    auto angle = M_PI / (numFaces * 2);
    // Front face
    auto previousPoint = plane.get(Size2(0, radius));
    vertices.push_back(plane.origin);
    vertices.push_back(previousPoint);
    for (int i = 1; i < numFaces; i++)
    {
        auto currentPoint = plane.get(Size2(sin(angle * i) * radius, cos(angle * i) * radius));
        vertices.push_back(currentPoint);
        indices.push_back(0);indices.push_back(i + 1);indices.push_back(i);
    }
    // Back face
    auto zAxis = xAxis.cross(yAxis).scale(length);
    auto backCenter = center.add(zAxis);
    auto backPlane = Plane(backCenter, xAxis, yAxis);
    previousPoint = backPlane.get(Size2(0, radius)).add(zAxis);
    vertices.push_back(backPlane.origin);
    vertices.push_back(previousPoint);
    for (int i = 1; i < numFaces; i++)
    {
        auto currentPoint = backPlane.get(Size2(sin(angle * i) * radius, cos(angle * i) * radius));
        vertices.push_back(currentPoint);
        indices.push_back(numFaces + 1);indices.push_back(i + numFaces + 1);indices.push_back(i + numFaces + 2);
    }
    // Side curved face
    for (int i = 1; i < numFaces; i++)
    {
        indices.push_back(i);indices.push_back(i + 1);indices.push_back(i + numFaces + 1);
        indices.push_back(i + 1);indices.push_back(i + numFaces + 2);indices.push_back(i + numFaces + 1);
    }
    return new Mesh(vertices, indices);
}
