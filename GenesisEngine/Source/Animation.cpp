#include "Animation.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "GnJSON.h"

#include "ResourceAnimation.h"

#include "glew/include/glew.h"
#include "ImGui/imgui.h"

#include "ResourceAnimation.h"

Animation::Animation() : Component(), name("No name"), _resource(nullptr)
{
	type = ComponentType::ANIMATION;

	name = nullptr;
	path = nullptr;
	rootChannel = nullptr;
	mesh = nullptr;
	currentanimation = nullptr;
	init = false;
	anim_time = 0;
	time = 0;
	animations_size = 3;
	actualState = ST_IDLE;
	attack_anim = false;
	viewbones = false;
}

Animation::~Animation()
{
	if (_resource != nullptr)
	{
		App->resources->ReleaseResource(_resource->GetUID());
		_resource = nullptr;
	}
}

void Animation::InitAnimation()
{
	anim_channels.clear();

	// Get root gameobject with all channels
	rootChannel = _gameObject->children[1]->children[0];
	
	// Get gameobject with mesh
	mesh = (GnMesh*)_gameObject->children[0]->GetComponent(ComponentType::MESH);

	// Create mesh animated
	mesh->CreateAnimMesh();

	// Order gameobjects
	std::vector<GameObject*> channels;
	App->scene->PreorderGameObjects(rootChannel, channels);

	// Pass them to map
	for (int i = 0; i < channels.size(); ++i)
	{
		anim_channels[channels[i]->name] = channels[i];
	}

	currentanimation = _resource;
}

void Animation::Update()
{
	if (init == false)
	{
		InitAnimation();
		init = true;
	}

	// Update animation only on play mode
	if (App->editor->play_anim == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			time = 0;
			actualState = ST_ATTACK;
			attack_anim = true;
		}
		else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
		{
			actualState = ST_WALK;
		}
		else
		{
			actualState = ST_IDLE;
		}

		if (attack_anim == true)
		{
			actualState = ST_ATTACK;
		}

		CheckAnimState();

		// Loop
		if (anim_time > currentanimation->anim_Duration)
		{
			time = 0;
		}

		PlayAnimation();
	}
	
	// Draw Bones
	if (viewbones)
	{
		Render();
	}
}

void Animation::CheckAnimState()
{
	if (actualState == ST_IDLE)
	{
		currentanimation->anim_Duration = 46;

		// Animation time
		time += App->GetLastDt();
		anim_time = currentanimation->anim_TicksPerSecond * time;
	}

	if (actualState == ST_WALK)
	{
		currentanimation->anim_Duration = 70;

		// Animation time
		time += App->GetLastDt();
		anim_time = currentanimation->anim_TicksPerSecond * time;
		anim_time += 48;
	}

	if (actualState == ST_ATTACK)
	{
		currentanimation->anim_Duration = 119;

		// Animation time
		time += App->GetLastDt();
		anim_time = currentanimation->anim_TicksPerSecond * time;
		anim_time += 72;

		if (anim_time == 119)
		{
			attack_anim = false;
		}
	}
}

void Animation::PlayAnimation()
{
	PlayAnimationTransform();
	PlayAnimationMesh(_gameObject->children[0]);
}

void Animation::PlayAnimationTransform()
{
	for (std::map<std::string, GameObject*>::iterator it = anim_channels.begin(); it != anim_channels.end(); it++)
	{
		// Get channel related with gameobject on list
		Channel channel = currentanimation->anim_Channels.find(it->first)->second;

		// Update Transforms
		Transform* transform = (Transform*)it->second->GetComponent(ComponentType::TRANSFORM);

		std::map<double, float3>::const_iterator pos_prev = channel.chan_PosKeys.lower_bound(anim_time);
		if (pos_prev != channel.chan_PosKeys.end())
		{
			transform->SetPosition(pos_prev->second);
		}

		std::map<double, Quat>::const_iterator rot_prev = channel.chan_RotKeys.lower_bound(anim_time);
		if (rot_prev != channel.chan_RotKeys.end())
		{
			transform->SetRotation(rot_prev->second);
		}

		std::map<double, float3>::const_iterator scl_prev = channel.chan_ScaleKeys.lower_bound(anim_time);
		if (scl_prev != channel.chan_ScaleKeys.end())
		{
			transform->SetScale(scl_prev->second);
		}

		// Update matrix
		transform->UpdateMatrix(transform);
	}
}

void Animation::PlayAnimationMesh(GameObject* gameObject)
{
	mesh->DeformMesh();

	for (size_t i = 0; i < gameObject->GetChildrenAmount(); i++)
	{
		PlayAnimationMesh(gameObject->GetChildAt(i));
	}
}

void Animation::Render()
{
	for (std::map<std::string, GameObject*>::const_iterator it = anim_channels.begin(); it != anim_channels.end(); it++)
	{
		for (int i = 0; i < it->second->GetChildrenAmount(); i++)
		{
			float4x4 start = float4x4::zero;
			float4x4 end = float4x4::zero;

			start = it->second->GetTransform()->GetGlobalTransform();
			end = it->second->GetChildAt(i)->GetTransform()->GetGlobalTransform();

			float3 startpos = float3::zero;
			float3 endpos = float3::zero;

			float3 _position = float3::zero;
			float3 _scale = float3::zero;
			Quat _rotation = Quat::identity;

			start.Decompose(_position, _rotation, _scale);
			startpos = _position;

			end.Decompose(_position, _rotation, _scale);
			endpos = _position;

			App->renderer3D->DrawCustomRay(startpos, endpos);
		}
	}
}

void Animation::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;

	save_object.AddInt("Type", type);
	save_object.AddInt("AnimationID", _resource->GetUID());

	save_array.AddObject(save_object);
}

void Animation::Load(GnJSONObj& load_object)
{
	int animationUID = load_object.GetInt("AnimationID");
	SetResourceUID(animationUID);
}

void Animation::SetResourceUID(uint UID)
{
	if (_resource != nullptr)
		App->resources->ReleaseResource(_resourceUID);

	_resourceUID = UID;
	_resource = (ResourceAnimation*)App->resources->RequestResource(_resourceUID);
}

Resource* Animation::GetResource(ResourceType type)
{
	return _resource;
}

void Animation::OnEditor()
{
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox(" Enabled", &enabled);

		ImGui::Separator();
		ImGui::Spacing();

		if (_resource != nullptr)
		{
			std::string animationID = "Animation: ";
			animationID.append(_resource->name);
			ImGui::Button(animationID.c_str());

			ImGui::Spacing();

			ImGui::Text("Assets path: %s", _resource->assetsFile.c_str());
			ImGui::Text("Library path: %s", _resource->libraryFile.c_str());

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Text("Channels: %d Duration: %f", _resource->anim_NumChannels, _resource->anim_Duration);
			ImGui::Text("Ticks: %f", _resource->anim_TicksPerSecond);
			ImGui::Text("Current animation time: %d", anim_time);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Show Bones", &viewbones);
		}
		else
		{
			ImGui::Button("Unknown Animation");
		}

		ImGui::Spacing();
		ImGui::Text("UID: %d", _resourceUID);
		ImGui::Spacing();
	}
}