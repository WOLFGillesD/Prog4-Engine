#include <string>
#include "GameObject.h"

#include <iostream>

#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Start()
{
	for (const auto& comp : m_Components)
	{
		comp->Start();
	}
}

void dae::GameObject::FixedUpdate()
{
	for (const auto& comp : m_Components)
	{
		comp->FixedUpdate();
	}
}

void dae::GameObject::Update()
{
	for (const auto& comp : m_Components)
	{
		comp->Update();
	}
}

void dae::GameObject::LateUpdate()
{
	for (const auto& comp : m_Components)
	{
		comp->LateUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const auto& comp : m_Components)
	{
		comp->Render();
	}
}

void dae::GameObject::RemoveMarkedForRemoval()
{

	//for (int index{ m_Components.size() }; index > 0 ; --index)
	//{
	//	const auto& comp{ m_Components[index] };
	//	if (comp->m_MarkedForRemoval)
	//	{
	//		m_Components.erase(m_Components.begin() + index);
	//	}
	//}

	std::erase_if(m_Components, [](const std::unique_ptr<Component>& comp) { return comp->m_MarkedForRemoval; });
}

void dae::GameObject::End()
{
	for (const auto& comp : m_Components)
	{
		comp->End();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
