#pragma once
//#include "GameObject.h"

namespace dae
{
	class GameObject;

	class Component
	{
		GameObject* m_Owner{nullptr};
	public:
		Component(GameObject& go);
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) noexcept = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept = delete;

		GameObject* GetOwner() const { return m_Owner; }

		virtual void Start() {}

		virtual void FixedUpdate(){}
		virtual void Update(){}
		virtual void LateUpdate(){}
		virtual void Render() const{}
		virtual void ImGuiUpdate(){}

		virtual void End() {}

		bool m_MarkedForRemoval{ false };

	protected:
	};
}
