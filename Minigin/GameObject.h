#pragma once
#include <memory>
#include <optional>
#include <vector>

#include "Components/Component.h"
#include "Transform.h"

namespace dae
{
	class GameObject final
	{
	public:
		void Start();

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;
		void RemoveMarkedForRemoval();

		void End();


		// Adds a component to the gameobject
		template<typename ComponentType, typename... Args>
		void AddComponent(Args... args)
		{
			m_Components.push_back(std::move(std::make_unique<ComponentType>(*this, args...)));
		}

		// Removes first component of specified type
		template<typename ComponentType>
		bool RemoveComponent() const
		{
			for (auto& comp : m_Components)
			{
				auto casted = dynamic_cast<ComponentType*>(comp.get());
				if (casted != nullptr)
				{
					comp->m_MarkedForRemoval = true;
					return true;
				}
			}

			return false;
		}


		// Returns first component of specified type
		template<typename ComponentType>
		ComponentType* GetComponent()
		{
			for (const auto& comp : m_Components)
			{
				auto casted = dynamic_cast<ComponentType*>(comp.get());
				if (casted != nullptr)
					return casted;
			}
			return nullptr;
		}

		void SetPosition(float x, float y);

		Transform GetWorldTransform();
		void SetTransform(const Transform& transform);

		void SetParent(GameObject* parent, bool keepWorldPosition);
		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const;

		void UpdateWorldPosition();

		GameObject* GetParent() const;
		void AddChild(GameObject* go);
		void RemoveChild(GameObject* go);

		bool GetMarkedForRemoval() const { return m_MarkedForRemoval; }
		void SetMarkForRemoval() { m_MarkedForRemoval = true; }


		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_WorldTransform{};
		Transform m_LocalTransform{};
		bool m_PositionIsDirty{};
		bool m_MarkedForRemoval{ false };

		GameObject* m_Parent{};
		std::vector<std::unique_ptr<Component>> m_Components{};
		std::vector<GameObject*> m_Children{};

		bool IsChild(const GameObject* go) const;
	};
}
