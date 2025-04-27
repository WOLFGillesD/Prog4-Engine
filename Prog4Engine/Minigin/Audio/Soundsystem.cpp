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
			m_CkeckForWork.notify_one();

			m_AudioThread.request_stop();
			m_AudioThread.join();
		}

		SDLSoundSystemImpl(SDLSoundSystemImpl&) = delete;
		SDLSoundSystemImpl(SDLSoundSystemImpl&&) = delete;
		SDLSoundSystemImpl& operator=(SDLSoundSystemImpl&) = delete;
		SDLSoundSystemImpl& operator=(SDLSoundSystemImpl&&) = delete;

		SoundID LoadSound(const std::string& path) override
		{
			// Load sound file
			std::lock_guard lock{ m_AudioMutex };
			std::string test = path;
			m_LoadQueue.push(test);
			m_CkeckForWork.notify_one();
			return m_registerdSounds++;

		}

		void Play(SoundID soundID) override
		{
			std::lock_guard lock{ m_AudioMutex };
			m_CkeckForWork.notify_one();
			m_SoundQueue.push(soundID);
		}

	private:

		void AudioLoop()
		{
			while (m_IsRunning)
			{
				std::unique_lock<std::mutex> lock(m_AudioMutex);
				while(!m_LoadQueue.empty())
				{
					std::string path = m_LoadQueue.front();
					m_LoadQueue.pop();

					Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
					if (!sound)
					{
						throw std::runtime_error(Mix_GetError());
					}
					m_Sounds.push_back(sound);
				}

				while(!m_SoundQueue.empty())
				{
					SoundID soundID = m_SoundQueue.front();
					m_SoundQueue.pop();

					if (soundID < m_Sounds.size())
					{
						Mix_PlayChannel(-1, m_Sounds[soundID], 0);
					}
					else
					{
						m_SoundQueue.push(soundID);
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}

				}
				m_CkeckForWork.wait(lock,[&]{ return !m_LoadQueue.empty() || !m_SoundQueue.empty() || !m_IsRunning; });
			}
		}

		bool m_IsRunning{ true };
		std::condition_variable m_CkeckForWork;

		std::jthread m_AudioThread;
		std::mutex m_AudioMutex;

		std::queue<std::string> m_LoadQueue{};
		std::queue<SoundID> m_SoundQueue{};
		std::vector<Mix_Chunk*> m_Sounds{};

		SoundID m_registerdSounds{};

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
