#include "MeshImporter.h"

#include "Timer.h"
#include "ResourceMesh.h"

#include "Assimp/Assimp/include/cimport.h"
#include "Assimp/Assimp/include/scene.h"
#include "Assimp/Assimp/include/postprocess.h"

void MeshImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

void MeshImporter::CleanUp()
{
	aiDetachAllLogStreams();
}

void MeshImporter::Import(const aiMesh* aimesh, ResourceMesh* mesh)
{
	Timer timer;
	timer.Start();

	//vertex copying
	mesh->vertices_amount = aimesh->mNumVertices;
	mesh->vertices = new float[mesh->vertices_amount * 3]();
	memcpy(mesh->vertices, aimesh->mVertices, sizeof(float) * mesh->vertices_amount * 3);
	LOG("%s imported with %d vertices", aimesh->mName.C_Str(), mesh->vertices_amount);

	//indices copying
	if (aimesh->HasFaces())
	{
		mesh->indices_amount = aimesh->mNumFaces * 3;
		mesh->indices = new uint[mesh->indices_amount]();
		LOG("%s imported with %d indices", aimesh->mName.C_Str(), mesh->indices_amount);

		for (size_t f = 0; f < aimesh->mNumFaces; f++)
		{
			if (aimesh->mFaces[f].mNumIndices != 3)
			{
				LOG_WARNING("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&mesh->indices[f * 3], aimesh->mFaces[f].mIndices, 3 * sizeof(uint));
			}
		}
	}

	mesh->texcoords_amount = aimesh->mNumVertices;
	mesh->texcoords = new float[mesh->vertices_amount * 2]();
	mesh->colors = new float[mesh->indices_amount * 4]();

	if (aimesh->HasNormals())
	{
		mesh->normals_amount = aimesh->mNumVertices;
		mesh->normals = new float[aimesh->mNumVertices * 3]();
	}

	//normals, color and texture coordinates
	for (size_t v = 0, n = 0, tx = 0, c = 0; v < aimesh->mNumVertices; v++, n += 3, tx += 2, c += 4)
	{
		//normal copying
		if (aimesh->HasNormals())
		{
			//normal copying
			mesh->normals[n] = aimesh->mNormals[v].x;
			mesh->normals[n + 1] = aimesh->mNormals[v].y;
			mesh->normals[n + 2] = aimesh->mNormals[v].z;
		}

		//texcoords copying
		if (aimesh->mTextureCoords[0])
		{
			mesh->texcoords[tx] = aimesh->mTextureCoords[0][v].x;
			mesh->texcoords[tx + 1] = aimesh->mTextureCoords[0][v].y;
		}
		else
		{
			mesh->texcoords[tx] = 0.0f;
			mesh->texcoords[tx + 1] = 0.0f;
		}

		//color copying
		if (aimesh->HasVertexColors(v))
		{
			mesh->colors[c] = aimesh->mColors[v]->r;
			mesh->colors[c + 1] = aimesh->mColors[v]->g;
			mesh->colors[c + 2] = aimesh->mColors[v]->b;
			mesh->colors[c + 3] = aimesh->mColors[v]->a;
		}
		else
		{
			mesh->colors[c] = 0.0f;
			mesh->colors[c + 1] = 0.0f;
			mesh->colors[c + 2] = 0.0f;
			mesh->colors[c + 3] = 0.0f;
		}
	}

	// Import Bones
	if (aimesh->HasBones())
	{
		mesh->boneSize.resize(aimesh->mNumBones);

		mesh->boneID = aimesh->mNumVertices * 4;
		mesh->boneWeight = aimesh->mNumVertices * 4;

		mesh->boneIDs = new int[mesh->boneID];
		for (int i = 0; i < mesh->boneID; i++)
		{
			mesh->boneIDs[i] = -1;
		}

		mesh->boneWeights = new float[mesh->boneWeight];
		for (int i = 0; i < mesh->boneWeight; i++)
		{
			mesh->boneWeights[i] = .0f;
		}

		for (int i = 0; i < aimesh->mNumBones; i++)
		{
			aiBone* bone = aimesh->mBones[i];
			mesh->boneMapping[bone->mName.C_Str()] = i;

			float4x4 offset = float4x4(bone->mOffsetMatrix.a1, bone->mOffsetMatrix.a2, bone->mOffsetMatrix.a3, bone->mOffsetMatrix.a4,
				bone->mOffsetMatrix.b1, bone->mOffsetMatrix.b2, bone->mOffsetMatrix.b3, bone->mOffsetMatrix.b4,
				bone->mOffsetMatrix.c1, bone->mOffsetMatrix.c2, bone->mOffsetMatrix.c3, bone->mOffsetMatrix.c4,
				bone->mOffsetMatrix.d1, bone->mOffsetMatrix.d2, bone->mOffsetMatrix.d3, bone->mOffsetMatrix.d4);

			mesh->boneOffsets.push_back(offset);

			for (int j = 0; j < bone->mNumWeights; j++)
			{
				int vertexID = bone->mWeights[j].mVertexId;

				for (int k = 0; k < 4; k++)
				{
					if (mesh->boneIDs[vertexID * 4 + k] == -1)
					{
						mesh->boneIDs[vertexID * 4 + k] = i;
						mesh->boneWeights[vertexID * 4 + k] = bone->mWeights[j].mWeight;
						break;
					}
				}
			}
		}
	}

	LOG("Mesh imported in %d ms", timer.Read());
}

uint64 MeshImporter::Save(ResourceMesh* mesh, char** fileBuffer)
{
	uint ranges[6] = { mesh->indices_amount, mesh->vertices_amount, mesh->normals_amount, mesh->texcoords_amount, 
		mesh->boneID, mesh->boneWeight };

	uint size = sizeof(ranges) + sizeof(uint) 
		+ sizeof(uint) * mesh->indices_amount + (sizeof(float) * mesh->vertices_amount * 3)
		+ sizeof(float) * mesh->normals_amount * 3 + (sizeof(float) * mesh->texcoords_amount * 2)
		+ sizeof(int) * mesh->boneID + sizeof(float) * mesh->boneWeight 
		+ sizeof(float) * 16 * mesh->boneOffsets.size() + sizeof(char) * 30 * mesh->boneMapping.size();

	for (std::map<std::string, uint>::const_iterator it = mesh->boneMapping.begin(); it != mesh->boneMapping.end(); ++it)
	{
		size += sizeof(uint);
		size += sizeof(char) * it->first.size();
	}

	*fileBuffer = new char[size];
	char* cursor = *fileBuffer;

	//store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	//store bone offsets
	bytes = sizeof(uint);
	uint bonesSize = mesh->boneOffsets.size();
	memcpy(cursor, &bonesSize, bytes);
	cursor += bytes;

	//store indices
	bytes = sizeof(uint) * mesh->indices_amount;
	memcpy(cursor, mesh->indices, bytes);
	cursor += bytes;

	//store vertices
	bytes = sizeof(float) * mesh->vertices_amount * 3;
	memcpy(cursor, mesh->vertices, bytes);
	cursor += bytes;

	//store normals
	bytes = sizeof(float) * mesh->normals_amount * 3;
	memcpy(cursor, mesh->normals, bytes);
	cursor += bytes;
	

	//store texcoords
	bytes = sizeof(float) * mesh->texcoords_amount * 2;
	memcpy(cursor, mesh->texcoords, bytes);
	cursor += bytes;
	
	// Save Bones
	uint _bytes = 0;

	if (mesh->boneID > 0)
	{
		_bytes = sizeof(int) * mesh->boneID;
		memcpy(cursor, mesh->boneIDs, _bytes);
		cursor += _bytes;
	}

	if (mesh->boneWeight > 0)
	{
		_bytes = sizeof(float) * mesh->boneWeight;
		memcpy(cursor, mesh->boneWeights, _bytes);
		cursor += _bytes;
	}

	if (mesh->boneOffsets.size() > 0)
	{
		for (int i = 0; i < mesh->boneOffsets.size(); ++i)
		{
			_bytes = sizeof(float) * 16;
			memcpy(cursor, mesh->boneOffsets[i].ptr(), _bytes);
			cursor += _bytes;
		}
	}

	for (int i = 0; i < mesh->boneMapping.size(); i++)
	{
		for (std::map<std::string, uint>::const_iterator it = mesh->boneMapping.begin(); it != mesh->boneMapping.end(); it++)
		{
			if (it->second == i)
			{
				_bytes = sizeof(uint);
				uint string_size = it->first.size();
				memcpy(cursor, &string_size, _bytes);
				cursor += _bytes;

				_bytes = sizeof(char) * string_size;
				memcpy(cursor, it->first.c_str(), _bytes);
				cursor += _bytes;
			}
		}
	}

	return size;
}

bool MeshImporter::Load(char* fileBuffer, ResourceMesh* mesh, uint size)
{
	bool ret = true;

	Timer timer;
	timer.Start();

	char* cursor = fileBuffer;

	uint ranges[6];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	// Bones
	bytes = sizeof(uint);
	uint bonesSize = 0;
	memcpy(&bonesSize, cursor, bytes);
	cursor += bytes;

	mesh->boneSize.resize(bonesSize);
	mesh->boneOffsets.resize(bonesSize);

	mesh->indices_amount = ranges[0];
	mesh->vertices_amount = ranges[1];
	mesh->normals_amount = ranges[2];
	mesh->texcoords_amount = ranges[3];
	mesh->boneID = ranges[4];
	mesh->boneWeight = ranges[5];

	// Load indices
	bytes = sizeof(uint) * mesh->indices_amount;
	mesh->indices = new uint[mesh->indices_amount];
	memcpy(mesh->indices, cursor, bytes);
	cursor += bytes;

	//load vertices
	bytes = sizeof(float) * mesh->vertices_amount * 3;
	mesh->vertices = new float[mesh->vertices_amount * 3];
	memcpy(mesh->vertices, cursor, bytes);
	cursor += bytes;

	//load normals
	bytes = sizeof(float) * mesh->normals_amount * 3;
	mesh->normals = new float[mesh->normals_amount * 3];
	memcpy(mesh->normals, cursor, bytes);
	cursor += bytes;

	//load texcoords
	bytes = sizeof(float) * mesh->texcoords_amount * 2;
	mesh->texcoords = new float[mesh->texcoords_amount * 2];
	memcpy(mesh->texcoords, cursor, bytes);
	cursor += bytes;

	//load bones
	uint _bytes = 0;

	if (mesh->boneID > 0)
	{
		_bytes = sizeof(int) * mesh->boneID;
		mesh->boneIDs = new int[mesh->boneID];
		memcpy(mesh->boneIDs, cursor, _bytes);
		cursor += _bytes;
	}

	if (mesh->boneWeight > 0)
	{
		_bytes = sizeof(float) * mesh->boneWeight;
		mesh->boneWeights = new float[mesh->boneWeight];
		memcpy(mesh->boneWeights, cursor, _bytes);
		cursor += _bytes;
	}

	float matrix[16];
	for (uint i = 0; i < mesh->boneOffsets.size(); ++i)
	{
		_bytes = sizeof(float) * 16;
		memcpy(matrix, cursor, _bytes);
		cursor += _bytes;

		float4x4 offset;
		offset.Set(matrix);
		mesh->boneOffsets[i] = offset;
	}

	char name[30];
	for (uint i = 0; i < mesh->boneSize.size(); ++i)
	{
		_bytes = sizeof(uint);
		uint stringSize = 0;
		memcpy(&stringSize, cursor, _bytes);
		cursor += _bytes;

		_bytes = sizeof(char) * stringSize;
		memcpy(name, cursor, _bytes);
		cursor += _bytes;

		name[stringSize] = '\0';
		std::string str(name);
		mesh->boneMapping[str.c_str()] = i;
	}

	mesh->GenerateBuffers();

	LOG("%s loaded in %d ms", mesh->libraryFile.c_str(), timer.Read());

	return ret;
}