#include "SceneManager.h"

#include <unordered_set>

#include "Scene.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::LateUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->LateUpdate();
	}
}

void dae::SceneManager::Render() const
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::ImGuiUpdate()
{
	for (const auto& scene : m_scenes)
	{
		scene->ImGuiUpdate();
	}
}

void dae::SceneManager::RemoveMarkedForRemoval()
{
	for (auto& scene : m_scenes)
	{
		scene->RemoveMarkedForRemoval();
	}

	for (auto& name : m_DeletedScenes)
	{
		auto newEnd = std::remove_if(
			m_scenes.begin(), m_scenes.end(),
			[&](std::shared_ptr<Scene> s) {
				return s->GetName() == name;
			}
		);
		m_scenes.erase(newEnd, m_scenes.end());
	}
	m_DeletedScenes.clear();
}

void dae::SceneManager::End()
{
	for (auto& scene : m_scenes)
	{
		scene->End();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

void dae::SceneManager::DestroyScene(const std::string& name)
{
	m_DeletedScenes.push_back(name);
}

bool dae::SceneManager::ContainsScene(const std::string& name) const
{

	for (const auto& scene : m_scenes)
	{
		if (scene->GetName() == name)
		{
			return true;
		}
	}
	return false;

}

void dae::SceneManager::TransitionScene(const std::string& from, const std::function<void()>& func)
{
	DestroyScene(from);
	func();
	Start();
}

void dae::SceneManager::Start()
{
	for (auto& scene : m_scenes)
	{
		scene->Start();
	}
}

void dae::SceneManager::FixedUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate();
	}
}
