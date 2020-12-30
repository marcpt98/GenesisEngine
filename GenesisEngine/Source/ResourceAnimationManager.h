#pragma once
#include "Resource.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include "ResourceAnimation.h"

class ResourceAnimationManager : public Resource
{
public:
	ResourceAnimationManager(uint UID);
	~ResourceAnimationManager();

public:
	void AddAnimation(uint64 UID);

	std::vector<ResourceAnimation> animations;
	
};
