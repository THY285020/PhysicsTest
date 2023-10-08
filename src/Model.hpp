#pragma once
#include "Shader.h"
#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include <iostream>

class Model
{
public:
	Model(const std::string& path)
	{
		loadModel(path);
	}
	void Draw(Shader shader);
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;//存储加载过的纹理

	void loadModel(std::string Path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);
};

void Model::Draw(Shader shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

inline void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	//加载模型（文件路径|后期处理）
	//aiProcess_GenNormals：如果模型不包含法向量的话，就为每个顶点创建法线。
	//aiProcess_SplitLargeMeshes：将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用。
	//aiProcess_OptimizeMeshes：和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化。
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

inline void Model::processNode(aiNode* node, const aiScene* scene)
{
	//处理节点的所有网格
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// 处理顶点位置、法线和纹理坐标
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;
		
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// 处理索引 mesh->face->indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// 处理材质
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

unsigned int TextureFromFile(const char* path, const std::string& directory);

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // 如果纹理还没有被加载，则加载它
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // 添加到已加载的纹理中
		}
	}
	return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory)//bool gamma
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

//#ifndef MODEL_H
//#define MODEL_H
//
//#include <glad/glad.h> 
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#include "Mesh.h"
//#include "Shader.h"
//
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <map>
//#include <vector>
//using namespace std;
//
//unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
//
//unsigned int loadTextureM(char const* path, bool gammaCorrection)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum internalFormat;
//        GLenum dataFormat;
//        if (nrComponents == 1)
//        {
//            internalFormat = dataFormat = GL_RED;
//        }
//        else if (nrComponents == 3)
//        {
//            //internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
//            internalFormat = GL_RGB;
//            dataFormat = GL_RGB;
//        }
//        else if (nrComponents == 4)
//        {
//            //internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
//            internalFormat = GL_RGBA;
//            dataFormat = GL_RGBA;
//        }
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        //指定纹理目标 多级渐远纹理级别 存储格式 宽度 高度 历史遗留0 格式 数据类型 数据 
//        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}
//
//class Model
//{
//public:
//    // model data 
//    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
//    vector<Mesh>    meshes;
//    string directory;
//    bool gammaCorrection;
//
//    // constructor, expects a filepath to a 3D model.
//    Model(string const& path, bool gamma = false) : gammaCorrection(gamma)
//    {
//        loadModel(path);
//    }
//
//    // draws the model, and thus all its meshes
//    void Draw(Shader& shader)
//    {
//        for (unsigned int i = 0; i < meshes.size(); i++)
//            meshes[i].Draw(shader);
//    }
//
//private:
//    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
//    void loadModel(string const& path)
//    {
//        // read file via ASSIMP
//        Assimp::Importer importer;
//        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
//        // check for errors
//        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
//        {
//            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
//            return;
//        }
//        // retrieve the directory path of the filepath
//        directory = path.substr(0, path.find_last_of('/'));
//
//        // process ASSIMP's root node recursively
//        processNode(scene->mRootNode, scene);
//    }
//
//    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
//    void processNode(aiNode* node, const aiScene* scene)
//    {
//        // process each mesh located at the current node
//        for (unsigned int i = 0; i < node->mNumMeshes; i++)
//        {
//            // the node object only contains indices to index the actual objects in the scene. 
//            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
//            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//            meshes.push_back(processMesh(mesh, scene));
//        }
//        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
//        for (unsigned int i = 0; i < node->mNumChildren; i++)
//        {
//            processNode(node->mChildren[i], scene);
//        }
//
//    }
//
//    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
//    {
//        // data to fill
//        vector<Vertex> vertices;
//        vector<unsigned int> indices;
//        vector<Texture> textures;
//
//        // walk through each of the mesh's vertices
//        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//        {
//            Vertex vertex;
//            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
//            // positions
//            vector.x = mesh->mVertices[i].x;
//            vector.y = mesh->mVertices[i].y;
//            vector.z = mesh->mVertices[i].z;
//            vertex.Position = vector;
//            // normals
//            if (mesh->HasNormals())
//            {
//                vector.x = mesh->mNormals[i].x;
//                vector.y = mesh->mNormals[i].y;
//                vector.z = mesh->mNormals[i].z;
//                vertex.Normal = vector;
//            }
//            // texture coordinates
//            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
//            {
//                glm::vec2 vec;
//                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
//                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
//                vec.x = mesh->mTextureCoords[0][i].x;
//                vec.y = mesh->mTextureCoords[0][i].y;
//                vertex.TexCoords = vec;
//                // tangent
//                vector.x = mesh->mTangents[i].x;
//                vector.y = mesh->mTangents[i].y;
//                vector.z = mesh->mTangents[i].z;
//                vertex.Tangent = vector;
//                // bitangent
//                vector.x = mesh->mBitangents[i].x;
//                vector.y = mesh->mBitangents[i].y;
//                vector.z = mesh->mBitangents[i].z;
//                vertex.Bitangent = vector;
//            }
//            else
//                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//
//            vertices.push_back(vertex);
//        }
//        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
//        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//        {
//            aiFace face = mesh->mFaces[i];
//            // retrieve all indices of the face and store them in the indices vector
//            for (unsigned int j = 0; j < face.mNumIndices; j++)
//                indices.push_back(face.mIndices[j]);
//        }
//        // process materials
//        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
//        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
//        // Same applies to other texture as the following list summarizes:
//        // diffuse: texture_diffuseN
//        // specular: texture_specularN
//        // normal: texture_normalN
//
//        //1. diffuse maps
//        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//        // 2. specular maps
//        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//        // 3. normal maps
//        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
//        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
//        // 4. height maps
//        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
//        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
//
//        //gun_model
//        //stbi_set_flip_vertically_on_load(false);
//
//        //int albedoMap = loadTextureM("./Gun/Textures/Cerberus_A.tga", true);
//        //Texture albedoMaps;
//        //albedoMaps.id = albedoMap;
//        //albedoMaps.type = "albedoMap";
//        //albedoMaps.path = "./Gun/Textures/Cerberus_A.tga";
//        //textures.push_back(albedoMaps);
//
//        //int normalMap = loadTextureM("./Gun/Textures/Cerberus_N.tga", true);
//        //Texture normalMaps;
//        //normalMaps.id = normalMap;
//        //normalMaps.type = "normalMap";
//        //normalMaps.path = "./Gun/Textures/Cerberus_N.tga";
//        //textures.push_back(normalMaps);
//
//        //int metallicMap = loadTextureM("./Gun/Textures/Cerberus_M.tga", true);
//        //Texture metallicMaps;
//        //metallicMaps.id = metallicMap;
//        //metallicMaps.type = "metallicMap";
//        //metallicMaps.path = "./Gun/Textures/Cerberus_M.tga";
//        //textures.push_back(metallicMaps);
//
//        //int roughnessMap = loadTextureM("./Gun/Textures/Cerberus_R.tga", true);
//        //Texture roughnessMaps;
//        //roughnessMaps.id = roughnessMap;
//        //roughnessMaps.type = "roughnessMap";
//        //roughnessMaps.path = "./Gun/Textures/Cerberus_R.tga";
//        //textures.push_back(roughnessMaps);
//
//        //int aoMap = loadTextureM("./Gun/Textures/Raw/Cerberus_AO.tga", true);
//        //Texture aoMaps;
//        //aoMaps.id = aoMap;
//        //aoMaps.type = "aoMap";
//        //aoMaps.path = "./Gun/Textures/Raw/Cerberus_AO.tga";
//        //textures.push_back(aoMaps);
//        //// return a mesh object created from the extracted mesh data
//        //return Mesh(vertices, indices, textures);
//    }
//
//    // checks all material textures of a given type and loads the textures if they're not loaded yet.
//    // the required info is returned as a Texture struct.
//    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
//    {
//        vector<Texture> textures;
//        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//        {
//            aiString str;
//            mat->GetTexture(type, i, &str);
//            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//            bool skip = false;
//            for (unsigned int j = 0; j < textures_loaded.size(); j++)
//            {
//                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
//                {
//                    textures.push_back(textures_loaded[j]);
//                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
//                    break;
//                }
//            }
//            if (!skip)
//            {   // if texture hasn't been loaded already, load it
//                Texture texture;
//                texture.id = TextureFromFile(str.C_Str(), this->directory);
//                texture.type = typeName;
//                texture.path = str.C_Str();
//                textures.push_back(texture);
//                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
//            }
//        }
//        return textures;
//    }
//};
//
//
//unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
//{
//    string filename = string(path);
//    filename = directory + '/' + filename;
//
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}
//#endif
