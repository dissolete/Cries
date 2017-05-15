/*
 * Grid.cpp
 *
 *  Created on: Apr 28, 2017
 *      Author: hrumjahn
 */

#include <Grid.h>
#include <math.h>
#include <climits>
#include <fstream>
#include <iostream>

GridParams::GridParams()
{
	id = -99;
	this->clear = true;
}

GridParams::GridParams( int gID, int row, int col, bool isClear)
{
	this->clear = isClear;
	this->rowCoordinate = row;
	this->colCoordinate = col;
	this->entity = NULL;

	// Check pathing
	if( clear )
		this->contains = '.';

	else
		this->contains = 'W';

	parent = NULL;
	F = G = H = INT_MAX;

}

GridParams::~GridParams()
{
	// Nothing to see here
}

void GridParams::setID( int i )
{
	this->id = i;
}

void GridParams::setRow( int r )
{
	this->rowCoordinate = r;
}

void GridParams::setCol( int c )
{
	this->colCoordinate = c;
}

int GridParams::getId()
{
	return id;
}

int GridParams::getRow()
{
	return rowCoordinate;
}

int GridParams::getCol()
{
	return colCoordinate;
}

Ogre::Vector3 GridParams::getCurrentPosition( int rows, int cols )
{
	Ogre::Vector3 position;

	position.x = (colCoordinate * GRID_MULT) - (rows * GRID_MULT)/2.0 + (GRID_MULT/2.0);
	position.y = 0;
	position.z = (rowCoordinate * GRID_MULT) - (rows * GRID_MULT)/2.0 + (GRID_MULT/2.0);

	return position;
}

void GridParams::walkable()
{
	this->clear = true;
	this->contains = '.';

}

void GridParams::notWalkable()
{
	this->clear = false;
	this->contains = 'W';
}

bool GridParams::isWalkable()
{
	return this->clear;
}

int GridParams::getG()
{
	return G;
}

int GridParams::getH()
{
	return H;
}

int GridParams::getF()
{
	return F;
}

void GridParams::setG(int nG)
{
	G = nG;
	F = G + H;
}

void GridParams::setH(int nH)
{
	H = nH;
	F = G + H;
}

void GridParams::setF(int nG, int nH)
{
	G = nG;
	H = nH;
	F = G + H;
}

GridParams* GridParams::getParent()
{
	return parent;
}

void GridParams::setParent(GridParams *nParent)
{
	parent = nParent;
}

void GridParams::resetAStarParams()
{
	parent = NULL;
	F = G = H = INT_MAX;
}

bool GridParams::operator() (GridParams *lhs, GridParams *rhs)
{
	if(lhs == rhs)
	{
		return false;
	} else
	{
		return lhs -> F < rhs -> F;
	}
}

////////////////////////////////////////////////////////////////////
/////// GRID ROW CLASS ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
GridRow::GridRow( int size )
{
	data.resize(size);
}

GridRow::~GridRow()
{

}

////////////////////////////////////////////////////////////////////
/////// GRID CLASS ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
Grid::Grid(Engine* eng) : Mgr(eng)
{
	mSceneMgr = NULL;
	rowNum = 0;
	colNum = 0;
}

Grid::Grid( Ogre::SceneManager* mSceneMgr, int numRows, int numCols, Engine* eng ) : Mgr(eng)
{
	this->mSceneMgr = mSceneMgr;

	//assert(numRows > 0 && numCols > 0);
	this->rowNum = numRows;
	this->colNum = numCols;

	data.resize( numCols, GridRow(numRows));

	// Set coordinates in each grid location
	int index = 0;
	for( int i = 0; i < numRows; i++ )
	{
		for( int j = 0; j < numCols; j++)
		{
			GridParams* n = this->getGrid(i, j);
			n->setRow(i);
			n->setCol(j);
			n->setID( index );
			index++;
		}
	}
}

Grid::~Grid()
{

}

void Grid::tick(float dt)
{

}

void Grid::init()
{

}

void Grid::loadLevel()
{

}

void Grid::stop()
{

}

int Grid::getRowCount()
{
	return this->rowNum;
}

