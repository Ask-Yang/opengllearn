#pragma once
#include <string>
#include <vector>
#include <set>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Texture.h"
#include "Mesh.h"
#include "RenderPass.h"
#include "Shader.h"


class Model {
public:
	Model(std::string path);
	void use(Shader& shader);
private:
	std::vector<std::shared_ptr<Texture2D>> textures_loaded;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::string directory;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scence);
	std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};