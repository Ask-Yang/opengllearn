#pragma once
#include "Shader.h"
#include "Texture.h"
#include <vector>
#include <string>
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture2D>> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);
	void use(Shader& shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};