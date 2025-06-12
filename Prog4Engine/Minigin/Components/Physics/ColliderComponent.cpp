#include "ColliderComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include <SDL_rect.h>
#include <string>

// Static member definition
std::vector<dae::ColliderComponent*> dae::ColliderComponent::s_AllColliders;

dae::ColliderComponent::ColliderComponent(GameObject& go, const glm::vec2& size, const glm::vec2& offset, const std::string& tag)
    : Component(go)
    , m_Size(size)
    , m_Offset(offset)
    , m_Tag(tag)
{
    s_AllColliders.push_back(this);
}

dae::ColliderComponent::~ColliderComponent()
{
    auto it = std::find(s_AllColliders.begin(), s_AllColliders.end(), this);
    if (it != s_AllColliders.end()) s_AllColliders.erase(it);
}

void dae::ColliderComponent::Update()
{
    for (auto* other : s_AllColliders)
    {
        if (other == this) continue;

        // Only collide objects with different tags
        if (!m_Tag.empty() && m_Tag == other->m_Tag) continue;

        if (IsColliding(*other))
        {
            if (m_Callback) m_Callback(*other);
            if (other->m_Callback) other->m_Callback(*this);
        }
    }
}

void dae::ColliderComponent::Render() const
{
    const SDL_Rect rect{
        static_cast<int>(GetWorldPosition().x),
        static_cast<int>(GetWorldPosition().y),
        static_cast<int>(m_Size.x),
        static_cast<int>(m_Size.y)
    };
    SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
    SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
}

glm::vec2 dae::ColliderComponent::GetWorldPosition() const
{
    glm::vec2 pos = GetOwner()->GetWorldPosition(); // Assuming GameObject has GetPosition()
    return pos + m_Offset;
}

bool dae::ColliderComponent::IsColliding(const ColliderComponent& other) const
{
    glm::vec2 aPos = GetWorldPosition();
    glm::vec2 bPos = other.GetWorldPosition();

    bool overlapX = aPos.x < bPos.x + other.m_Size.x && aPos.x + m_Size.x > bPos.x;
    bool overlapY = aPos.y < bPos.y + other.m_Size.y && aPos.y + m_Size.y > bPos.y;
    return overlapX && overlapY;
}
