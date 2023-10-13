#include "mesh.h"

unsigned int libMesh::Mesh::nextId = 0;

libMesh::Mesh::Mesh(std::vector<Point3> vertices, std::vector<unsigned int> indices, std::vector<Vector3> normals)
: vertices(vertices), indices(indices), normals(normals)
{
    id = nextId++;
}

libMesh::Mesh::Mesh(std::vector<Point3> vertices, std::vector<unsigned int> indices)
: vertices(vertices), indices(indices), normals(std::vector<Vector3>())
{
    id = nextId++;
}

void libMesh::Mesh::transform(Transformation t)
{
    for(auto & vertex: vertices) {
        vertex.transform(t);
    }
}
