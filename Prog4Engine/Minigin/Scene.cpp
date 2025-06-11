#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Start()
{
	for (auto& object : m_objects)
	{
		object->Start();
	}
}

void Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}
}

void Scene::Render() const
{
	auto renderList = m_objects;
	std::sort(renderList.begin(), renderList.end(), [](const std::shared_ptr<GameObject>& a, const std::shared_ptr<GameObject>& b) {
		return a->GetDepthIndex() > b->GetDepthIndex();
		});

	for (const auto& object : renderList)
	{
		object->Render();
	}
}

void Scene::ImGuiUpdate()
{
	for (const auto& object : m_objects)
	{
		object->ImGuiUpdate();
	}
}

void Scene::RemoveMarkedForRemoval()
{
	std::erase_if(m_objects, [](std::shared_ptr<GameObject> go) { return go->GetMarkedForRemoval(); });

	for (const auto& object : m_objects)
	{
		object->RemoveMarkedForRemoval();
	}
}

void Scene::End()
{
	for (auto& object : m_objects)
	{
		object->End();
	}
}

