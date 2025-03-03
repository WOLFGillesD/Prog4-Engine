#include <string>
#include "GameObject.h"

#include <iostream>

#include "ResourceManager.h"
#include "Renderer.h"

dae::Transform dae::GameObject::GetWorldTransform()
{
	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldTransform;
}

void dae::GameObject::SetLocalTransform(const Transform& transform)
{
	m_LocalTransform = transform;
	SetPositionDirty();
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_Parent == parent)
		return;
	if (parent == nullptr)
		m_LocalTransform.SetPosition(GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			m_LocalTransform.SetPosition(GetWorldPosition() - parent->GetWorldPosition());
		SetPositionDirty();
	}

	if (m_Parent) m_Parent->RemoveChild(this);
	m_Parent = parent;
	if (m_Parent) m_Parent->AddChild(this);
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldTransform.GetPosition();
}

const glm::vec3& dae::GameObject::GetLocalPosition() const
{
	return m_LocalTransform.GetPosition();
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (m_Parent == nullptr)
			m_WorldTransform = m_LocalTransform;
		else
			m_WorldTransform.SetPosition(m_Parent->GetWorldPosition() + m_LocalTransform.GetPosition());
	}
	m_PositionIsDirty = false;

}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_Parent;
}

void dae::GameObject::SetMarkForRemoval()
{
	m_MarkedForRemoval = true;
	for (const auto& child : m_Children)
	{
		child->SetMarkForRemoval();
	}
}

void dae::GameObject::SetPositionDirty()
{
	m_PositionIsDirty = true;
	for (const auto& child : m_Children)
	{
		child->SetPositionDirty();
	}
}

void dae::GameObject::AddChild(GameObject* go)
{
	m_Children.push_back(go);
}

void dae::GameObject::RemoveChild(GameObject* go)
{
	std::erase_if(m_Children, [go](GameObject* gameObject) { return go == gameObject; });
}

bool dae::GameObject::IsChild(const GameObject* go) const
{
	for (const auto & child : m_Children)
	{
		if (child == go)
			return true;
	} 
	return false;
}

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
	std::erase_if(m_Components, [](const std::unique_ptr<Component>& comp) { return comp->m_MarkedForRemoval; });
}

void dae::GameObject::ImGuiUpdate()
{
	for (const auto& comp : m_Components)
	{
		comp->ImGuiUpdate();
	}
}

void dae::GameObject::End()
{
	for (const auto& comp : m_Components)
	{
		comp->End();
	}
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_LocalTransform.SetPosition(x, y, 0.0f);
	SetPositionDirty();
}
