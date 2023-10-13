#include "mesh.h"

unsigned int libMesh::Mesh::nextId = 0;

libMesh::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Index3> indices)
: vertices(vertices), indices(indices)
{
    id = nextId++;
}

void libMesh::Mesh::transform(Transformation t)
{
    for(auto & vertex: vertices) {
        vertex.position.transform(t);
    }
}
