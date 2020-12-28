#pragma once
#include "Resource.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <map>

struct VecKey
{
	double veck_time = 0;
	float3 veck_value = float3::zero;
};

struct QuatKey
{
	double quatk_time = 0;
	Quat quatk_value = Quat::identity;
};

struct Channel
{
	std::string chan_Name;

	// Transforms
	uint chan_NumPosKeys = 0;
	std::vector<VecKey> chan_PosKeys;

	uint chan_NumScaleKeys = 0;
	std::vector<VecKey> chan_ScaleKeys;

	uint chan_NumRotKeys = 0;
	std::vector<QuatKey> chan_RotKeys;
};

class ResourceAnimation : public Resource
{
public:
	ResourceAnimation(uint UID);
	~ResourceAnimation();

public:
	float anim_Duration;
	float anim_TicksPerSecond;

	// Channels list
	uint anim_NumChannels;
	std::vector<Channel> anim_Channels;
};
