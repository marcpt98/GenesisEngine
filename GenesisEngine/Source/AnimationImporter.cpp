#include "AnimationImporter.h"

#include "Timer.h"
#include "ResourceAnimation.h"

#include "Assimp/Assimp/include/cimport.h"
#include "Assimp/Assimp/include/scene.h"
#include "Assimp/Assimp/include/postprocess.h"

void AnimationImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

void AnimationImporter::CleanUp()
{
	aiDetachAllLogStreams();
}

void AnimationImporter::Import(const aiAnimation* aianimation, ResourceAnimation* animation)
{
	Timer timer;
	timer.Start();


	LOG("Animation imported in %d ms", timer.Read());
}

uint64 AnimationImporter::Save(ResourceAnimation* animation, char** fileBuffer)
{
	uint size = 0;

	return size;
}

bool AnimationImporter::Load(char* fileBuffer, ResourceAnimation* animation, uint size)
{
	bool ret = true;

	Timer timer;
	timer.Start();



	return ret;
}