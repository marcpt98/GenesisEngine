#include "ResourceAnimationManager.h"

ResourceAnimationManager::ResourceAnimationManager(uint UID) : Resource(UID, ResourceType::RESOURCE_ANIMATION_MANAGER)
{
	
}

ResourceAnimationManager::~ResourceAnimationManager()
{

}

void ResourceAnimationManager::AddAnimation(uint64 UID)
{
	animations.push_back(UID);
}