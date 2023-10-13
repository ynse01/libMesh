#define _USE_MATH_DEFINES
#include <math.h>

#include "meshExtruder.h"
#include "plane.h"

libMesh::Mesh *libMesh::MeshExtruder::box(Point3 center, Size2 size, Vector3 xAxis, Vector3 yAxis, float length)
{
    std::vector<Vertex> vertices;
    std::vector<Index3> indices;
    auto plane = Plane(center, xAxis, yAxis);
    auto halfSize = size.scale(0.5f);
    // First the in-plane rectangle.
    Point3 points [4];
    points[0] = plane.get(halfSize);
    halfSize.height = -halfSize.height;
    points[1] = plane.get(halfSize);
    halfSize.width = -halfSize.width;
    points[2] = plane.get(halfSize);
    halfSize.height = -halfSize.height;
    points[3] = plane.get(halfSize);
    makeFaceFromParallelCurves(vertices, indices, points, points + 2, 4);
    // Secondly, the opposite rectangle.
    auto extruded = xAxis.cross(yAxis).scale(length);
    points[0] = points[0].add(extruded);
    points[1] = points[1].add(extruded);
    points[2] = points[2].add(extruded);
    points[3] = points[3].add(extruded);
    makeFaceFromParallelCurves(vertices, indices, points, points + 2, 4);
    //     7 ===  4
    //   //    // ||
    //  3 === 0   5
    //  ||  6 ||//
    //  2 === 1
    // Indices of front face
    indices.push_back(Index3(0, 2, 1));
    indices.push_back(Index3(0, 3, 2));
    // Indices of top face
    indices.push_back(Index3(3, 0, 7));
    indices.push_back(Index3(4, 7, 0));
    // Indices of left face
    indices.push_back(Index3(2, 3, 6));
    indices.push_back(Index3(3, 7, 6));
    // Indices of bottom face
    indices.push_back(Index3(1, 2, 6));
    indices.push_back(Index3(1, 6, 5));
    // Indices of right face
    indices.push_back(Index3(0, 5, 4));
    indices.push_back(Index3(0, 1, 5));
    // Indices of back face
    indices.push_back(Index3(4, 5, 6));
    indices.push_back(Index3(4, 6, 7));
    return new Mesh(vertices, indices);
}

libMesh::Mesh *libMesh::MeshExtruder::cylinder(Point3 center, float diameter, Vector3 xAxis, Vector3 yAxis, float length, unsigned int numFaces)
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
        points[i] = plane.get(Size2(sin(angle * i) * radius, cos(angle * i) * radius));
    }
    makeFaceAroundCenter(vertices, indices, center, points, numFaces);

    // Back face
    auto extruded = plane.getNormal().scale(length);
    for(int i = 0; i < numFaces; i++)
    {
        points[numFaces + i] = points[i].add(extruded);
    }
    makeFaceAroundCenter(vertices, indices, center, points + numFaces, numFaces);

    makeFaceFromParallelCurves(vertices, indices, points, points + numFaces, numFaces);
    return new Mesh(vertices, indices);
}

void libMesh::MeshExtruder::makeFaceFromParallelCurves(std::vector<Vertex> &vertices, std::vector<Index3> &indices, Point3 *left, Point3 *right, size_t size)
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

void libMesh::MeshExtruder::makeFaceAroundCenter(std::vector<Vertex> &vertices, std::vector<Index3> &indices, Point3 center, Point3 *edge, size_t size)
{
    unsigned int centerIndex = vertices.size();
    Vertex centerVertex = { center, Vector3(), Point2() };
    vertices.push_back(centerVertex);
    for (int i = 0; i < size; i++)
    {
        Vertex edgeVertex = { edge[i], Vector3(), Point2() };
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
