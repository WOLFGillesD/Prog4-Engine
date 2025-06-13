#include "Components/TextureComponent.h"

#include "DaeTime.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

dae::TextureComponent::TextureComponent(GameObject& go, const std::string& fullPath, float scale, const glm::vec2& offset)
	: Component(go)
	, m_Scale(scale)
	, m_Offset(offset)
{
	m_Texture2D = ResourceManager::GetInstance().LoadTexture(fullPath);
}

void dae::TextureComponent::Render() const
{
	Renderer::GetInstance().RenderTexture(*m_Texture2D, m_Offset.x + GetOwner()->GetWorldTransform().GetPosition().x,
											m_Offset.y + GetOwner()->GetWorldTransform().GetPosition().y,
	                                      m_Scale * static_cast<float>(m_Texture2D->GetSize().x),
	                                      m_Scale * static_cast<float>(m_Texture2D->GetSize().y));
}

dae::SpriteComponent::SpriteComponent(GameObject& go, const std::string& fullPath, int rows, int columns, int index, float rotation, bool isAnimated)
    : Component(go)
    , m_Rows(rows)
    , m_Columns(columns)
    , m_Index(index)
    , m_Rotation(rotation)
	, m_IsAnimated(isAnimated)
{
    m_Texture = ResourceManager::GetInstance().LoadTexture(fullPath);
    int texW, texH;
    SDL_QueryTexture(m_Texture->GetSDLTexture(), nullptr, nullptr, &texW, &texH);
    m_SpriteWidth = texW / m_Columns;
    m_SpriteHeight = texH / m_Rows;
    UpdateSourceRect();
}

void dae::SpriteComponent::Update()
{
    if (!m_IsAnimated) return;
    m_ElapsedTime += Time::m_DeltaTime;

    const float timePerFrame = 1.0f / m_AnimSpeed;
    if (m_ElapsedTime >= timePerFrame)
    {
        m_ElapsedTime -= timePerFrame;
        m_Index = (m_Index + 1) % (m_Rows * m_Columns);
        UpdateSourceRect();
    }
}

void dae::SpriteComponent::Render() const
{
    auto pos = GetOwner()->GetWorldPosition();
    // TODO: Add logic for flipping and rotating the spite based on move direction
    Renderer::GetInstance().RenderTexture(*m_Texture, m_SourceRect, pos.x, pos.y, static_cast<float>(m_SpriteWidth), static_cast<float>(m_SpriteHeight), m_Rotation, SDL_FLIP_HORIZONTAL);
}

void dae::SpriteComponent::SetIndex(int index)
{
    m_Index = index;
    UpdateSourceRect();
}

void dae::SpriteComponent::UpdateSourceRect()
{
    int row = m_Index / m_Columns;
    int col = m_Index % m_Columns;
    m_SourceRect.x = col * m_SpriteWidth;
    m_SourceRect.y = row * m_SpriteHeight;
    m_SourceRect.w = m_SpriteWidth;
    m_SourceRect.h = m_SpriteHeight;
}
