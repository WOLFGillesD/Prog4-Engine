#pragma once
#include <string>
#include <memory>

#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public Component
	{
	public:
		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);

		TextComponent(GameObject& go, const std::string& text, std::shared_ptr<Font> font);
		~TextComponent() override = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}
