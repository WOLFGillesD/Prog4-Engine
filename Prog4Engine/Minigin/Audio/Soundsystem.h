#pragma once 
#include <memory>
#include <string>

namespace dae
{
	using SoundID = unsigned int;
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual SoundID LoadSound(const std::string&) = 0;
		virtual void Play(SoundID soundID) = 0;
	private:

	};

	class SDLSoundSystem final : public SoundSystem
	{
		class SDLSoundSystemImpl;
		SDLSoundSystemImpl* m_pImpl;

	public:
		SDLSoundSystem();
		~SDLSoundSystem() override;

		SoundID LoadSound(const std::string& path) override;

		void Play(SoundID soundID) override;
	};
}
