/*
 * gameMgr.h
 *
 *  Created on: Mar 11, 2017
 *      Author: sushil
 */

#ifndef GAMEMGR_H_
#define GAMEMGR_H_

#include <mgr.h>
#include <Grid.h>
#include <Utils.h>
#include <OgreMeshManager.h>
#include <string>

class Grid;
class GridParams;

// Constants for Grid Array Size
const int COL_SIZE = 20;
const int ROW_SIZE = 17;


class GameMgr : public Mgr {

protected:
	Grid* grid;

private:
	void createSky();
	void createGround(int width, int heigth, std::string &material);
	void createCeiling();

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
	void setupSounds();
	void loadObjects();
	void loadCharacters();

	std::string getNewName();

	Ogre::Plane floor;
	Ogre::Plane ceiling;
	float gameplayTime;

	Grid* getGrid() const;

	std::vector<Ogre::SceneNode*> entitySceneNodes;

	int entityCount;

};



#endif /* GAMEMGR_H_ */
