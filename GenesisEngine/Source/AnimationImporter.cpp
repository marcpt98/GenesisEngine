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

		LOG("Channel %s", aianim->mNodeName.C_Str());
		/*LOG("Number of position keys: %d", aianim->mNumPositionKeys);
		LOG("Number of scaling keys: %d", aianim->mNumScalingKeys);
		LOG("Number of rotation keys: %d", aianim->mNumRotationKeys);*/

		for (int j = 0; j < aianim->mNumPositionKeys; j++)
		{
			aiVectorKey poskey = aianim->mPositionKeys[j];
			channel.chan_PosKeys[poskey.mTime] = float3(poskey.mValue.x, poskey.mValue.y, poskey.mValue.z);
		}
		for (int j = 0; j < aianim->mNumScalingKeys; j++)
		{
			aiVectorKey sclkey = aianim->mScalingKeys[j];
			channel.chan_PosKeys[sclkey.mTime] = float3(sclkey.mValue.x, sclkey.mValue.y, sclkey.mValue.z);
		}
		for (int j = 0; j < aianim->mNumRotationKeys; j++)
		{
			aiQuatKey rotkey = aianim->mRotationKeys[j];
			channel.chan_PosKeys[rotkey.mTime] = float3(rotkey.mValue.x, rotkey.mValue.y, rotkey.mValue.z);
		}

		animation->anim_Channels.push_back(channel);
	}

	LOG("Animation %s imported in %d ms", animation->name.c_str(), timer.Read());
}

uint64 AnimationImporter::Save(ResourceAnimation* animation, char** fileBuffer)
{
	uint size = 0;

	// Save animation
	size = sizeof(float) + sizeof(float) + sizeof(uint);

	for (int i = 0; i < animation->anim_Channels.size(); i++)
	{
		Channel channel = animation->anim_Channels[i];

		// Name
		size += sizeof(uint) + channel.chan_Name.size() + sizeof(uint) * 3;

		// Transforms
		size += sizeof(double) * channel.chan_PosKeys.size() + sizeof(float) * channel.chan_PosKeys.size() * 3;
		size += sizeof(double) * channel.chan_ScaleKeys.size() + sizeof(float) * channel.chan_ScaleKeys.size() * 3;
		size += sizeof(double) * channel.chan_RotKeys.size() + sizeof(float) * channel.chan_RotKeys.size() * 4;
	}

	*fileBuffer = new char[size];
	char* cursor = *fileBuffer;

	memcpy(cursor, &animation->anim_Duration, sizeof(float));
	cursor += sizeof(float);

	memcpy(cursor, &animation->anim_TicksPerSecond, sizeof(float));
	cursor += sizeof(float);

	// Save channels
	uint chan_size = animation->anim_Channels.size();
	memcpy(cursor, &chan_size, sizeof(uint));
	cursor += sizeof(uint);

	for (int i = 0; i < animation->anim_Channels.size(); i++)
	{
		Channel channel = animation->anim_Channels[i];

		SaveChannel(channel, &cursor);
	}

	return size;
}

void AnimationImporter::SaveChannel(Channel& channel, char** cursor)
{
	// Name
	uint chan_size = channel.chan_Name.size();
	memcpy(*cursor, &chan_size, sizeof(uint));
	*cursor += sizeof(uint);

	memcpy(*cursor, channel.chan_Name.c_str(), channel.chan_Name.size());
	*cursor += channel.chan_Name.size();

	// Transforms
	uint ranges[3] = { channel.chan_PosKeys.size(), channel.chan_RotKeys.size(), channel.chan_ScaleKeys.size() };
	memcpy(*cursor, ranges, sizeof(uint) * 3);
	*cursor += sizeof(uint) * 3;

	for (std::map<double, float3>::const_iterator it = channel.chan_PosKeys.begin(); it != channel.chan_PosKeys.end(); it++)
	{
		memcpy(*cursor, &it->first, sizeof(double));
		*cursor += sizeof(double);

		memcpy(*cursor, &it->second, sizeof(float) * 3);
		*cursor += sizeof(float) * 3;
	}

	for (std::map<double, float3>::const_iterator it = channel.chan_ScaleKeys.begin(); it != channel.chan_ScaleKeys.end(); it++)
	{
		memcpy(*cursor, &it->first, sizeof(double));
		*cursor += sizeof(double);

		memcpy(*cursor, &it->second, sizeof(float) * 3);
		*cursor += sizeof(float) * 3;
	}

	for (std::map<double, Quat>::const_iterator it = channel.chan_RotKeys.begin(); it != channel.chan_RotKeys.end(); it++)
	{
		memcpy(*cursor, &it->first, sizeof(double));
		*cursor += sizeof(double);

		memcpy(*cursor, &it->second, sizeof(float) * 4);
		*cursor += sizeof(float) * 4;
	}
}

bool AnimationImporter::Load(char* fileBuffer, ResourceAnimation* animation, uint size)
{
	bool ret = true;

	Timer timer;
	timer.Start();

	// Load animation
	char* cursor = fileBuffer;
	uint bytes;

	memcpy(&animation->anim_Duration, cursor, sizeof(float));
	cursor += sizeof(float);

	memcpy(&animation->anim_TicksPerSecond, cursor, sizeof(float));
	cursor += sizeof(float);

	// Load channels
	uint chan_size = 0;
	memcpy(&chan_size, cursor, sizeof(uint));
	cursor += sizeof(uint);

	for (int i = 0; i < chan_size; ++i)
	{
		Channel channel;

		LoadChannel(channel, &cursor);
	}
	LOG("ANIMATION LOADED");
	return ret;
}

void AnimationImporter::LoadChannel(Channel& channel, char** cursor)
{
	uint bytes = 0;

	// Name
	uint name_size = 0;
	memcpy(&name_size, *cursor, sizeof(uint));
	*cursor += sizeof(uint);

	// Transforms
	uint ranges[3];
	memcpy(&ranges, *cursor, sizeof(uint) * 3);
	*cursor += sizeof(uint) * 3;

	for (int i = 0; i < ranges[0]; i++)
	{
		double time;
		memcpy(&time, *cursor, sizeof(double));
		*cursor += sizeof(double);
		float data[3];
		memcpy(&data, *cursor, sizeof(float) * 3);
		*cursor += sizeof(float) * 3;

		channel.chan_PosKeys[time] = float3(data);
	}

	for (int i = 0; i < ranges[1]; i++)
	{
		double time;
		memcpy(&time, *cursor, sizeof(double));
		*cursor += sizeof(double);
		float data[3];
		memcpy(&data, *cursor, sizeof(float) * 3);
		*cursor += sizeof(float) * 3;

		channel.chan_ScaleKeys[time] = float3(data);
	}

	for (int i = 0; i < ranges[2]; i++)
	{
		double time;
		memcpy(&time, *cursor, sizeof(double));
		*cursor += sizeof(double);
		float data[3];
		memcpy(&data, *cursor, sizeof(float) * 4);
		*cursor += sizeof(float) * 4;

		channel.chan_RotKeys[time] = Quat(data);
	}
}

uint64 AnimationManagerImporter::Save(ResourceAnimationManager* animationmanager, char** buffer)
{

	return 0;
}

bool AnimationManagerImporter::Load(char* buffer, ResourceAnimationManager* animationmanager, uint size)
{

	return true;
}