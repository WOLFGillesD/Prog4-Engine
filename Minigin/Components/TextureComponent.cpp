#include "Components/TextureComponent.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

dae::TextureComponent::TextureComponent(GameObject& go, const std::string& fullPath)
	: Component(go)
{
	m_Texture2D = ResourceManager::GetInstance().LoadTexture(fullPath);
}

void dae::TextureComponent::Render() const
{
	Renderer::GetInstance().RenderTexture(*m_Texture2D, GetOwner()->GetWorldTransform().GetPosition().x, GetOwner()->GetWorldTransform().GetPosition().y);
}
