#ifndef MODEL_H
#define MODEL_H
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"
#include "Mesh.h"

//import a model and translate it to my own structure
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//ASSIMP abstract neatly all the technical details of loading all the different file formats 
//and does all this with a single one-liner.

#include <fstream>
#include <sstream>
#include <map>

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false) {
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1) format = GL_RED;
		else if (nrComponents == 3) format = GL_RGB;
		else if (nrComponents == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}



class Model
{
public:
	//model data
	vector<Texture> textures_loaded; 
	//└stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;

	//constructor
	Model(string const &path, bool gamma = false) : gammaCorrection(gamma) {
		//path: a file location
		loadModel(path);
	}

	//draw the model
	void Draw(Shader& shader) {
		//loops over each of the meshes to call their respective Draw()
		for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].Draw(shader);
	}
	

	



private:
	//load the model data into a data structure of ASSIMP-scene obj.(the route obj of ASSIMP's data interface)
	//have the scene obj -> can access all the data from the laded model.
	void loadModel(string path) {
		//read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace );
		/*param1: file pathe
		* param2: several post-processing options
			└ASSIMP can calculations/operations on the imported data.
			* aiProcess_Triangulate: transform all the model's privitive shapes to triangles
			  (if the model does not entirely consist of triangles)
			* aiProcess_FlipUVs: flip the texture coordinates on the y-aixs
			  (the most images in OpenGL were reversed around the y-axis)
			* aiProcess_GenNormals: create normal vectors for each vertex(if the model doen't contain it)
			* aiProcess_SplitLargeMeshes: split large meshes into smaller sub-meshes
			* aiProcess_OptimizeMeshes: several small meshes into one larger mesh(reducing drawing for optimization)
			refer) http://assimp.sourceforge.net/lib_html/postprocess_8h.html */

		//after load the model, check error
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			//└check if the scene or the root node of the scene are null and check its flags return incomplete data
			cout << "(Model.h 104)★ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}
		//directory path of the given file path
		directory = path.substr(0, path.find_last_of('/'));
		
		processNode(scene->mRootNode, scene);
	}


	//ASSIMP's structure: each node contains a set of mesh index that points to a specific mesh in the secne object.
	//retreive these mesh indices->retrueve each mesh->process each mesh->do this all again for each of the node's children nodes.
	//
	//recursive function process each node until all nodes have been processed
	void processNode(aiNode* node, const aiScene* scene) {
		//process all the node's meshes 
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			//first check each of the node's mesh index 
			//and retrieve the mesh by indexing the scene's mMeshes[].
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			//the returned mesh is passed to the processMesh()
			/*processMesh(): returns a mesh object that we can store in the meshes list/vector*/
			meshes.push_back(processMesh(mesh, scene));
		}

		//then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++) processNode(node->mChildren[i], scene);
	}


	//Assimp -> Mesh
	/*processing a mesh: 
	* 1.retrieve al the vertex data
	* 2.retrieve the mesh's indices
	* 3.retrieve the relevant material data
	* the processed data is stored in one of the 3vectors and from those a Mesh is created and returned.
	*/
	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		//data to fill
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		//repeated for each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
												//└length of vertices in the mesh
			//retrieve the vertex data
			Vertex vertex;
			glm::vec3 vector;
			/*define Assimp's data to vec3 to convert glm's data types.
			--declare a placeholder vector since ASSIMP uses its own vector class 
			--that dosen't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.*/
			//positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			//normals
			if(mesh->HasNormals()) {
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}

			//texture coordinates (if mesh contain tex-coor)
			if (mesh->mTextureCoords[0]) {
				glm::vec2 vec;
				/*a vertex can contain 0~8 different t-c.
				--now only care about the first set of t-c
				--so always take the first set 0 */
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
				//tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;
				//bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
			}
			else vertex.TexCoords = glm::vec2(.0f, .0f);

			vertices.push_back(vertex);
		}//-----------------------------------------------┘for()

		/*index
		* Assimp's interface defines each mesh as having an array of faces
		--each face represents a single primitive, which in this case are always triangles. (--aoProcess_Triangulate option)
		* A face contains the index of vertex we need to draw in what order for its primitive.*/
		//to store all the face's indices in the vector
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];

			//retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
		}

		/*process material 
		To retrieve the material of a mesh, we need to index the scene's mMaterials[] array.
		The mesh's material index is set in its mMaterialIndex property(--we can check with property if the mesh contains a material).
		*/
		if (mesh->mMaterialIndex >= 0) {
			//process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			//we assume a convention for sampler names in the shaders.
			//Each diffuse texture should be named as 'texture_diffuseN' where N is a sequential number ranging from 1~MAX_SAMPLER_NUMBER.
			//diffuse : texture_diffuseN
			//specular : texture_specularN
			//normal : texture_normalN
			
			//1.diffuse maps
			vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			//2.specular maps
			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			//3.normal maps
			std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			//4.height maps
			std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

			//return a mesh object created from the resultant mesh data
			return Mesh(vertices, indices, textures);
		}
	}


	//iterates over all the texture locations of the given texture type, 
	//retrieves the texture's file location and then loads and generates the texture
	//and stores the information in a Vertex struct
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
											//└check the amount of textures stored in the material
			aiString str;
			mat->GetTexture(type, i, &str);
					//└retrieve each of the texture's file location(stores the result in an aiString)
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++) {
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}

			if (!skip) {
				//if texture hasn't been loaded already, load it
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory);
				//└loads a texture with "stb_image.h"
				texture.type = typeName;
				texture.path = str.C_Str();//assumption that texture file paths in model files are local to the actual model oject
				textures.push_back(texture);
				textures_loaded.push_back(texture); 
				//└store it as texture loaded for entire model, to won't unnecessary load duplicate textures.
			}			
		}
		return textures;
	}
};
#endif // !MODEL_H
