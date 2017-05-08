#include "soundMgr.h"

SoundMgr::SoundMgr(Engine * engine)
	: Mgr(engine)
	, soundEngine(nullptr)
	, m_soundMap()
	, m_songMap()
	, soundEngineCreated(false)
{

}

SoundMgr::~SoundMgr()
{

}


void SoundMgr::init()
{
	// Instantiate irrklang sound engine
	soundEngine = irrklang::createIrrKlangDevice();

	// Set flag if the sound engine was created properly
	if(soundEngine) soundEngineCreated = true;
	else std::cerr << "SoundMgr attempted to create sound engine in init() but failed. The sound engine was not created!" << std::endl;

}
void SoundMgr::tick(float dt)
{

}
void SoundMgr::loadLevel()
{
	//load_song("Menu", "resources/Cries - Theme.wav");
}
void SoundMgr::stop()
{
	// Cleanup sound engine
	if(soundEngineCreated) soundEngine->drop();

	soundEngineCreated = false;
}

void SoundMgr::load_song(std::string songName, std::string filepath)
{
	// Preload song if sound engine was created
	if(soundEngineCreated)
	{
		if(m_songMap.find(songName) == m_songMap.end())
		{
			irrklang::ISound * newSong = soundEngine->play2D(filepath.c_str(), false, true, true);

			// Check if sound was preloaded correctly
			if(newSong) m_songMap.insert( std::pair<std::string, irrklang::ISound*>(songName, newSong));
			else std::cerr << "The song " << songName << " failed to load! Check if " << filepath << " is a valid filepath." << std::endl;
		}
		else
			std::cerr << "Attempt to load duplicate song " << songName << "!" << std::endl;
	}
	else
		std::cerr << "The song " << songName << " could not be loaded because the sound engine was not created!" << std::endl;
}
void SoundMgr::load_sound(std::string soundName, std::string filepath)
{
	if(soundEngineCreated)
	{
		if(m_soundMap.find(soundName) == m_soundMap.end())
		{
			irrklang::ISound * newSound = soundEngine->play2D(filepath.c_str(), false, true, true);

			// Check if sound was preloaded correctly
			if(newSound) m_soundMap.insert( std::pair<std::string, irrklang::ISound*>(soundName, newSound));
			else std::cerr << "The sound " << soundName << " failed to load! Check if " << filepath << " is a valid filepath." << std::endl;
		}
		else
			std::cerr << "Attempt to load duplicate sound " << soundName << "!" << std::endl;
	}
	else
		std::cerr << "The song " << soundName << " could not be loaded because the sound engine was not created!" << std::endl;
}

void SoundMgr::play_sound2D(std::string soundName, bool looped)
{

	if(soundEngineCreated)
	{
		// Check if sound exists
		if(m_soundMap.find(soundName) != m_soundMap.end())
		{
			irrklang::ISound * sound = m_soundMap[soundName];

			// Make sure this isn't null, god forbid a segfault happens here that would be really annoying
			if(sound)
				soundEngine->play2D(sound->getSoundSource(), looped, false, true);
			else
				std::cerr << "For some reason, the sound returned from the map for " << soundName << " was null. Skipping..." << std::endl;
		}
		else
			std::cerr << "Failed to play 2D sound " << soundName << " because the sound could not be found in the sound map!" << std::endl;
	}
	else
		std::cerr << "Failed to play 2D sound " << soundName << " because the sound engine was not created!" << std::endl;

}

void SoundMgr::play_song2D(std::string songName, bool looped)
{
	if(soundEngineCreated)
	{
		// Check if the song exists
		if(m_songMap.find(songName) != m_songMap.end())
		{
			irrklang::ISound * song = m_songMap[songName];

			if(song)
				soundEngine->play2D(song->getSoundSource(), looped, false, true);
			else
				std::cerr << "For some reason, the song returned from the map for " << songName << " was null. Skipping..." << std::endl;
		}
		else
			std::cerr << "Failed to play 2D song " << songName << " because the song could not be found in the song map!" << std::endl;
	}
	else
		std::cerr << "Failed to play 2D song " << songName << " because the sound engine was not created!" << std::endl;
}

void SoundMgr::play_sound3D(std::string soundName, bool looped, Ogre::Vector3 pos, Ogre::Vector3 lookVector)
{
	if(soundEngineCreated)
	{
		// Check if sound exists
		if(m_soundMap.find(soundName) != m_soundMap.end())
		{
			irrklang::ISound * sound = m_soundMap[soundName];

			// Make sure this isn't null, god forbid a segfault happens here that would be really annoying
			if(sound)
			{
				irrklang::vec3df position(pos.x, pos.y, pos.z);
				//irrklang::vec3df look(lookVector.x, lookVector.y, lookVector.z);
				//irrklang::vec3df velPerSecond(0, 0, 0);
				//irrklang::vec3df upVector(0,1,0);

				soundEngine->play3D(sound->getSoundSource(), position, looped);
			}
			else
				std::cerr << "For some reason, the sound returned from the map for " << soundName << " was null. Skipping..." << std::endl;
		}
		else
			std::cerr << "Failed to play 3D sound " << soundName << " because the sound could not be found in the sound map!" << std::endl;
	}
	else
		std::cerr << "Failed to play 3D sound " << soundName << " because the sound engine was not created!" << std::endl;
}

void SoundMgr::stop_song(std::string songName)
{
	if(soundEngineCreated)
	{
		if(m_songMap.find(songName) != m_songMap.end())
		{
			irrklang::ISound * song = m_songMap[songName];

			if(song and not song->isFinished()){
				//soundEngine->removeSoundSource(song->getSoundSource());
				song->stop();
				//song->drop();
			}
			else std::cerr << "Could not stop song " << songName << " because the song was null in the song map!" << std::endl;
		}
	}
	else
		std::cerr << "Could not stop song " << songName << " because the sound engine was not created!" << std::endl;
}

void SoundMgr::stop_sound(std::string soundName)
{
	if(soundEngineCreated)
		{
			if(m_soundMap.find(soundName) != m_soundMap.end())
			{
				irrklang::ISound * sound = m_soundMap[soundName];

				if(sound and not sound->isFinished()){
					//soundEngine->removeSoundSource(sound->getSoundSource());
					sound->stop();
					//sound->drop();
				}
				else std::cerr << "Could not stop sound " << soundName << " because the sound was null in the sound map!" << std::endl;
			}
		}
		else
			std::cerr << "Could not stop sound " << soundName << " because the sound engine was not created!" << std::endl;
}

