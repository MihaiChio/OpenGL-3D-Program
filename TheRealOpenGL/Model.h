#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "textureClass.h"

class Model
{
public:
	Model();

	void loadModel(const std::string& filename);
	void renderModel();
	void clearModel();


	~Model();
private:

	void loadNode(aiNode * node, const aiScene *scene); //this will slightly save memory because will use the node within the texture,scene and object
	void loadMesh(aiMesh* mesh, const aiScene* scene);
	void loadMaterial(const aiScene* scene);

	std::vector<Mesh*> meshList;
	std::vector<textureClass*> textureList;
	std::vector<unsigned int> meshToTex;

};

