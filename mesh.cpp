#include "mesh.h"

libMesh::Mesh::Mesh(std::vector<Point> vertices, std::vector<unsigned int> indices, std::vector<Vector> normals)
: vertices(vertices), indices(indices), normals(normals)
{
}

libMesh::Mesh::Mesh(std::vector<Point> vertices, std::vector<unsigned int> indices)
: vertices(vertices), indices(indices), normals(std::vector<Vector>()){
}

void libMesh::Mesh::transform(Transformation t)
{
    for(auto & vertex: vertices) {
        vertex.transform(t);
    }
}
