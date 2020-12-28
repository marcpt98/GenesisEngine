#include "Animation.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "Transform.h"

#include "ResourceAnimation.h"

Animation::Animation() : Component(), name("No name"), _resource(nullptr)
{
	type = ComponentType::ANIMATION;

	name = nullptr;
	path = nullptr;
}

Animation::~Animation()
{
	if (_resource != nullptr)
	{
		App->resources->ReleaseResource(_resource->GetUID());
		_resource = nullptr;
	}
}

void Animation::Update()
{
	Render();
}

void Animation::Render()
{

}

void Animation::OnEditor()
{

}

