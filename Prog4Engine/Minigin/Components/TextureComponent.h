#pragma once
#include <memory>
#include <string>

#include "Component.h"
#include "Texture2D.h"

struct SDL_Texture;
namespace dae
{
	class TextureComponent : public Component
	{
	public:
		explicit TextureComponent(GameObject& go, const std::string& fullPath);
		~TextureComponent() override = default;

		void Render() const override;

		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) noexcept = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) noexcept = delete;

	private:
		std::shared_ptr<Texture2D> m_Texture2D{};
	};
}