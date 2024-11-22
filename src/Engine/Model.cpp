#include "Model.hpp"

#include <ctype.h>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <stdlib.h>

Model::Model(const std::string& modelFilepath)
	: rendererID_(0)
	, filePath_(modelFilepath)
	, va_()
	, ib_(nullptr, 0)
{

	std::vector<f32> vertex_positions;
	std::vector<f32> vertex_uvs;
	std::vector<f32> vertex_normals;
	std::vector<f32> faces;

	std::ifstream t(modelFilepath);
	std::string s;
	i32 line_number = 0;
	std::string line[5];
	while (getline(t, s))
	{
		u32 offset = 0;
		i32 i;
		for (i = 0; i < 5; i++)
		{
			if (offset > s.size()) break;
			u32 token_length = 0;
			while (offset + token_length < s.size() && !isspace(s[offset + token_length]) || '/' == s[offset + token_length]) token_length++;
			line[i] = s.substr(offset, offset + token_length);
			offset += token_length + 1;
		}

		if (line[0] == "\0")
		{
			// blank line
		}
		else if (line[0] == "mtllib")
		{
			// TODO external material file
		}
		else if (line[0] == "usemtl")
		{
			// TODO switch material
		}
		else if (line[0] == "#")
		{
			// comment
		}
		else if (line[0] == "g")
		{
			// group name
		}
		else if (line[0] == "o")
		{
			// object name
		}
		else if (line[0] == "vp")
		{
			// TODO begin curve
		}
		else if (line[0] == "s")
		{
			// TODO smooth shading enable/disabled
		}
		else if (line[0] == "u")
		{
			// object name
		}
		else if (line[0] == "l")
		{
			// object name
		}
		else if (line[0] == "v")
		{
			for (i32 j = 1; j < i; j++)
			{
				vertex_positions.push_back((f32)atof(line[j].c_str()));
			}
		}
		else if (line[0] == "vt")
		{
			for (i32 j = 1; j < i; j++)
			{
				vertex_uvs.push_back((f32)atof(line[j].c_str()));
			}
		}
		else if (line[0] == "vn")
		{
			for (i32 j = 1; j < i; j++)
			{
				vertex_normals.push_back((f32)atof(line[j].c_str()));
			}
		}
		else if (line[0] == "f")
		{
			for (i32 j = 1; j < i; j++)
			{
				faces.push_back((f32)atof(line[j].c_str()));
			}
		}
		else
		{
			std::cout << "OBJ element not recognized '" << line[0] << "'" << std::endl;
			ASSERT(false);
		}
	}
	std::cout << vertex_positions.size() << " " << vertex_uvs.size() << " " << vertex_normals.size() << std::endl;
	exit(0);

	VertexBufferLayout layout;
	layout.Push<f32>(3);	// vertex position
	layout.Push<f32>(2);	// vertex uv
	layout.Push<f32>(3);	// vertex normal

	// TODO generate vertices on model from parsed vertex information

	va_.Bind();
}

Model::~Model()
{

}

VertexArray Model::getVertexArray() const
{
	return va_;
}

i32 Model::getVertexCount() const
{
	return vertex_count_;
}