int Grid::getColCount()
{
	return this->colNum;
}

GridParams* Grid::getGrid( int rows, int cols )
{
	if( rows >= rowNum || cols >= colNum || rows < 0 || cols < 0 )
		return NULL;

	return &this->data[cols].data[rows];
}

std::vector<GridParams*> Grid::getNeighbors( GridParams* n )
{
	std::vector<GridParams*> neighbors;

	neighbors.push_back(this->getNW(n));
	neighbors.push_back(this->getNorth(n));
	neighbors.push_back(this->getNE(n));
	neighbors.push_back(this->getEast(n));
	neighbors.push_back(this->getSE(n));
	neighbors.push_back(this->getSouth(n));
	neighbors.push_back(this->getSW(n));
	neighbors.push_back(this->getWest(n));

	return neighbors;
}

GridParams* Grid::getNorth( GridParams* n )
{
	if(n == NULL)
		return NULL;

	return this->getGrid(n->getRow()-1, n->getCol());
}

GridParams* Grid::getSouth( GridParams* n )
{
	if(n == NULL)
		return NULL;
	return this->getGrid(n->getRow()+1, n->getCol());
}

GridParams* Grid::getWest( GridParams* n )
{
	if(n == NULL)
			return NULL;
	return this->getGrid(n->getRow(), n->getCol()-1);
}

GridParams* Grid::getEast( GridParams* n )
{
	if(n == NULL)
			return NULL;
	return this->getGrid(n->getRow(), n->getCol()+1);
}

GridParams* Grid::getNE( GridParams* n )
{
	return this->getEast(this->getNorth(n));
}

GridParams* Grid::getNW( GridParams* n )
{
	return this->getWest(this->getNorth(n));
}

GridParams* Grid::getSE( GridParams* n )
{
	return this->getEast(this->getSouth(n));
}

GridParams* Grid::getSW( GridParams* n )
{
	return this->getWest(this->getSouth(n));
}

int Grid::getDistanceBetweenGrid( GridParams* n1, GridParams* n2 )
{
	if(n1 == NULL || n2 == NULL)
	{
		return -1;
	}

	int deltaX = abs(n2->getRow() - n1->getRow());
	int deltaY = abs(n2->getCol() - n1->getCol());

	return (int)(10 * (std::max(deltaX, deltaY) +
								0.41 * std::min(deltaX,deltaY)));
}


// Load object at given location
void Grid::loadObject( std::string name, std::string mesh, int row, int col,
		Ogre::Vector3 offset, float orientation, float scale )
{
	if (row >= rowNum || col >= colNum || row < 0 || col < 0)
		return;
/*
	Ogre::Entity *ent = mSceneMgr->createEntity(name, mesh);

    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name,
        Ogre::Vector3::ZERO);

    node->attachObject(ent);
    node->setScale(scale, scale, scale);
	node->yaw(Ogre::Degree(orientation));

	GridParams* gp = this->getGrid(row, col);
	node->setPosition(this->getPosition(gp) + offset);

	gp->notWalkable();
	gp->entity = ent;
	*/
}


// Get position in 3d space
Ogre::Vector3 Grid::getPosition( GridParams* n )
{
	return this->getPosition(n->getRow(), n->getCol());
}

Ogre::Vector3 Grid::getPosition( int row, int col )
{
	Ogre::Vector3 position;
	position.z = (row * GRID_MULT) - (this->rowNum * GRID_MULT)/2.0; //+ GRID_MULT/2.0;
	position.y = 5;
	position.x = (col * GRID_MULT) - (this->colNum * GRID_MULT)/2.0; //+ GRID_MULT/2.0;

	return position;
}

