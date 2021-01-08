#pragma once
#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Mesh.h"
#include "Time.h"

#include <vector>;
#include <map>

class ResourceAnimation;
class GameObject;
class Channel;
class GnMesh;

enum Anim_States
{
	ST_IDLE,
	ST_WALK,
	ST_ATTACK
};

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
	void CheckAnimState();
	void PlayAnimation();
	void PlayAnimationTransform();
	void PlayAnimationMesh(GameObject* gameobject);

public:
	ResourceAnimation* _resource;

	const char* name;
	char* path;

	GameObject* rootChannel;
	GnMesh* mesh;
	std::map<std::string, GameObject*> anim_channels;

	std::vector<ResourceAnimation*> animations;

	ResourceAnimation* currentanimation;
	
	Anim_States	actualState;

	uint anim_time;
	float time;

	bool init;
	int animations_size;
	bool attack_anim;
};