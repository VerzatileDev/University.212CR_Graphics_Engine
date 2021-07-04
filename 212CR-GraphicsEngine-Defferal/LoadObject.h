#pragma once
//STD LIBS
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>


//OPENGL MATH LIBS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>


#include "Vertex.h"
#include <GL/glew.h> // for Glint

static std::vector<VertexWithAll> loadOBJ(const char* file_name)
{

	//vertex portions
	std::vector<glm::fvec3> vertex_positions;
	std::vector<glm::fvec2> vertex_textcoords;
	std::vector<glm::fvec3> vertex_normals;

	//vectors face
	std::vector<GLint> vertex_position_indicies;
	std::vector<GLint> vertex_textcoord_indicies;
	std::vector<GLint> vertex_normal_indicies;

	//VERTEX ARRAY
	std::vector<VertexWithAll> vertices;

	std::stringstream ss;
	std::ifstream in_file(file_name);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glinit = 0;

	//file open error check
	if (!in_file.is_open())
	{
		throw "ERROR :: READING SPECIFIED OBJECT FILE", file_name, "FAIED";
	}

	while (std::getline(in_file, line))
	{
		//get the prefix of each line
		ss.clear();
		ss.str(line);
		ss >> prefix;


		if (prefix == "#")
		{

		}

		else if (prefix == "o")
		{

		}

		else if (prefix == "s")
		{

		}

		else if (prefix == "use_mtl")
		{

		}

		else if (prefix == "v")//VERTEX POSITION
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_positions.push_back(temp_vec3);
		}

		else if (prefix == "vt")//VERTEX POSITION
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_textcoords.push_back(temp_vec2);
		}

		else if (prefix == "vn")//VERTEX POSITION
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normals.push_back(temp_vec3);
		}

		else if (prefix == "f")
		{
			int counter = 0;
			while (ss >> temp_glinit)
			{
				//pushing indicies into correct arrays
				if (counter == 0)
					vertex_position_indicies.push_back(temp_glinit);
				else if (counter == 1)
					vertex_textcoord_indicies.push_back(temp_glinit);
				else if (counter == 2)
					vertex_normal_indicies.push_back(temp_glinit);

				//handling characters
				if (ss.peek() == '/')
				{
					++counter;
					ss.ignore(1, '/');
				}

				else if (ss.peek() == ' ')
				{
					++counter;
					ss.ignore(1, ' ');
				}

				//reset the counter
				if (counter > 2)
					counter = 0;
			}
		}
		else
		{
		}

	}

	//build final vertex array (mesh)
	vertices.resize(vertex_position_indicies.size(), VertexWithAll());

	//loading in all indices
	for (size_t i = 0; i < vertices.size(); i++)
	{

		vertices[i].position = vertex_positions[vertex_position_indicies[i] - 1];
		vertices[i].textcoord = vertex_textcoords[vertex_textcoord_indicies[i] - 1];
		vertices[i].normal = vertex_normals[vertex_normal_indicies[i] - 1];
		vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
	}

	// Let Us know Object Vertices :Amount: and if succesful in loading files.
	std::cout << file_name <<"Supplied with " << vertices.size() << " vertices" <<"\n";
	std::cout << " Loading ofObject(s) was succesful.. " << "\n";
	return vertices;
}