// Get Closest Grid area
GridParams* Grid::getPos( Ogre::Vector3 position )
{

	// Closest Row Value
//	int r = clamp(round((position.z - GRID_MULT/2.0 + (this->rowNum * GRID_MULT)/2.0) /
//		float(GRID_MULT)), 0, this->rowNum - 1);
//
//	int c = clamp(round((position.x - GRID_MULT/2.0 + (this->colNum * GRID_MULT)/2.0) /
//		float(GRID_MULT)), 0, this->colNum - 1);

	int r = (position.z + GRID_MULT/2.0)/(GRID_MULT) + this->rowNum/2;
	int c = (position.x + GRID_MULT/2.0)/(GRID_MULT) + this->colNum/2;

	//std::cerr << "The row and column calculated is: " << r << " " << c << std::endl;
	//std::cerr << "Rows: " << this->rowNum << ", Cols: " << this->colNum << std::endl;

	return this->getGrid(r, c);
}

// Find a path to walk in map
std::list<GridParams*> Grid::findPath( GridParams* start, GridParams* end )
{
	std::list<GridParams*> path;

	if(start == NULL || end == NULL)
	{
		std::cout << "Ended at start == NULL or end == NULL" << std::endl;
		return path;
	} else
	{
		AStarPriorityQueue open = AStarPriorityQueue();
		AStarPriorityQueue closed = AStarPriorityQueue();

		start->setParent(NULL);
		start->setF(0, getDistanceBetweenGrid(start, end));

		GridParams *currentNode = start;
		while(currentNode == start || !(open.empty()))
		{
			if(!(open.empty()))
			{
				currentNode = *(open.begin()); //Gets the first sorted node
				open.erase(open.begin()); //Pop from the queue
			}

			//If we are at the destination, we are done
			if(currentNode == end)
			{
				break;
			}

			std::vector<GridParams*> neighbors = getNeighbors(currentNode);

			//Now go through each neighbor
			for(int i = 0; i < 8; i++)
			{
				GridParams * neighbor = neighbors[i];

				if(neighbor == NULL)
				{
					continue;
				}
				//If you can't walk on the neighbor
				if(!(neighbor->isWalkable()))
				{
					continue;
				}

				//Do not cut corners if ortho is blocked
				if(i % 2 == 0 && (!(neighbors[(i + 7) % 8]->isWalkable()) ||
						!(neighbors[(i + 1) % 8]->isWalkable())))
				{
					continue;
				}

				//Calculate the new G value
				int newG = currentNode->getG() + ((i % 2) ? (10) : (14));

				//If the node has already been explored
				auto closedNeighbor = findGridNode(closed, neighbor);
				if(closedNeighbor != closed.end())
				{
					//Update closed with the new path
					if(newG < (*closedNeighbor)->getG())
					{
						closed.erase(closedNeighbor);
						neighbor->setF(newG, this->getDistanceBetweenGrid(neighbor, end));
						neighbor->setParent(currentNode);
						closed.insert(neighbor);
					}
					continue;
				}

				auto prevNeighbor = findGridNode(open, neighbor);
				//If neighbor not in open or node in open's G is greater
				if(prevNeighbor == open.end() || newG < (*prevNeighbor)->getG())
				{
					if(prevNeighbor != open.end())
					{
						open.erase(prevNeighbor);
					}

					neighbor->setF(newG, this->getDistanceBetweenGrid(neighbor, end));
					neighbor->setParent(currentNode);
					open.insert(neighbor);
				}
			}

			closed.insert(currentNode);
		}

		if(currentNode != end)
		{
			std::cout << "Ending at currentNode != end" << std::endl;
			std::cout << path.front() << std::endl;
			std::cout << "Path: " << &path << std::endl;
			return path;
		}

		//Recreate the shortest path
		std::cout << "Made it to path recreation" << std::endl;
		while(currentNode != NULL)
		{
			path.push_front(currentNode);
			currentNode = currentNode->getParent();
		}

		//Reset all the nodes we used during the search
		for(auto iter = open.begin(); iter != open.end(); iter++)
		{
			(*iter)->resetAStarParams();
		}
		for(auto iter = closed.begin(); iter != closed.end(); iter++)
		{
			(*iter)->resetAStarParams();
		}
	}

	return path;
}

AStarPriorityQueue::iterator findGridNode(AStarPriorityQueue &pq, GridParams *gn)
{
	auto first = pq.begin();
	auto last = pq.end();
	while(first != last)
	{
		if(*first == gn)
		{
			return first;
		}
		++first;
	}
	return last;
}





