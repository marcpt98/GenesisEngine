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

	// General
	animation->name = aianimation->mName.C_Str();
	animation->anim_Duration = aianimation->mDuration;
	animation->anim_TicksPerSecond = aianimation->mTicksPerSecond;
	animation->anim_NumChannels = aianimation->mNumChannels;

	LOG("Starting import of %s animation", animation->name.c_str());
	LOG("Number of channels: %d", animation->anim_NumChannels);

	// Every channel
	for (int i = 0; i < aianimation->mNumChannels; i++)
	{
		aiNodeAnim* aianim = aianimation->mChannels[i];
		Channel channel;
		
		// Name
		channel.chan_Name = aianim->mNodeName.C_Str();

		// Transforms
		channel.chan_NumPosKeys = aianim->mNumPositionKeys;
		channel.chan_NumScaleKeys = aianim->mNumScalingKeys;
		channel.chan_NumRotKeys = aianim->mNumRotationKeys;

		LOG("Channel %s: ", aianim->mNodeName.C_Str());
		LOG("Number of position keys: %d", aianim->mNumPositionKeys);
		LOG("Number of scaling keys: %d", aianim->mNumScalingKeys);
		LOG("Number of rotation keys: %d", aianim->mNumRotationKeys);

		for (int j = 0; j < aianim->mNumPositionKeys; j++)
		{
			aiVectorKey poskey = aianim->mPositionKeys[j];
			VecKey veckey;

			veckey.veck_time = poskey.mTime;
			veckey.veck_value = float3(poskey.mValue.x, poskey.mValue.y, poskey.mValue.z);

			channel.chan_PosKeys.push_back(veckey);
		}
		for (int j = 0; j < aianim->mNumScalingKeys; j++)
		{
			aiVectorKey sclkey = aianim->mScalingKeys[j];
			VecKey veckey;

			veckey.veck_time = sclkey.mTime;
			veckey.veck_value = float3(sclkey.mValue.x, sclkey.mValue.y, sclkey.mValue.z);

			channel.chan_ScaleKeys.push_back(veckey);
		}
		for (int j = 0; j < aianim->mNumRotationKeys; j++)
		{
			aiQuatKey rotkey = aianim->mRotationKeys[j];
			QuatKey quatkey;

			quatkey.quatk_time = rotkey.mTime;
			quatkey.quatk_value = Quat(rotkey.mValue.x, rotkey.mValue.y, rotkey.mValue.z, rotkey.mValue.w);

			channel.chan_RotKeys.push_back(quatkey);
		}

		animation->anim_Channels.push_back(channel);
	}

	LOG("Animation %s imported in %d ms", animation->name.c_str(), timer.Read());
}

uint64 AnimationImporter::Save(ResourceAnimation* animation, char** fileBuffer)
{
	uint size = 0;

	// Save animation

	// Save channels

	return size;
}

bool AnimationImporter::Load(char* fileBuffer, ResourceAnimation* animation, uint size)
{
	bool ret = true;

	Timer timer;
	timer.Start();

	// Load animation

	// Load channels

	return ret;
}