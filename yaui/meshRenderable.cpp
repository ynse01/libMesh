#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "meshRenderable.h"
#include "../vertex.h"

void YetAnotherUI::MeshRenderable::Initialize()
{
    glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mMesh.verticesCount() * sizeof(libMesh::Vertex), mMesh.getVertexPtr(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh.trianglesCount() * 3 * sizeof(unsigned int), mMesh.getIndexPtr(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(libMesh::Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(libMesh::Vertex), (void*)offsetof(libMesh::Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(libMesh::Vertex), (void*)offsetof(libMesh::Vertex, texCoord));

	// Reset state machine
	glBindVertexArray(0);

	mBrush.Initialize();
}

void YetAnotherUI::MeshRenderable::Render()
{
	mBrush.Render();
	glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, mMesh.trianglesCount() * 3);
	glBindVertexArray(0);
}

void YetAnotherUI::MeshRenderable::Destroy()
{
	mBrush.Destroy();
    glDeleteBuffers(1, &mEBO);
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);
}
