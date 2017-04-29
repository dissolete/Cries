/*
 * Grid.h
 *
 *  Created on: Apr 28, 2017
 *      Author: hrumjahn
 */

#ifndef INC_GRID_H_
#define INC_GRID_H_

#include <iostream>
#include <vector>
#include <assert.h>
#include <GfxMgr.h>
#include <Utils.h>
#include <mgr.h>

#define GRID_MULT 10.0

class Engine;

class GridParams {
protected:
	int id;
	int rowCoordinate;
	int colCoordinate;
	bool clear;

	// May not need
	GridParams* parent;
	int G, H, F;

public:
	Ogre::Entity* entity; // pointer to entity in this grid

	GridParams();
	GridParams( int gID, int row, int col, bool isClear = true);
	~GridParams();

	void setID( int i );
	void setRow( int r );
	void setCol( int c );

	int getId();
	int getRow();
	int getCol();

	// Get Position of object in grid
	Ogre::Vector3 getCurruntPosition( int rows, int cols );
	void walkable();
	void notWalkable();
	bool isWalkable();

	char contains;
};

class GridRow {
public:
	std::vector<GridParams> data;
	GridRow( int size );
	~GridRow();
};

class Grid :public Mgr {
private:
	Ogre::SceneManager* mSceneMgr;
	std::vector<GridRow> data;
	int rowNum;
	int colNum;

public:
	Grid(Engine* eng);
	Grid( Ogre::SceneManager* mSceneMgr, int numRows, int numCols, Engine* eng );
	~Grid();

	virtual void tick(float dt);
	virtual void init();
	virtual void loadLevel();
	virtual void stop();

	int getRowCount();
	int getColCount();

	GridParams* getGrid( int rows, int cols );

	// Get Grid Positions around location
	std::list<GridParams*> *getNeighbors( GridParams* n );
	GridParams* getNorth( GridParams* n );
	GridParams* getSouth( GridParams* n );
	GridParams* getWest( GridParams* n );
	GridParams* getEast( GridParams* n );
	GridParams* getNE( GridParams* n );
	GridParams* getNW( GridParams* n );
	GridParams* getSE( GridParams* n );
	GridParams* getSW( GridParams* n );

	int getDistanceBetweenGrid( GridParams* n1, GridParams* n2 );

	// Load object at given location
	void loadObject( std::string name, std::string mesh, int row, int col,
			Ogre::Vector3 offset, float orientation, float scale = 1 );

	// Get position in 3d space
	Ogre::Vector3 getPosition( GridParams* n );
	Ogre::Vector3 getPosition( int row, int col );

	// Get Closest Grid area
	GridParams* getPos( Ogre::Vector3 position );

	std::list<GridParams*> findPath( GridParams* start, GridParams* goal );

};

#endif /* INC_GRID_H_ */
