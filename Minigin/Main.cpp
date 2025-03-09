#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"

#include <filesystem>

#include "InputManager.h"
#include "Components/FpsComponent.h"
#include "Components/RotatorComponent.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Components/ControllerMovementComponent.h"
#include "Components/KeyboardMovementComponent.h"
namespace fs = std::filesystem;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& inputManager = dae::InputManager::GetInstance();

	auto imc = std::make_unique<dae::InputMapping>();
	auto imk = std::make_unique<dae::InputMapping>();
	inputManager.SetControllerInputMapping(std::move(imc));
	inputManager.SetKeyboardInputMapping(std::move(imk));


	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>("logo.tga");
	go->SetLocalPosition(216, 180);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->SetLocalPosition(80, 20);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("FPS", font);
	go->AddComponent<dae::FpsComponent>();
	go->SetLocalPosition(20, 80);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->SetLocalPosition(500, 350);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>("CEMERALD.tga");
	go->AddComponent<dae::KeyboardMovementComponent>();
	go->SetLocalPosition(500, 350);

	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>("CEMERALD.tga");
	go->AddComponent<dae::ControllerMovementComponent>();
	go->SetLocalPosition(400, 350);

	scene.Add(go);
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
