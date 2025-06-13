#pragma once
#include <memory>
#include <string>

#include "Component.h"
#include "SDL_rect.h"
#include "Texture2D.h"

struct SDL_Texture;
namespace dae
{
	class TextureComponent : public Component
	{
	public:
		explicit TextureComponent(GameObject& go, const std::string& fullPath, float scale = 1.f, const glm::vec2& offset = {0,0});
		~TextureComponent() override = default;

		void Render() const override;

		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) noexcept = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) noexcept = delete;

	private:
		std::shared_ptr<Texture2D> m_Texture2D{};
        float m_Scale{};
        glm::vec2 m_Offset{};
	};

    class SpriteComponent : public Component
    {
    public:
        explicit SpriteComponent(GameObject& go, const std::string& fullPath, int rows, int columns, int index = 0, float rotation = 0.f, bool isAnimated = true);
        ~SpriteComponent() override = default;

        void Update() override;
        void Render() const override;

        void SetIndex(int index);
        int GetIndex() const { return m_Index; }

        void SetIsAnimated(bool state) { m_IsAnimated = state; }

        void SetAnimationSpeed(float fps)
        {
            if (fps < 0.001f) fps = 0.001f;
            m_AnimSpeed = fps;
        }
        void SetRotation(float angle) { m_Rotation = angle; }

    private:
        int m_Rows;
        int m_Columns;
        int m_Index;

        int m_SpriteWidth;
        int m_SpriteHeight;

        float m_ElapsedTime = 0.0f;
        float m_AnimSpeed = 10.0f; // frames per second
        float m_Rotation = 0.0f;   // rotation in degrees

        bool m_IsAnimated{};

        SDL_Rect m_SourceRect;
        std::shared_ptr<Texture2D> m_Texture;

        void UpdateSourceRect();
    };
}