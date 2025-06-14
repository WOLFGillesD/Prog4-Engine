#pragma once
#include <functional>
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void DestroyScene(const std::string& name);
		bool ContainsScene(const std::string& name) const;

		void TransitionScene(const std::string& from, const std::function<void()>& func);

		void Start();

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;
		void ImGuiUpdate();
		void RemoveMarkedForRemoval();

		void End();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;

		std::vector<std::string> m_DeletedScenes{};
	};
}
