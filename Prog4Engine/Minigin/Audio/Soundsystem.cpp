#include "Soundsystem.h"

#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>

#include <SDL.h>
#include <SDL_mixer.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace dae
{
	class SDLSoundSystem::SDLSoundSystemImpl : public SoundSystem
	{
	public:
		SDLSoundSystemImpl()
		{
			// Initialize SDL audio subsystem
			//if (SDL_Init(SDL_INIT_AUDIO) < 0)
			//{
			//	throw std::runtime_error("Failed to initialize SDL audio");
			//}

			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
			{
				throw std::runtime_error(Mix_GetError());
			}

			m_AudioThread = std::jthread{ &SDLSoundSystemImpl::AudioLoop, this };
		}

		~SDLSoundSystemImpl() override
		{
			Mix_CloseAudio();

			for (auto& chunk : m_Sounds)
			{
				Mix_FreeChunk(chunk);
			}
			m_Sounds.clear();
			m_IsRunning = false;

			m_AudioThread.request_stop();
			m_AudioThread.join();
		}

		SoundID LoadSound(const std::string& path) override
		{
			// Load sound file
			Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
			if (!sound)
			{
				throw std::runtime_error(Mix_GetError());
			}
			m_Sounds.push_back(sound);
			return static_cast<SoundID>(m_Sounds.size() - 1);
		}

		void Play(SoundID soundID) override
		{
			if (soundID < m_Sounds.size())
			{
				Mix_PlayChannel(-1, m_Sounds[soundID], 0);
			}
			else
			{
				throw std::out_of_range("Invalid sound ID");
			}
		}

	private:

		void AudioLoop()
		{
			while (m_IsRunning)
			{
				std::unique_lock<std::mutex> lock(m_AudioMutex);
				if (!m_SoundQueue.empty())
				{
					SoundID soundID = m_SoundQueue.front();
					m_SoundQueue.pop();
					lock.unlock();

					if (soundID < m_Sounds.size())
					{
						Mix_PlayChannel(-1, m_Sounds[soundID], -1);
					}

					// SDL_QueueAudio(...);
				}
				else
				{
					lock.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			}
		}

		bool m_IsRunning{ true };

		std::jthread m_AudioThread;
		std::mutex m_AudioMutex;

		std::queue<SoundID> m_SoundQueue;
		std::vector<Mix_Chunk*> m_Sounds;

	};

	SDLSoundSystem::SDLSoundSystem()
	{
		m_pImpl = new SDLSoundSystemImpl{};
	}

	SDLSoundSystem::~SDLSoundSystem()
	{
		delete m_pImpl;
	}

	SoundID SDLSoundSystem::LoadSound(const std::string& path)
	{
		return m_pImpl->LoadSound(path);
	}

	void SDLSoundSystem::Play(SoundID soundID)
	{
		m_pImpl->Play(soundID);
	}


	// =============================================
	// LOGGER
	// =============================================
	class SoundSystemLogger::SoundSystemLoggerImpl : public SoundSystem
	{
		std::shared_ptr<spdlog::logger> m_fileLogger;
		int m_loadRequests{};

	public:
		SoundSystemLoggerImpl()
		{
			 m_fileLogger = spdlog::basic_logger_mt("file_logger", "logs/my_log.txt");
		}

		SoundID LoadSound(const std::string& filename) override
		{
			std::stringstream ss{};
			ss << "User played audio: " << filename;

			spdlog::info(ss.str());
			m_fileLogger->info(ss.str());
			return m_loadRequests++;
		}

		void Play(SoundID soundID) override
		{
			std::stringstream ss{};
			ss << "User requesting to play sound: " << soundID;

			spdlog::info(ss.str());
			m_fileLogger->info(ss.str());
		}
	};

	SoundSystemLogger::SoundSystemLogger()
	{
		m_pImpl = new SoundSystemLoggerImpl{};
	}

	SoundSystemLogger::~SoundSystemLogger()
	{
		delete m_pImpl;
	}

	SoundID SoundSystemLogger::LoadSound(const std::string& path)
	{
		return m_pImpl->LoadSound(path);
	}

	void SoundSystemLogger::Play(SoundID soundID)
	{
		m_pImpl->Play(soundID);
	}
}
