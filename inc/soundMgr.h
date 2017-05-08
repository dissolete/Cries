
#ifndef __SOUNDMGR_H_INCLUDED
#define __SOUNDMGR_H_INCLUDED

#include <string>
#include <iostream>
#include <map>
#include <OgreVector3.h>
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

	void play_sound2D(std::string soundName, bool looped = false);
	void play_song2D(std::string songName, bool looped = false);

	void play_sound3D(std::string soundName, bool looped, Ogre::Vector3 positionRelativeToCamera, Ogre::Vector3 lookVector);

	void stop_sound(std::string soundName);
	void stop_song(std::string soundName);

private:

	irrklang::ISoundEngine * soundEngine;

	std::map<std::string, irrklang::ISound*> m_soundMap, m_songMap;

	bool soundEngineCreated;
};


#endif
