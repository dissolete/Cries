/*
 * gameMgr.h
 *
 *  Created on: Mar 11, 2017
 *      Author: sushil
 */

#ifndef GAMEMGR_H_
#define GAMEMGR_H_

#include <mgr.h>
#include <OgreMeshManager.h>


class GameMgr : public Mgr {

private:
	void createEnts();
	void createSky();
	void createGround(int &width, int &heigth, std::string &material);

	// Loads a level given from file
	void loadLevel(std::string levelFilename);


public:
	GameMgr(Engine *engine);
	~GameMgr();
	virtual void init();
	virtual void tick(float dt);
	virtual void loadLevel();
	virtual void stop();

	// Environment Setup
	void loadEnvironment(std::string levelFilename);
	void setupEnvironment();
	void loadObjects();
	void loadCharacters();

	Ogre::Plane ocean;

};



#endif /* GAMEMGR_H_ */
