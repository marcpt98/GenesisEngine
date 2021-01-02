#pragma once
#include "Resource.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "GnJSON.h"
#include "parson\parson.h"
#include "ResourceAnimation.h"

class ResourceAnimationManager : public Resource
{
public:
	ResourceAnimationManager(uint UID);
	~ResourceAnimationManager();

	uint SaveMeta(GnJSONObj& base_object, uint last_modification) override;

public:
	void AddAnimation(uint64 UID);

	std::vector<uint> animations;
	
};
