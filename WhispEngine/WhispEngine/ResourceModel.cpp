#include "ResourceModel.h"
#include "Assimp/include/scene.h"
#include "Application.h"
#include "JSON/json.hpp"
#include "ComponentTransform.h"

ResourceModel::ResourceModel(const uint64 & uid) : Resource(uid, Resource::Type::MODEL)
{
}

ResourceModel::~ResourceModel()
{
}

ResourceModel::HierarchyInfo ResourceModel::CalculateHierarchy(const aiNode * node, const aiScene * scene, const std::vector<uint64>& meshes, const std::vector<uint64>& materials, ResourceModel::HierarchyInfo * node_from)
{
	HierarchyInfo info;
	info.parent = node_from;
	info.name.assign(node->mName.C_Str());

	aiVector3D pos, scale; aiQuaternion rot;
	node->mTransformation.Decompose(scale, rot, pos);
	info.position.Set(pos.x, pos.y, pos.z);
	info.rotation.Set(rot.w, rot.x, rot.y, rot.z);
	float div_scale = std::max(scale.x, scale.y);
	div_scale = std::max(div_scale, scale.z);
	scale /= div_scale;
	info.scale.Set(scale.x, scale.y, scale.z);

	if (node->mNumMeshes == 1) {
		info.mesh = meshes[node->mMeshes[0]];
		info.material = materials[scene->mMeshes[0]->mMaterialIndex];
	}
	else if (node->mNumMeshes > 1) {
		for (int j = 0; j < node->mNumMeshes; ++j) {
			HierarchyInfo child_mesh;
			child_mesh.mesh = meshes[node->mMeshes[j]];
			child_mesh.name.assign(scene->mMeshes[node->mMeshes[j]]->mName.C_Str());
			info.children.push_back(child_mesh);
		}
	}

	if (node->mNumChildren > 0)
		for (int i = 0; i < node->mNumChildren; i++) {
			info.children.push_back(CalculateHierarchy(node->mChildren[i], scene, meshes, materials, &info));
		}

	if (node_from == nullptr)
		model = info;
	return info;
}

bool ResourceModel::LoadInMemory()
{
	nlohmann::json data = App->dummy_file_system->OpenFile(resource_path.c_str());
	if (data == nullptr) {
		LOG("Model %s not found", resource_path.c_str());
		return false;
	}

	GameObject* container = App->object_manager->CreateGameObject(nullptr);
	if (data.is_object()) {
		nlohmann::json::iterator object = data.begin();
		container->SetName(object.key().c_str());
		if ((*object).is_array()) {
			for (nlohmann::json::iterator it = (*object).begin(); it != (*object).end(); it++) {
				CreateObjects(container, *it);
			}
		}
	}

	return true;
}

bool ResourceModel::FreeMemory()
{
	return false;
}

void ResourceModel::CreateObjects(GameObject * container, const nlohmann::json & data)
{
	GameObject* child = App->object_manager->CreateGameObject(container);

	child->SetName(data["name"].get<std::string>().c_str());
	ComponentTransform* transform = (ComponentTransform*)child->GetComponent(ComponentType::TRANSFORM);
	transform->SetLocalMatrix(
		float3(data["position"][0], data["position"][1], data["position"][2]),
		Quat(data["rotation"][1], data["rotation"][2], data["rotation"][3], data["rotation"][0]),
		float3(data["scale"][0], data["scale"][1], data["scale"][2])
	);
	transform->CalculateGlobalMatrix();

	ComponentMesh* mesh = (ComponentMesh*)child->CreateComponent(ComponentType::MESH);
	mesh->uid = data.value("meshId", 0u);
	App->resources->LoadToMemory(mesh->uid);

	if (data.find("children") != data.end()) {
		for (nlohmann::json::const_iterator it = data["children"].begin(); it != data["children"].end(); it++) {
			CreateObjects(child, *it);
		}
	}
}

bool ResourceModel::Save()
{
	nlohmann::json json;

	SaveNode(model, json[file.c_str()]);

	App->dummy_file_system->SaveFile(resource_path.c_str(), json);

	return true;
}

void ResourceModel::SaveNode(const HierarchyInfo & object, nlohmann::json & node)
{
	nlohmann::json section;

	section["name"] = object.name.c_str();

	if (object.mesh > 0u)
		section["meshId"] = object.mesh;
	if(object.material > 0u)
		section["materialId"] = object.material;

	section["position"] = { object.position.x, object.position.y, object.position.z };
	section["rotation"] = { object.rotation.x, object.rotation.y, object.rotation.z, object.rotation.w };
	section["scale"]	= { object.scale.x,    object.scale.y,	  object.scale.z };

	if (object.children.size() > 0) {
		for (auto i = object.children.begin(); i != object.children.end(); ++i) {
			SaveNode(*i, section["children"]);
		}
	}

	node.push_back(section);
}
