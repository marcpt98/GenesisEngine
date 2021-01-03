#pragma once
#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <vector>;
#include <map>

class ResourceAnimation;
class GameObject;
class Channel;
class Mesh;

class Animation : public Component 
{
public:
	Animation();
	virtual ~Animation();

	virtual void Update() override;
	virtual void Render();
	virtual void OnEditor() override;

	void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;
	void SetResourceUID(uint UID) override;
	Resource* GetResource(ResourceType type) override;

public:
	ResourceAnimation* _resource;

	const char* name;
	char* path;
};