
#ifndef __SOUNDMGR_H_INCLUDED
#define __SOUNDMGR_H_INCLUDED

#include <string>
#include <iostream>
#include <map>
#include "mgr.h"
#include "irrKlang.h"

class SoundMgr : public Mgr
{
public:

	SoundMgr(Engine * engine);
	~SoundMgr();

	virtual void init();
	virtual void tick(float dt);
	virtual void loadLevel();
	virtual void stop();

	void load_song(std::string songName, std::string filePath);
	void load_sound(std::string soundName, std::string filePath);

	void play_sound(std::string soundName);
	void play_song(std::string songName, bool looped);

	void stop_sound(std::string soundName);
	void stop_song(std::string soundName);

private:

	irrklang::ISoundEngine * soundEngine;

	std::map<std::string, std::string> m_soundMap, m_songMap;

	bool soundEngineCreated;
};


#endif
