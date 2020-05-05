#pragma once
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>

#include<iostream>

using namespace std;

/*========================================
	ABBREVIATIONS

	VUN = Vertices / UVs / Normals
==========================================*/

class Model {
	std::vector< glm::vec3 > originalVertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
public:
	std::vector< glm::vec3 > baseVertices;
	
	Model(const char* fileLocation);
	void setLocation(float x, float y, float z);
};

bool internalLoadOBJ(
	const char * path,							//path stores the location of the model
	std::vector < glm::vec3 > & out_vertices,	//number of vertices of the model
	std::vector < glm::vec2 > & out_uvs,		//number of uv of the model
	std::vector < glm::vec3 > & out_normals) {	//number of normals of the model

	//stores the index values of VUN
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;

	//temporary values for VUN
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	//importing the model from 'path'
	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("File not found at location\n");
		return false;
	}

	//reading the data stored within the obj file
	char lineHeader[128];
	while (int res = fscanf(file, "%s", lineHeader) != EOF) {
		
		//reading the V data
		if (!strcmp(lineHeader, "v")) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		//reading the U data 
		else if (!strcmp(lineHeader, "vt")) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		//reading the N data
		else if (!strcmp(lineHeader, "vn")) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		//intertwining the VUN data of the model
		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("This file is corrupted..");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	//processing the data read from the obj file
	//for each vertex of each triangle in the model
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];		//the index of the vertex is assigned
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];	//the model is mapped vertex by vertex
		out_vertices.push_back(vertex);						//the finished vertices are pushed to 'out_vertices'
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];		//the index of the vertex is assigned
		glm::vec2 uv = temp_uvs[uvIndex - 1];	//the model is mapped vertex by vertex
		out_uvs.push_back(uv);						//the finished vertices are pushed to 'out_vertices'
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];		//the index of the vertex is assigned
		glm::vec3 normal = temp_normals[normalIndex - 1];	//the model is mapped vertex by vertex
		out_normals.push_back(normal);						//the finished vertices are pushed to 'out_vertices'
	}

}

Model::Model(const char* fileLocation) {
	internalLoadOBJ(fileLocation, originalVertices, uvs, normals);
	setLocation(0.0f, 0.0f, 0.0f);
}

void Model::setLocation(float x, float y, float z) {

	baseVertices.clear();

	for (int i = 0; i < originalVertices.size(); i++) {

		glm::vec3 vertex;
		vertex.x = originalVertices[i].x + x;
		vertex.y = originalVertices[i].y + y;
		vertex.z = originalVertices[i].z + z;

		baseVertices.push_back(vertex);

	}

}


