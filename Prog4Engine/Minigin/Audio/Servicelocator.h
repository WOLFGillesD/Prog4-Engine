#pragma once
#include <memory>
#include <stdexcept>

#include "Observer.h"
#include "SoundSystem.h"


namespace dae
{
	class Servicelocator final
	{
	public:
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
		{
			m_SoundSystemInstance = std::move(soundSystem);
		}

		static SoundSystem& GetSoundSystem()
		{
			if (!m_SoundSystemInstance)
			{
				throw std::runtime_error("Sound system not registered");
			}
			return *m_SoundSystemInstance;
		}

	private:
		//static std::unique_ptr<sound_system> m_ss_instance;
		inline static std::unique_ptr<SoundSystem> m_SoundSystemInstance;
	};

	class SoundObserver : public Observer
	{
		SoundID m_SoundID;
	public:
		SoundObserver(SoundID soundID) : m_SoundID(soundID) {}
		void OnTrigger(GameObject*) override
		{
			Servicelocator::GetSoundSystem().Play(m_SoundID);
		}
	};
	
}
