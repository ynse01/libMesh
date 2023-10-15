#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "meshFactory.h"
#include "plane.h"
#include "point2.h"
#include "vertex.h"

libMesh::Mesh *libMesh::MeshFactory::box(Point3 center, Size2 size, Vector3 xAxis, Vector3 yAxis, float length)
{
    std::vector<Vertex> vertices;
    std::vector<Index3> indices;
    auto centerPlane = Plane(center, xAxis, yAxis);
    auto zAxis = centerPlane.getNormal();
    auto extruded = zAxis.scaled(length / 2);
    // First the front face.
    auto frontPlane = Plane(center.added(extruded), xAxis, yAxis);
    makeFaceFromRectangle(vertices, indices, frontPlane, size);
    // Secondly, the back face.
    auto backPlane = Plane(center.subtracted(extruded), xAxis.scaled(-1), yAxis);
    makeFaceFromRectangle(vertices, indices, backPlane, size);
    // Top face
    extruded = yAxis.scaled(size.height / -2);
    auto topPlane = Plane(center.added(extruded), xAxis, zAxis);
    makeFaceFromRectangle(vertices, indices, topPlane, size);
    // Bottom face
    auto bottomPlane = Plane(center.subtracted(extruded), xAxis.scaled(-1), zAxis);
    makeFaceFromRectangle(vertices, indices, bottomPlane, size);
    // Right face
    extruded = xAxis.scaled(size.width / -2);
    auto rightPlane = Plane(center.added(extruded), zAxis, yAxis);
    makeFaceFromRectangle(vertices, indices, rightPlane, size);
    // Left face
    auto leftPlane = Plane(center.subtracted(extruded), zAxis.scaled(-1), yAxis);
    makeFaceFromRectangle(vertices, indices, leftPlane, size);
    return new Mesh(vertices, indices);
}

libMesh::Mesh *libMesh::MeshFactory::cylinder(Point3 center, float diameter, Vector3 xAxis, Vector3 yAxis, float length, unsigned int numFaces)
{
    std::vector<Vertex> vertices;
    std::vector<Index3> indices;
    auto plane = Plane(center, xAxis, yAxis);
    auto radius = diameter / 2;
    auto angle = M_PI / (numFaces * 2);
    Point3 points [numFaces * 2];
    // Front face
    for(int i = 0; i < numFaces; i++)
    {
        points[i] = plane.get(Point2(sin(angle * i) * radius, cos(angle * i) * radius));
    }
    makeFaceAroundCenter(vertices, indices, center, points, numFaces);

    // Back face
    auto extruded = plane.getNormal().scaled(length);
    for(int i = 0; i < numFaces; i++)
    {
        points[numFaces + i] = points[i].added(extruded);
    }
    makeFaceAroundCenter(vertices, indices, center, points + numFaces, numFaces);

    makeFaceFromParallelCurves(vertices, indices, points, points + numFaces, numFaces);
    return new Mesh(vertices, indices);
}

void libMesh::MeshFactory::makeFaceFromRectangle(std::vector<Vertex> &vertices, std::vector<Index3> &indices, Plane &plane, Size2 size)
{
    Point3 points [4];
    auto startIndex = vertices.size();
    auto normal = plane.getNormal();
    Point2 corner = { size.width * 0.5f, size.height * 0.5f };
    auto position = plane.get(corner);
    //   3 == 0
    //   |    |
    //   2 == 1
    Vertex vertex = { position, normal, Point2(1, 1) };
    vertices.push_back(vertex);
    //std::cout << "0: " << corner << std::endl;
    corner.y = -corner.y;
    position = plane.get(corner);
    vertex = { position, normal, Point2(1, 0) };
    vertices.push_back(vertex);
    //std::cout << "1: " << corner << std::endl;
    corner.x = -corner.x;
    position = plane.get(corner);
    vertex = { position, normal, Point2(0, 0) };
    vertices.push_back(vertex);
    //std::cout << "2: " << corner << std::endl;
    corner.y = -corner.y;
    position = plane.get(corner);
    vertex = { position, normal, Point2(0, 1) };
    vertices.push_back(vertex);
    //std::cout << "3: " << corner << std::endl;
    // Create 2 triangles between the four corners.
    indices.push_back(Index3(startIndex, startIndex + 3, startIndex + 2));
    //std::cout << "Triangle: 0, 3, 2" << std::endl;
    indices.push_back(Index3(startIndex + 2, startIndex + 1, startIndex));
    //std::cout << "Triangle: 2, 1, 0" << std::endl;
}

void libMesh::MeshFactory::makeFaceFromParallelCurves(std::vector<Vertex> &vertices, std::vector<Index3> &indices, Point3 *left, Point3 *right, size_t size)
{
    unsigned int startIndex = vertices.size();
    for (int i = 0; i < size; i++)
    {
        Vertex leftVertex = { left[i], Vector3(), Point2() };
        vertices.push_back(leftVertex);
        Vertex rightVertex = { right[i], Vector3(), Point2() };
        vertices.push_back(rightVertex);
    }
    unsigned int previousLeft = startIndex;
    unsigned int previousRight = startIndex + 1;
    for (int i = 1; i < size; i++)
    {
        const unsigned int currentLeft = startIndex + i;
        const unsigned int currentRight = startIndex + i + 1;
        // Fill the rectangle between current and previous with 2 triangles.
        indices.push_back(Index3(previousLeft, currentLeft, previousRight));
        indices.push_back(Index3(currentLeft, currentRight, previousRight));
        // Preparte for next iteration
        previousLeft = currentLeft;
        previousRight = currentRight;
    }
}

void libMesh::MeshFactory::makeFaceAroundCenter(std::vector<Vertex> &vertices, std::vector<Index3> &indices, Point3 center, Point3 *edge, size_t size)
{
    unsigned int centerIndex = vertices.size();
    auto normal = Point3::betweenPoints(center, edge[size >> 2]).cross(Point3::betweenPoints(center, edge[0]));
    Vertex centerVertex = { center, normal, Point2() };
    vertices.push_back(centerVertex);
    for (int i = 0; i < size; i++)
    {
        Vertex edgeVertex = { edge[i], normal, Point2() };
        vertices.push_back(edgeVertex);
    }
    unsigned int previousIndex = centerIndex + 1;
    for (int i = 1; i < size; i++)
    {
        const unsigned int currentIndex = previousIndex + 1;
        // Put triangle between current and previous edge points.
        indices.push_back(Index3(centerIndex, currentIndex, previousIndex));
        // Preparte for next iteration
        previousIndex = currentIndex;
    }
}
