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

#include "ColliderComponent.h"
#include "InputManager.h"
#include "Audio/Servicelocator.h"
#include "Input/EventManager.h"
#include "Components/FpsComponent.h"
#include "Components/GridComponent.h"
#include "Components/Characters/HealthComponent.h"
#include "Components/Characters/PlayerComponent.h"
#include "Components/RotatorComponent.h"
#include "Components/Score/ScoreComponent.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"

namespace fs = std::filesystem;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& inputManager = dae::InputManager::GetInstance();
	//auto& eventManager = dae::EventManager::GetInstance();

	auto imc = std::make_unique<dae::InputMapping>();
	auto imk = std::make_unique<dae::InputMapping>();
	//auto imc2 = std::make_unique<dae::InputMapping>();

	auto gp = std::make_unique<XInputGamepad>(0);
	auto gp2 = std::make_unique<XInputGamepad>(1);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);

	// GameObjects
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>("background.tga");
	go->SetDepthIndex(2);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>("logo.tga");
	go->SetLocalPosition(216, 180);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->SetLocalPosition(80, 20);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("FPS", font);
	go->AddComponent<dae::FpsComponent>();
	go->SetLocalPosition(20, 80);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<game::GridComponent>(10, 15, 40, glm::vec2{20, 80});

	auto go2 = std::make_shared<dae::GameObject>();
	go2->AddComponent<game::MovementComponent>(go->GetComponent<game::GridComponent>(), 100.f);

	go2->AddComponent<dae::SpriteComponent>("Player/PlayerMovement.png", 1, 6, 0, 90.f);
	go2->AddComponent<dae::ColliderComponent>(glm::vec2(40, 40), glm::vec2(0, 0), "Player");

	imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_UP), dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::MovementComponent>(), glm::vec2(0,-1)) });
	imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_DOWN), dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::MovementComponent>(), glm::vec2(0,1)) });
	imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_LEFT), dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::MovementComponent>(), glm::vec2(-1,0)) });
	imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_RIGHT), dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::MovementComponent>(), glm::vec2(1,0)) });

	imk->AddCommand(SDLK_w, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::MovementComponent>(), glm::vec2(0,-1)), dae::InputState::IsPressed });
	//imk->AddCommand(SDLK_w, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::PlayerComponent>(), glm::vec2(0,0)), dae::InputState::IsUp });

	imk->AddCommand(SDLK_s, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::MovementComponent>(), glm::vec2(0,1)), dae::InputState::IsPressed });
	//imk->AddCommand(SDLK_s, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::PlayerComponent>(), glm::vec2(0,0)), dae::InputState::IsUp });

	imk->AddCommand(SDLK_a, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::MovementComponent>(), glm::vec2(-1,0)), dae::InputState::IsPressed });
	//imk->AddCommand(SDLK_a, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::PlayerComponent>(), glm::vec2(0,0)), dae::InputState::IsUp });

	imk->AddCommand(SDLK_d, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::MovementComponent>(), glm::vec2(1,0)), dae::InputState::IsPressed });
	//imk->AddCommand(SDLK_d, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::PlayerComponent>(), glm::vec2(0,0)), dae::InputState::IsUp });

	scene.Add(go2);
	scene.Add(go);
	//// AUDIO

	//// ----- Sound Service logger
	////dae::Servicelocator::RegisterSoundSystem(std::make_unique<dae::SoundSystemLogger>());

	//dae::Servicelocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());
	//auto songID = dae::Servicelocator::GetSoundSystem().LoadSound("Data/Audio/Popcorn_Remix_for_Digger.mp3");
	//dae::Servicelocator::GetSoundSystem().Play(songID);
	//auto deathID = dae::Servicelocator::GetSoundSystem().LoadSound("Data/Audio/DeathSound.mp3");


	//// GameObjects
	//auto go = std::make_shared<dae::GameObject>();
	//go->AddComponent<dae::TextureComponent>("background.tga");
	//scene.Add(go);

	//go = std::make_shared<dae::GameObject>();
	//go->AddComponent<dae::TextureComponent>("logo.tga");
	//go->SetLocalPosition(216, 180);
	//scene.Add(go);

	//go = std::make_shared<dae::GameObject>();
	//go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	//go->SetLocalPosition(80, 20);
	//scene.Add(go);

	//go = std::make_shared<dae::GameObject>();
	//go->AddComponent<dae::TextComponent>("FPS", font);
	//go->AddComponent<dae::FpsComponent>();
	//go->SetLocalPosition(20, 80);
	//scene.Add(go);

	//go = std::make_shared<dae::GameObject>();
	//go->SetLocalPosition(500, 350);

	//go = std::make_shared<dae::GameObject>();
	//go->AddComponent<dae::TextureComponent>("VRHOB1.png");
	//go->AddComponent<dae::HealthComponent>();
	//go->AddComponent<dae::ScoreComponent>();
	//go->SetLocalPosition(500, 350);

	//auto healthComponent = go->GetComponent<dae::HealthComponent>();
	//auto scoreComponent = go->GetComponent<dae::ScoreComponent>();
	//imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_X), dae::InputCommand{new dae::DieCommand(healthComponent), dae::InputState::IsDown});
	//imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_A), dae::InputCommand{ new dae::UpScoreCommand(scoreComponent, 20), dae::InputState::IsUp });
	//imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_B), dae::InputCommand{ new dae::UpScoreCommand(scoreComponent, 100), dae::InputState::IsUp });

	//

	//auto go2 = std::make_shared<dae::GameObject>();
	//go2->AddComponent<dae::TextComponent>("AAA", font2);
	//go2->SetLocalPosition(10, 200);

	//auto txtComp = go2->GetComponent<dae::TextComponent>();
	//auto healthObserver = std::make_unique<dae::HealthObserver>(txtComp);
	//txtComp->SetText("# Lives: " + std::to_string(healthComponent->GetLives()));

	//healthComponent->GetOnDieEvent()->AddObserver(healthObserver.get());
	//auto soundObserver = std::make_unique<dae::SoundObserver>(deathID);

	//healthComponent->GetOnDieEvent()->AddObserver(soundObserver.get());
	//eventManager.AddObserver(std::move(soundObserver));

	//eventManager.AddObserver(std::move(healthObserver));

	//scene.Add(go2);


	//go2 = std::make_shared<dae::GameObject>();
	//go2->AddComponent<dae::TextComponent>("AAA", font2);
	//go2->SetLocalPosition(10, 220);

	//txtComp = go2->GetComponent<dae::TextComponent>();
	//auto scoreObserver = std::make_unique<dae::ScoreObserver>(txtComp);
	//txtComp->SetText("Score: " + std::to_string(scoreComponent->GetScore()));

	//scoreComponent->OnScoreChanged()->AddObserver(scoreObserver.get());
	//eventManager.AddObserver(std::move(scoreObserver));
	//scene.Add(go2);

	////imc2->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_UP), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(0,-1)) });
	////imc2->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_DOWN), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(0,1)) });
	////imc2->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_LEFT), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(-1,0)) });
	////imc2->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_RIGHT), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(1,0)) });

	//imk->AddCommand(SDLK_w, dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(0,-1)) });
	//imk->AddCommand(SDLK_s, dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(0,1)) });
	//imk->AddCommand(SDLK_a, dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(-1,0)) });
	//imk->AddCommand(SDLK_d, dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(1,0)) });

	//scene.Add(go);

	//go = std::make_shared<dae::GameObject>();
	//go->AddComponent<dae::TextureComponent>("VNOBD.png");
	//go->AddComponent<dae::HealthComponent>();
	//go->AddComponent<dae::ScoreComponent>();
	//go->SetLocalPosition(400, 350);

	//healthComponent = go->GetComponent<dae::HealthComponent>();

	//soundObserver = std::make_unique<dae::SoundObserver>(deathID);

	//healthComponent->GetOnDieEvent()->AddObserver(soundObserver.get());
	//eventManager.AddObserver(std::move(soundObserver));


	//scoreComponent = go->GetComponent<dae::ScoreComponent>();
	//imk->AddCommand(SDLK_c, dae::InputCommand{ new dae::DieCommand(healthComponent), dae::InputState::IsDown });
	//imk->AddCommand(SDLK_z, dae::InputCommand{ new dae::UpScoreCommand(scoreComponent, 20), dae::InputState::IsUp });
	//imk->AddCommand(SDLK_x, dae::InputCommand{ new dae::UpScoreCommand(scoreComponent, 100), dae::InputState::IsUp });

	//scene.Add(go);

	//imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_UP), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(0,-1)) });
	//imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_DOWN), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(0,1)) });
	//imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_LEFT), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(-1,0)) });
	//imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_RIGHT), dae::InputCommand{ new MoveCommand(*go.get(), glm::vec2(1,0)) });

	//go2 = std::make_shared<dae::GameObject>();
	//go2->AddComponent<dae::TextComponent>("AAA", font2);
	//go2->SetLocalPosition(10, 280);

	//txtComp = go2->GetComponent<dae::TextComponent>();
	//healthObserver = std::make_unique<dae::HealthObserver>(txtComp);
	//txtComp->SetText("# Lives: " + std::to_string(healthComponent->GetLives()));

	//healthComponent->GetOnDieEvent()->AddObserver(healthObserver.get());
	//eventManager.AddObserver(std::move(healthObserver));

	//scene.Add(go2);


	//go2 = std::make_shared<dae::GameObject>();
	//go2->AddComponent<dae::TextComponent>("AAA", font2);
	//go2->SetLocalPosition(10, 300);

	//txtComp = go2->GetComponent<dae::TextComponent>();
	//scoreObserver = std::make_unique<dae::ScoreObserver>(txtComp);
	//txtComp->SetText("Score: " + std::to_string(scoreComponent->GetScore()));

	//scoreComponent->OnScoreChanged()->AddObserver(scoreObserver.get());
	//eventManager.AddObserver(std::move(scoreObserver));
	//scene.Add(go2);




	//go = std::make_shared<dae::GameObject>();
	//go->AddComponent<dae::TextComponent>("Use the D-Pad to move P1, X to inflict damage, A and B to pick up pellets", font2);
	//go->SetLocalPosition(10, 130);
	//scene.Add(go);

	//go = std::make_shared<dae::GameObject>();
	//go->AddComponent<dae::TextComponent>("Use WASD to move P2, C to inflict damage, Z and X to pick up pellets", font2);
	//go->SetLocalPosition(10, 150);
	//scene.Add(go);


	inputManager.RegisterGamepad(std::move(gp));
	//inputManager.RegisterGamepad(std::move(gp2));
	inputManager.SetControllerInputMapping(std::move(imc), 0);
	//inputManager.SetControllerInputMapping(std::move(imc2), 1);
	inputManager.SetKeyboardInputMapping(std::move(imk));
}

int main(int, char*[])
{
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
