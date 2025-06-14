#pragma once

#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"

#include "ColliderComponent.h"
#include "GridComponent.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "NPCComponents.h"
#include "PlayerComponent.h"
#include "RotatorComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "UIControllerComponent.h"
#include "Audio/Servicelocator.h"
#include "Components/FpsComponent.h"
#include "Input/EventManager.h"


namespace game
{
	inline void LoadLevel01()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Level 1");
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

		//go = std::make_shared<dae::GameObject>();
		//go->AddComponent<dae::TextureComponent>("logo.tga");
		//go->SetLocalPosition(216, 180);
		//scene.Add(go);

		go = std::make_shared<dae::GameObject>();
		go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
		go->SetLocalPosition(80, 20);
		scene.Add(go);

		go = std::make_shared<dae::GameObject>();
		go->AddComponent<dae::TextComponent>("FPS", font);
		go->AddComponent<dae::FpsComponent>();
		go->SetLocalPosition(20, 40);
		scene.Add(go);

		auto scoreIndicator = std::make_shared<dae::GameObject>();
		scoreIndicator->AddComponent<dae::TextComponent>("AAA", font2);
		scoreIndicator->SetLocalPosition(10, 10);
		scene.Add(scoreIndicator);

		auto goGrid = std::make_shared<dae::GameObject>();
		goGrid->AddComponent<game::GridComponent>(10, 15, 40, "Env/VBACK1.png", glm::vec2{ 20, 80 });
		goGrid->GetComponent<game::GridComponent>()->LoadFromCSV("Levels/LVL_00.csv");
		scene.Add(goGrid);

		auto playerGameObj = std::make_shared<dae::GameObject>();
		playerGameObj->AddComponent<game::MovementComponent>(goGrid->GetComponent<game::GridComponent>(), 100.f, glm::ivec2{ 7,9 });

		playerGameObj->AddComponent<dae::SpriteComponent>("Player/PlayerMovement.png", 1, 6, 0, 90.f);
		playerGameObj->AddComponent<dae::ColliderComponent>(glm::vec2(40, 40), glm::vec2(0, 0), "Player");
		playerGameObj->AddComponent<game::ScoreComponent>();
		playerGameObj->AddComponent<game::HealthComponent>();

		//auto scoreObserver = std::make_unique<game::ScoreObserver>(scoreIndicator->GetComponent<dae::TextComponent>());
		//playerGameObj->GetComponent<game::ScoreComponent>()->OnScoreChanged()->AddObserver(scoreObserver.get());

		playerGameObj->AddComponent<game::PlayerComponent>(goGrid->GetComponent<game::GridComponent>()
			, playerGameObj->GetComponent<game::MovementComponent>()
			, scoreIndicator->GetComponent<dae::TextComponent>()
			, playerGameObj->GetComponent<game::ScoreComponent>()
			, playerGameObj->GetComponent<game::HealthComponent>());
		scene.Add(playerGameObj);
		//eventManager.AddObserver(std::move(scoreObserver));

		imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_UP), dae::InputCommand{ new game::MoveCommand(playerGameObj->GetComponent<game::MovementComponent>(), glm::vec2(0,-1)) });
		imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_DOWN), dae::InputCommand{ new game::MoveCommand(playerGameObj->GetComponent<game::MovementComponent>(), glm::vec2(0,1)) });
		imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_LEFT), dae::InputCommand{ new game::MoveCommand(playerGameObj->GetComponent<game::MovementComponent>(), glm::vec2(-1,0)) });
		imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_RIGHT), dae::InputCommand{ new game::MoveCommand(playerGameObj->GetComponent<game::MovementComponent>(), glm::vec2(1,0)) });

		imk->AddCommand(SDLK_w, dae::InputCommand{ new game::MoveCommand(playerGameObj->GetComponent<game::MovementComponent>(), glm::vec2(0,-1)), dae::InputState::IsPressed });
		//imk->AddCommand(SDLK_w, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::PlayerComponent>(), glm::vec2(0,0)), dae::InputState::IsUp });

		imk->AddCommand(SDLK_s, dae::InputCommand{ new game::MoveCommand(playerGameObj->GetComponent<game::MovementComponent>(), glm::vec2(0,1)), dae::InputState::IsPressed });
		//imk->AddCommand(SDLK_s, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::PlayerComponent>(), glm::vec2(0,0)), dae::InputState::IsUp });

		imk->AddCommand(SDLK_a, dae::InputCommand{ new game::MoveCommand(playerGameObj->GetComponent<game::MovementComponent>(), glm::vec2(-1,0)), dae::InputState::IsPressed });
		//imk->AddCommand(SDLK_a, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::PlayerComponent>(), glm::vec2(0,0)), dae::InputState::IsUp });

		imk->AddCommand(SDLK_d, dae::InputCommand{ new game::MoveCommand(playerGameObj->GetComponent<game::MovementComponent>(), glm::vec2(1,0)), dae::InputState::IsPressed });
		//imk->AddCommand(SDLK_d, dae::InputCommand{ new game::MoveCommand(go2->GetComponent<game::PlayerComponent>(), glm::vec2(0,0)), dae::InputState::IsUp });


		// Add all the players
		{
			auto csvData = dae::ResourceManager::GetInstance().LoadCSV("Levels/LVL_00.csv");

			std::stringstream ss(csvData);
			std::string line;
			std::vector<std::vector<int>> parsedGrid;

			while (std::getline(ss, line))
			{
				if (line.empty())
					continue;

				std::stringstream lineStream(line);
				std::string cellStr;
				std::vector<int> rowValues;

				while (std::getline(lineStream, cellStr, ';'))
				{
					// Remove whitespace
					cellStr.erase(std::remove_if(cellStr.begin(), cellStr.end(), isspace), cellStr.end());

					if (cellStr.empty())
						rowValues.push_back(0); // Treat empty as 0
					else
						rowValues.push_back(std::stoi(cellStr));
				}

				parsedGrid.push_back(std::move(rowValues));
			}

			for (int row = 0; row < goGrid->GetComponent<game::GridComponent>()->GetRows(); ++row)
			{
				for (int col = 0; col < goGrid->GetComponent<game::GridComponent>()->GetColumns(); ++col)
				{
					int value = parsedGrid[row][col];
					switch (value)
					{
					case 2:
					{
						auto go2 = std::make_shared<dae::GameObject>();
						go2->AddComponent<dae::TextureComponent>("Env/VEMERALD.png", 1.3f, glm::vec2{ 5,5 });
						go2->SetLocalPosition(goGrid->GetComponent<game::GridComponent>()->GetCellPosition(col, row));
						go2->AddComponent<dae::ColliderComponent>(glm::vec2{ 10,10 }, glm::vec2{ 0,0 }, "Test");
						go2->AddComponent<game::EmeraldComponent>(go2->GetComponent<dae::ColliderComponent>(), playerGameObj->GetComponent<game::ScoreComponent>());
						scene.Add(go2);
					}
					break;
					case 3:
					{
						auto go2 = std::make_shared<dae::GameObject>();
						go2->AddComponent<dae::TextureComponent>("Env/VFBAG.png");
						go2->SetLocalPosition(goGrid->GetComponent<game::GridComponent>()->GetCellPosition(col, row));
						go2->AddComponent<dae::ColliderComponent>(glm::vec2{ 20,20 }, glm::vec2{ 0,0 }, "Test");
						go2->AddComponent<game::BagComponent>(goGrid->GetComponent<game::GridComponent>()
							, go2->GetComponent<dae::ColliderComponent>()
							, playerGameObj->GetComponent<game::ScoreComponent>());
						scene.Add(go2);
					}
					break;
					}
				}
			}
		}

		// NOBIN
		//auto npc1 = std::make_shared<dae::GameObject>();
		//npc1->AddComponent<game::MovementComponent>(goGrid->GetComponent<game::GridComponent>(), 100.f, glm::ivec2{ 14,0 });

		//npc1->AddComponent<dae::SpriteComponent>("Player/PlayerMovement.png", 1, 6, 0, 90.f);
		//npc1->AddComponent<dae::ColliderComponent>(glm::vec2(40, 40), glm::vec2(0, 0), "Enemy");
		//npc1->AddComponent<game::HealthComponent>();

		//npc1->AddComponent<game::Nobbin>(goGrid->GetComponent<game::GridComponent>()
		//	, npc1->GetComponent<game::MovementComponent>()
		//	, playerGameObj->GetComponent<game::PlayerComponent>()
		//	, playerGameObj->GetComponent<game::ScoreComponent>()
		//	, npc1->GetComponent<game::HealthComponent>());

		//scene.Add(npc1);


		// HOBIN
		/*npc1 = std::make_shared<dae::GameObject>();
		npc1->AddComponent<game::MovementComponent>(goGrid->GetComponent<game::GridComponent>(), 100.f, glm::ivec2{ 14,0 });

		npc1->AddComponent<dae::SpriteComponent>("Player/PlayerMovement.png", 1, 6, 0, 90.f);
		npc1->AddComponent<dae::ColliderComponent>(glm::vec2(40, 40), glm::vec2(0, 0), "Enemy");
		npc1->AddComponent<game::HealthComponent>();

		npc1->AddComponent<game::Hobbin>(goGrid->GetComponent<game::GridComponent>()
			, npc1->GetComponent<game::MovementComponent>()
			, playerGameObj->GetComponent<game::PlayerComponent>()
			, playerGameObj->GetComponent<game::ScoreComponent>()
			, npc1->GetComponent<game::HealthComponent>());

		scene.Add(npc1);*/


		inputManager.RegisterGamepad(std::move(gp));
		//inputManager.RegisterGamepad(std::move(gp2));
		inputManager.SetControllerInputMapping(std::move(imc), 0);
		//inputManager.SetControllerInputMapping(std::move(imc2), 1);
		inputManager.SetKeyboardInputMapping(std::move(imk));
	}

	inline void LoadMenu()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Level 1");
		auto& inputManager = dae::InputManager::GetInstance();
		auto imc = std::make_unique<dae::InputMapping>();
		auto imk = std::make_unique<dae::InputMapping>();
		auto gp = std::make_unique<XInputGamepad>(0);
		auto gp2 = std::make_unique<XInputGamepad>(1);
		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);

		auto bg = std::make_shared<dae::GameObject>();
		bg->AddComponent<dae::TextureComponent>("UI/CTITLE.png",2.f, glm::vec2{ 0, 15 });
		bg->SetDepthIndex(10);
		scene.Add(bg);

		auto manager = std::make_shared<dae::GameObject>();
		manager->AddComponent<MainScreenUIManagerComponent>(LoadLevel01, []() {}, []() {});
		scene.Add(manager);

		auto btn1 = std::make_shared<dae::GameObject>();
		btn1->AddComponent<game::UIButtonComponent>(glm::ivec2{145,50}, glm::ivec2{-40, -10});
		btn1->AddComponent<dae::TextComponent>("PVE", font);
		btn1->GetComponent<game::UIButtonComponent>()->GetOnButtonPress()->AddObserver(manager->GetComponent<MainScreenUIManagerComponent>()->GetPVEObserver());
		btn1->SetLocalPosition(450, 110);
		scene.Add(btn1);

		auto btn2 = std::make_shared<dae::GameObject>();
		btn2->AddComponent<game::UIButtonComponent>(glm::ivec2{ 145,50 }, glm::ivec2{ -40, -10 });
		btn2->AddComponent<dae::TextComponent>("PVP", font);
		btn2->SetLocalPosition(450, 180);
		scene.Add(btn2);

		auto btn3 = std::make_shared<dae::GameObject>();
		btn3->AddComponent<game::UIButtonComponent>(glm::ivec2{ 145,50 }, glm::ivec2{ -40, -10 });
		btn3->AddComponent<dae::TextComponent>("Co-Op", font);
		btn3->SetLocalPosition(450, 250);
		scene.Add(btn3);

		auto uiController = std::make_shared<dae::GameObject>();
		uiController->AddComponent<UIControllerComponent>();
		auto uiC = uiController->GetComponent<UIControllerComponent>();
		uiC->RegisterButton(btn1->GetComponent<UIButtonComponent>());
		uiC->RegisterButton(btn2->GetComponent<UIButtonComponent>());
		uiC->RegisterButton(btn3->GetComponent<UIButtonComponent>());
		scene.Add(uiController);

		imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_UP), dae::InputCommand{ new game::UIMoveUICommand(uiC, 1), dae::InputState::IsDown });
		imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_DPAD_DOWN), dae::InputCommand{ new game::UIMoveUICommand(uiC, -1), dae::InputState::IsDown });
		imc->AddCommand(static_cast<unsigned int>(GamePadInput::GAMEPAD_A), dae::InputCommand{ new game::UIPressCommand(uiC), dae::InputState::IsDown });

		imk->AddCommand(SDLK_w, dae::InputCommand{ new game::UIMoveUICommand(uiC, 1) , dae::InputState::IsPressed });
		imk->AddCommand(SDLK_s, dae::InputCommand{ new game::UIMoveUICommand(uiC, -1), dae::InputState::IsPressed });
		imk->AddCommand(SDLK_SPACE, dae::InputCommand{ new game::UIPressCommand(uiC), dae::InputState::IsPressed });


		inputManager.RegisterGamepad(std::move(gp));
		inputManager.SetControllerInputMapping(std::move(imc), 0);
		inputManager.SetKeyboardInputMapping(std::move(imk));
	}
}
