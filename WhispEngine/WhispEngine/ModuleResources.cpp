#include "ModuleResources.h"
#include "Application.h"
#include "ResourceModel.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"

ModuleResources::ModuleResources()
{
	name.assign("resources");
}


ModuleResources::~ModuleResources()
{
}

bool ModuleResources::CleanUp()
{
	for (auto i = resources.begin(); i != resources.end(); i++)
		delete (*i).second;

	return true;
}

uint64 ModuleResources::Find(const char * assets_file) const
{
	for (auto i = resources.begin(); i != resources.end(); ++i) {
		if ((*i).second->file.compare(assets_file) == 0)
			return (*i).first;
	}

	return 0u;
}

uint64 ModuleResources::ImportFile(const char * file_path)
{
	uint64 uid = 0u;
	if (App->importer->Import(file_path, uid)) {

		if (uid != 0u)
			return uid;
	}
	
	LOG("Failed to Create Resource with path %s", file_path);

	return 0u;
}

Resource * ModuleResources::CreateResource(Resource::Type type, const uint64 & force_uid)
{
	Resource* ret = nullptr;

	uint64 uid = force_uid;
	if (uid == 0U)
		uid = App->random->RandomGUID();

	switch (type) {
		case Resource::TEXTURE: ret = (Resource*) new ResourceTexture(uid); break;
		case Resource::MODEL:	ret = (Resource*) new ResourceModel(uid);	break;
		case Resource::MESH:	ret = (Resource*) new ResourceMesh(uid);	break;
		//case Resource::scene: ret = (Resource*) new ResourceScene(uid); break;

		default: LOG("Resource with type %i has not setted to be created", (int)type); break;
	}
	if (ret != nullptr)
		resources[uid] = ret;

	return ret;
}

bool ModuleResources::LoadToMemory(const uint64 &uid)
{
	Resource* res = Get(uid);
	if (res != nullptr)
		return res->LoadToMemory();
	return false;
}

bool ModuleResources::FreeMemory(const uint64 & uid)
{
	Resource* res = Get(uid);
	if (res != nullptr)
		return res->FreeMemory();
	return false;
}

const Resource * ModuleResources::Get(const uint64 & uid) const
{
	auto ret = resources.find(uid);
	return ret != resources.end() ? (*ret).second : nullptr;
}

Resource * ModuleResources::Get(const uint64 & uid)
{
	auto ret = resources.find(uid);
	return ret != resources.end() ? (*ret).second : nullptr;
}
