#pragma once
#include "Resource.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <map>

class GnJSONObj;

class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint UID);
	~ResourceMesh();

	void Load(GnJSONObj& base_object);

	void GenerateBuffers();
	void GenerateAnimBuffers();
	void DeleteBuffers();

public:
	uint vertices_buffer = 0;
	uint vertices_amount = 0;
	float* vertices = nullptr;

	uint indices_buffer = 0;
	uint indices_amount = 0;
	uint* indices = nullptr;

	uint normals_buffer = 0;
	uint normals_amount = 0;
	float* normals;

	uint texcoords_buffer = 0;
	uint texcoords_amount = 0;
	float* texcoords = nullptr;

	float* colors = nullptr;

	// Bones
	// Buffer of 4 * nº vertices. 4 bones for each vertex
	int* boneIDs = nullptr;
	int boneID = 0;

	// Influences that each bone has over each vertex
	float* boneWeights = nullptr;
	float boneWeight = 0;

	// A way to relate bone names (Game Object names) with array positions
	std::map<std::string, uint> boneMapping;

	// Transformation from bind pose to bone space
	std::vector<float4x4> boneOffsets;
	std::vector<float4x4> boneSize;

private:
	bool _buffers_created;
};