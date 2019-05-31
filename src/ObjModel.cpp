#include "ObjModel.h"
#include "MyGL.h"
#include "Shaders.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

FVector3D ParseVert(const string & line)
{
	FVector3D ret;
	stringstream ssLine(line);
	string tmp;
	ssLine >> tmp;
	ssLine >> tmp; //X
	ret.X = stof(tmp);
	ssLine >> tmp; //Y
	ret.Y = stof(tmp);
	ssLine >> tmp; //Z
	ret.Z = stof(tmp);

	return ret;
}

FVector2D ParseUV(const string & line)
{
	FVector2D ret;
	stringstream ssLine(line);
	string tmp;
	ssLine >> tmp;
	ssLine >> tmp; //X
	ret.X = stof(tmp);
	ssLine >> tmp; //Y
	ret.Y = 1.0f - stof(tmp); //flip uv

	return ret;
}

struct ObjFace
{
	IVector3D Verts;
	IVector3D UVs;
};

struct ObjVert
{
	FVector3D Vert;
	FVector2D UV;

	bool operator==(const ObjVert& other) const
	{
		return Vert == other.Vert && UV == other.UV;
	}

	bool operator!=(const ObjVert& other) const
	{
		return !(*this == other);
	}

};

ObjFace ParseFace(const string & line)
{
	const char* FaceFormat = "%d/%d/%d";
	int A, B, C;
	ObjFace ret;
	stringstream ssLine(line);
	string tmp;
	ssLine >> tmp; //f

	ssLine >> tmp; //X
	sscanf(tmp.c_str(), FaceFormat, &A, &B, &C);

	ret.Verts.X = A;
	ret.UVs.X = B;

	ssLine >> tmp; //Y
	sscanf(tmp.c_str(), FaceFormat, &A, &B, &C);

	ret.Verts.Y = A;
	ret.UVs.Y = B;

	ssLine >> tmp; //Z
	sscanf(tmp.c_str(), FaceFormat, &A, &B, &C);

	ret.Verts.Z = A;
	ret.UVs.Z = B;

	ret.Verts -= IVector3D(1, 1, 1);
	ret.UVs -= IVector3D(1, 1, 1);

	return ret;
}

ObjModel::ObjModel(const char * model)
{
	ifstream file;
	file.open(model, ios::in);
	if (file.is_open())
	{
		vector<FVector3D> verts;
		vector<FVector2D> UVs;
		vector<ObjFace> faces;

		string line;
		while (getline(file,line))
		{
			if (line.length() == 0)
				continue;
			
			switch (line[0])
			{
			case 'v':
			case 'V':
				if (line[1] == 't' || line[1] == 'T')
					UVs.push_back(ParseUV(line));
				else
					verts.push_back(ParseVert(line));
				break;

			case 'f':
			case 'F':
				faces.push_back(ParseFace(line));
				break;

			default:
				break;
			}


		}

		file.close();

		//build VertexBuffer and index buffer
		vector<ObjVert> vertexBuffer;
		vector<unsigned int> indicies;
		vertexBuffer.reserve(verts.size());
		indicies.reserve(faces.size());

		for (auto& item : faces) 
		{
			ObjVert A, B, C;
			int index[3];
			A.Vert = verts[item.Verts.X];
			B.Vert = verts[item.Verts.Y];
			C.Vert = verts[item.Verts.Z];

			A.UV = UVs[item.UVs.X];
			B.UV = UVs[item.UVs.Y];
			C.UV = UVs[item.UVs.Z];

			auto FindOrAddVert = [&](const ObjVert& item) {
				for (unsigned int i = 0; i < (int)vertexBuffer.size(); i++)
				{
					if (vertexBuffer[i] == item)
					{
						return i;
					}
				}

				vertexBuffer.push_back(item);
				return (unsigned int)(vertexBuffer.size()-1);
				
			};

			indicies.push_back(FindOrAddVert(A));
			indicies.push_back(FindOrAddVert(B));
			indicies.push_back(FindOrAddVert(C));

		}

		//create GL resource
		unsigned int PositionAttrib = ShaderPrograms::GetVertexAttrib(ShaderProgram::SP_ImageBox, "aPos");
		unsigned int TexAttrib = ShaderPrograms::GetVertexAttrib(ShaderProgram::SP_ImageBox, "aTexCoord");

		const auto VBSize = sizeof(ObjVert)*vertexBuffer.size();
		const auto stride = sizeof(float) * 5;
		const auto TexOffset = sizeof(float) * 3;

		VertexBuffer.InitData((void*)&vertexBuffer[0], VBSize);
		GlVbAttrib Atrribs[] = { { PositionAttrib ,3	,stride ,0 },
								 { TexAttrib	,2	,stride,(void*)TexOffset} };

		VertexBuffer.AddAttrib(Atrribs[0]);
		VertexBuffer.AddAttrib(Atrribs[1]);

		IndexBuffer.InitData((void*)&indicies[0], sizeof(int)*indicies.size());
		GL_ASSERT;

		NumIndicies = indicies.size();
	}
}

ObjModel::~ObjModel()
{

}

void ObjModel::Draw() const
{
	VertexBuffer.Bind();
	IndexBuffer.Bind();

	glDrawElements(
		GL_TRIANGLES,   
		NumIndicies,    
		GL_UNSIGNED_INT,
		(void*)0 
	);
}


