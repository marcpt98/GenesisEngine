#ifndef _ANIMATION_IMPORTER_H_
#define _ANIMATION_IMPORTER_H_
#include "Globals.h"

class ResourceAnimation;

struct Channel;
class aiAnimation;


namespace AnimationImporter
{
	void Init();
	void CleanUp();

	void Import(const aiAnimation* aianimation, ResourceAnimation* animation);
	uint64 Save(ResourceAnimation* animation, char** fileBuffer);
	void SaveChannel(const Channel& channel, char** cursor);
	bool Load(char* fileBuffer, ResourceAnimation* animation, uint size);
}

#endif // !_ANIMATION_IMPORTER_H_