#pragma once

#include <vector>
#include <functional>
#include <string>

#include "Component.h"
#include "vec2.hpp"

namespace dae
{
    class ColliderComponent : public Component
    {
    public:
        using CollisionCallback = std::function<void(ColliderComponent& other)>;

        ColliderComponent(GameObject& go, const glm::vec2& size = { 0,0 }, const glm::vec2& offset = { 0,0 }, const std::string& tag = "");
        ~ColliderComponent() override;

        ColliderComponent(const ColliderComponent& other) = delete;
        ColliderComponent(ColliderComponent&& other) noexcept = delete;
        ColliderComponent& operator=(const ColliderComponent& other) = delete;
        ColliderComponent& operator=(ColliderComponent&& other) noexcept = delete;

        void Update() override;
        void Render() const override;

        const glm::vec2& GetSize() const { return m_Size; }
        const glm::vec2& GetOffset() const { return m_Offset; }
        glm::vec2 GetWorldPosition() const;

        bool IsColliding(const ColliderComponent& other) const;

        void SetCollisionCallback(CollisionCallback callback) { m_Callback = std::move(callback); }
        const std::string& GetTag() const { return m_Tag; }
        void SetTag(const std::string& tag) { m_Tag = tag; }

        static const std::vector<ColliderComponent*>& GetAllColliders() { return s_AllColliders; }

    private:
        glm::vec2 m_Size{};
        glm::vec2 m_Offset{};
        std::string m_Tag{};
        CollisionCallback m_Callback{};

        static std::vector<ColliderComponent*> s_AllColliders;
    };
}
