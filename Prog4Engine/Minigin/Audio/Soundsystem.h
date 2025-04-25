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

		SDLSoundSystem(SDLSoundSystem&) = delete;
		SDLSoundSystem(SDLSoundSystem&&) = delete;
		SDLSoundSystem& operator=(SDLSoundSystem&) = delete;
		SDLSoundSystem& operator=(SDLSoundSystem&&) = delete;


		SoundID LoadSound(const std::string& path) override;

		void Play(SoundID soundID) override;
	};

	class SoundSystemLogger final : public SoundSystem
	{
		class SoundSystemLoggerImpl;
		SoundSystemLoggerImpl* m_pImpl;

	public:
		SoundSystemLogger();
		~SoundSystemLogger() override;

		SoundSystemLogger(SoundSystemLogger&) = delete;
		SoundSystemLogger(SoundSystemLogger&&) = delete;
		SoundSystemLogger& operator=(SoundSystemLogger&) = delete;
		SoundSystemLogger& operator=(SoundSystemLogger&&) = delete;

		SoundID LoadSound(const std::string& path) override;

		void Play(SoundID soundID) override;
	};
}
