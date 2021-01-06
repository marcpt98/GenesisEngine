#pragma once
#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Mesh.h"
#include "Time.h"

#include <vector>;
#include <map>

class ResourceAnimation;
class ResourceAnimationManager;
class GameObject;
class Channel;
class GnMesh;

class Animation : public Component 
{
public:
	Animation();
	virtual ~Animation();

	void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;
	void SetResourceUID(uint UID) override;
	Resource* GetResource(ResourceType type) override;

	virtual void Update() override;
	virtual void Render();
	virtual void OnEditor() override;

	void InitAnimation();
	void AddAnimations();
	void PlayAnimation();

public:
	ResourceAnimation* _resource;

	const char* name;
	char* path;

	GameObject* rootChannel;
	std::map<std::string, GameObject*> anim_channels;

	std::vector<ResourceAnimation*> animations;

	ResourceAnimation* currentanimation;

	uint anim_time;
	float time;

	bool init;
};