#include "FpsComponent.h"

#include <iomanip>
#include <sstream>

#include "DaeTime.h"
#include "GameObject.h"
#include "TextComponent.h"

dae::FpsComponent::FpsComponent(GameObject& go)
	: Component(go)
{
}

void dae::FpsComponent::Start()
{
	auto textComponent = GetOwner()->GetComponent<dae::TextComponent>();
	if (textComponent != nullptr)
	{
		m_TextComponent = textComponent;
	}
}

void dae::FpsComponent::Update()
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << 1.f / dae::Time::m_DeltaTime << "Fps";
	m_TextComponent->SetText(ss.str());
}
