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
namespace fs = std::filesystem;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& inputManager = dae::InputManager::GetInstance();

	auto imc = std::make_unique<dae::InputMapping>();
	auto imk = std::make_unique<dae::InputMapping>();

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
	go->SetLocalPosition(500, 350);


	imk->AddCommand(SDLK_w, dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(0,-1)) });
	imk->AddCommand(SDLK_z, dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(0,-1)) });
	imk->AddCommand(SDLK_s, dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(0,1)) });
	imk->AddCommand(SDLK_q, dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(-1,0)) });
	imk->AddCommand(SDLK_a, dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(-1,0)) });
	imk->AddCommand(SDLK_d, dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(1,0)) });

	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>("CEMERALD.tga");
	go->SetLocalPosition(400, 350);

	scene.Add(go);

	imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_UP), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(0,-1)) });
	imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_DOWN), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(0,1)) });
	imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_LEFT), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(-1,0)) });
	imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_RIGHT), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(1,0)) });


	inputManager.SetControllerInputMapping(std::move(imc));
	inputManager.SetKeyboardInputMapping(std::move(imk));
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
