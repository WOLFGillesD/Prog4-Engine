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

		void SetPosition(float x, float y);

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

		Transform GetTransform() const { return m_transform; }
		void SetTransform(const Transform& transform) { m_transform = transform; }

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		std::vector<std::unique_ptr<Component>> m_Components{};
	};
}
