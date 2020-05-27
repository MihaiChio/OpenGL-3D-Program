#include "Model.h"





Model::Model()
{

}
void Model::renderModel()
{
	for (size_t i = 0; i < meshList.size(); i++) // going through all the meshes.
	{
		unsigned int  materialIndex = meshToTex[i]; // the current meshlist will have the same position as the material
		if (materialIndex < textureList.size() && textureList[materialIndex] )
		{
			textureList[materialIndex]->useTexture();
		}

		meshList[i]->renderMesh();
	}
}

void Model::loadModel(const std::string& filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);// Triangulate forces more complex shapes to be split into triangles
	if (!scene)
	{
		printf("model failed to load: %s", importer.GetErrorString());
		return;
	}
	
	loadNode(scene->mRootNode, scene); //gets the first node of the scene.
	loadMaterial(scene);
}				//Smooth normals because I am using interpolation so this will make everythign rounded.	// if there are two vertices at the same point it won't draw it again


void Model::clearModel()
{
	///// deleting the mesh
	for (size_t i = 0; i < meshList.size(); i++) // going through all the mesh list and delete it.
	{
		if(meshList[i])
			{
				delete meshList[i];
				meshList[i] = nullptr;
			}
	}

	////deleting the texture
	for (size_t i = 0; i < textureList.size(); i++) // going through all the mesh list and delete it.
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}

}

void Model::loadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes;i++)
	{
		loadMesh(scene->mMeshes[node->mMeshes[i]], scene); // gets the ID of the mesh to take from scene. node->mMesh stores only a reference to the mesh.
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		loadNode(node->mChildren[i], scene); // This is gonna be saved directly into the node.
	}

}//this will slightly save memory because will use the node within the texture,scene and object
void Model::loadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;
	
	for (size_t i = 0; i < mesh->mNumVertices;i++)
	{									//object coordinates.
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y ,mesh->mVertices[i].z }); // adding multiple values to the vector at the same time.
		
		if (mesh->mTextureCoords[0]) //checking if the texture exists
		{
												//Texture coordinates UV
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y }); // adding multiple values to the vector at the same time.	
		}
		else
		{
			vertices.insert(vertices.end(), {0.0f,0.0f}); 
			// if values are not passed openGL will skip and will take the wrong values in.
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y ,-mesh->mNormals[i].z }); // adding multiple values to the vector at the same time.
		//we generate normals so they will always exist. // Adding the normals in reverse because the fragment shader was defined with positive normals.
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);// going through each face and adding it to the indices list.
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->createMesh(&vertices[0], &indices[0], vertices.size(), indices.size()); // making the vector an array. 
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);

}
void Model::loadMaterial(const aiScene* scene) // going through all the material and placing it in the appropriate location in the scene.
{
	textureList.resize(scene->mNumMaterials); // resizing the list because there will be some bits that will not be used
	
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i]; // looping through each material.

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int index = std::string(path.data).rfind("\\"); // this is to be able to load models that were exported with an absolute path
				std::string fileName = std::string(path.data).substr(index + 1);


				std::string texPath = std::string("Textures/") + fileName;

				textureList[i] = new textureClass(texPath.c_str());

				if (!textureList[i]->loadTexture(true))
				{
					printf("failed to load texture");
					delete textureList[i];
					textureList[i] = nullptr; // safe delete to stop memory leaks.
				}
			}
		}

		if (!textureList[i])
		{
			textureList[i] = new textureClass("Textures/rust.jpg");
			textureList[i]->loadTextureA(); // if files are missing, this default texture is gonna be applied.
		}

	}

}
Model::~Model()
{}
