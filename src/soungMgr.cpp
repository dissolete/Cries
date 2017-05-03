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

}
void SoundMgr::tick(float dt)
{

}
void SoundMgr::loadLevel()
{

}
void SoundMgr::stop()
{
	// Cleanup sound engine
	if(soundEngineCreated) soundEngine->drop();

	soundEngineCreated = false;
}

void SoundMgr::load_song(std::string songName, std::string filePath)
{
	m_songMap.insert( std::pair<std::string, std::string>(songName, filePath));
}
void SoundMgr::load_sound(std::string soundName, std::string filePath)
{
	m_soundMap.insert( std::pair<std::string, std::string>(soundName, filePath));
}

void SoundMgr::play_sound(std::string soundName)
{
	if(not soundEngineCreated)
	{
		std::cerr << "Attempt to play sound " << soundName << " failed! The sound engine was not created!" << std::endl;
		return;
	}

	// Check if sound exists
	if(m_soundMap.find(soundName) != m_soundMap.end())
	{
		soundEngine->play2D(m_soundMap[soundName].c_str());
	}
	else
	{
		std::cerr << "Attempt to play sound " << soundName << " failed! The sound could not be found in the sound engine!" << std::endl;
	}

}
void SoundMgr::play_song(std::string songName, bool looped)
{
	if(not soundEngineCreated)
	{
		std::cerr << "Attempt to play song " << songName << " failed! The sound engine was not created!" << std::endl;
	}

	// Check if song exists
	if(m_songMap.find(songName) != m_songMap.end())
	{
		soundEngine->play2D(m_songMap[songName].c_str(), looped);
	}
	else
	{
		std::cerr << "Attempt to play song " << songName << " failed! The song could not be found in the sound engine!" << std::endl;
	}
}

void SoundMgr::stop_song(std::string songName)
{
	// Check if song exists
//	if(not m_songMap.find(songName) != m_songMap.end())
//	{
//
//	}
}

void SoundMgr::stop_sound(std::string soundName)
{

}

