#ifndef _ANIMATION_IMPORTER_H_
#define _ANIMATION_IMPORTER_H_
#include "Globals.h"

class ResourceAnimation;
class ResourceAnimationManager;

struct Channel;
class aiAnimation;


namespace AnimationImporter
{
	void Init();
	void CleanUp();

	void Import(const aiAnimation* aianimation, ResourceAnimation* animation);
	uint64 Save(ResourceAnimation* animation, char** fileBuffer);
	void SaveChannel(Channel& channel, char** cursor);
	bool Load(char* fileBuffer, ResourceAnimation* animation, uint size);
	void LoadChannel(Channel& channel, char** cursor);
}

namespace AnimationManagerImporter
{
	uint64 Save(ResourceAnimationManager* animationmanager, char** buffer);
	bool Load(char* buffer, ResourceAnimationManager* animationmanager, uint size);
}

#endif // !_ANIMATION_IMPORTER_H_