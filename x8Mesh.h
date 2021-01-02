/*A Mesh represents a single drawable entity.
* A Mesh should at least need a set of vertices, where each vertex contains 
* a positin vector,
* a normal vector,
* a texture coordinate vector.
* A Mesh sould also contain indices for indexed drawing,
* and material data in the form of textures(diffuse/specular maps).*/

#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include <string>
#include <vector>
using namespace std;

//store each of the vertex attribute in a struct.
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

//store texture's id and its type(diffuse or specular).
struct Texture {
	unsigned int id;
	string type;
};





class Mesh 
{
public:
	//mesh data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	//constructor <- give the mesh all the necessary data
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
		//lists of all required mesh data that I can use for rendering
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		//set the vertex buffers and its attribute pointers.
		setupMesh();
	}
	
	//finally draw the mesh
	/*★by passing the shader to the mesh we can set several uniforms before drawing.
		(like linking samplers to texture units)*/
	void Draw(Shader shader) {
		/*Before rendering the mesh(by calling glDrawElements), bind appropriate textures first
		* To bind texture, we should know how many textures/what type of textures the mesh has.
		* A naming convention to set the texture units and samplers in the shaders
		* like this:
		uniform sampler2D texture_diffuse1;
		uniform sampler2D texture_diffuse2;
		unifrom sampler2D texture_diffuse3;
		unifrom sampler2D texture_specular1;
		unifrom sampler2D textuer_specular2;
		* we can define as many texture samplers as we want in the shader.
		* we can know what texture's name is.*/
		unsigned int diffuseN = 1;
		unsigned int specularN = 1;
		unsigned int normalN = 1;
		unsigned int heightN = 1;

		for (unsigned int i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);//active texture unit before binding 

			//retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = textures[i].type;

			//1.calculate the N-comoponent per texture type 
			//and concatenate it to the texture's type string to get uniform name.
			if (name == "texture_diffuse") number = std::to_string(diffuseN++);
			else if (name == "texture_specular") number = std::to_string(specularN++);
			else if (name == "texture_normal") number = std::to_string(normalN++);
			else if (name == "texture_height") number = std::to_string(heightN++);

			//2.locate the sampler, give it the location value to correspond 
			//with the currently active texture unit, and bind the texture.
			shader.setFloat(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		//draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//After configuration, set everything back to defaults
	}



private:
	//render data
	unsigned int VAO, VBO, EBO;

	//initialize the buffers
	//setup the buffers and specify the vertex shader layout via vertex attribute pointers.
	/*In C++, Structs's property memoray layout is sequential.
	* If I were to represent a struct as an array of data,
	* it would only contain the struct's variables in sequential order
	* which directly traslates to a float(actually byte) array that we want for an array buffer.
	* For example)
	Vertex vertex;
	vertex.Position = glm::vec3(0.2f, 0.4f, 0.6f);
	vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	vertex.TexCoords = glm::vec2(1.0f, 0.0f);
	* ↓ its memoory layout
	* = [0.2f, 0.4f, 0.6f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f];
	* we can directly pass a pointer to a large list of Vertex structs as the buffer's data
	* and they translate to glBufferData()'s argument*/
	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		/*A great thing about structs is that their momory layout is sequential for all its items.
		The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		again translate to 3/2 float which translate to a byte array*/
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		//parameter2 == 32 bytes (8 floats * 4 byte each)

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//vertex position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		/*parameter
		* 1: which vertex attribute.
		*    we specified the location of position vertex attribute in the vertex shader
		*    with [[[ layout (location = 0) ]]]
		*    set the location of the vertex attribute to 0
		*    since we want to pass data to this vertex attribute, we pass in 0.
		*
		* 2: the size of the vertex attribute.
		*    The vertex attributes is a [[[ vec3 ]]] so if is composed of 3 values.
		*
		* 3: the type of the data
		*    GL_FLOAT (a vec* in GLSL consists of floating point values)
		*
		* 4: whether if we want the data to be normalized
		*    GL_TRUE -- the integer data is normalized to 0(or -1) and 1 when converted to float.
		*
		* 5: stride (the space between consecutive vertex attributes)
		*	 --this time,set the stride to Vertex struct's size.
		*
		* 6: the offset of where the position data begins in the buffer
		*/
		//vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		/* parameter6 == 12byte(3 float * 4 byte)
		* (to set the byte offset of the normal vector equal to the byte offset of the normal attribute in the struct.
		* * * offset(strct, name--variable name of the struct)
		* * *   └return the byte offset of that variable from the start of the struct.
		*/
		//vertex texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		//vertex tangent


		glBindVertexArray(0);
	}
//If I want another vertex attribute, I can simply add it to the struct 
//and due to its flexible nature, the rendering code won't break.
};
#endif // !MESH_H
