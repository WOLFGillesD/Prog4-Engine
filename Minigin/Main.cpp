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

#include "Components/FpsComponent.h"
#include "Components/RotatorComponent.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Components/ThrashAndCacheComponent.h"
namespace fs = std::filesystem;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

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

	auto go2 = std::make_shared<dae::GameObject>();
	go2->AddComponent<dae::RotatorComponent>();
	go2->AddComponent<dae::TextureComponent>("CEMERALD.tga");
	go2->SetParent(go.get(), false);

	//go->AddComponent<dae::RotatorComponent>();
	auto go3 = std::make_shared<dae::GameObject>();
	go3->AddComponent<dae::TextureComponent>("CEMERALD.tga");
	go3->AddComponent<dae::RotatorComponent>(25.f, static_cast<float>(2*M_PI));
	go3->SetParent(go2.get(), false);

	scene.Add(go);
	scene.Add(go2);
	scene.Add(go3);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::ThrashAndCacheComponent>();

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
