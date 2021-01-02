#include "ResourceAnimationManager.h"
#include "GnJSON.h"
#include "Application.h"

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

uint ResourceAnimationManager::SaveMeta(GnJSONObj& base_object, uint last_modification)
{
	base_object.AddInt("UID", _uid);
	base_object.AddString("Library path", App->resources->GetLibraryPath(_uid));
	base_object.AddInt("lastModified", last_modification);

	GnJSONArray nodes_array = base_object.AddArray("Nodes");

	return 1;
}