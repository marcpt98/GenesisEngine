#include "ResourceAnimation.h"

ResourceAnimation::ResourceAnimation(uint UID) : Resource(UID, ResourceType::RESOURCE_ANIMATION)
{
	anim_Name = "no_name";

	anim_Duration = 0;
	anim_TicksPerSecond = 0;

	anim_NumChannels = 0;
}

ResourceAnimation::~ResourceAnimation()
{

}