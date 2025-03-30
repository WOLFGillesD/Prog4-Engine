#include "Component.h"

dae::Component::Component(GameObject& go)
	: m_Owner(&go)
{
}
