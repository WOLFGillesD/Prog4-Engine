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

#include "Scenes/SceneLoader.h"

namespace fs = std::filesystem;

int main(int, char*[])
{
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
	dae::Minigin engine(data_location);
	game::LoadMenu();
	engine.Run();
    return 0;
}
