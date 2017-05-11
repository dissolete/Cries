/*
 * Types.h
 *
 *  Created on: Feb 22, 2017
 *      Author: sushil
 */

#ifndef TYPES_H_
#define TYPES_H_


enum class EntityType {
	SEENO,
	HEARNO,
	SPEAKNO,
	WALL,
	ARCH,
	LOGO,
	ENDARCH
};

enum class COMMAND_TYPE{
	SEARCH,
	PURSUE,
	SEARCHPATH,
	PURSUEPATH
};

enum class ASPECT_TYPE {
	PHYSICS,
	RENDERABLE,
	AI,
	NONE
};

enum class STATE {
	SPLASH,
	MAIN_MENU,
	GAMEPLAY,
	GAMEOVER
};

enum class Status {
	WAITING,
	ALERTED,
	SEARCHING
};

enum class DIRECTION {
	SOUTH,
	SOUTHWEST,
	WEST,
	NORTHWEST,
	NORTH,
	NORTHEAST,
	EAST,
	SOUTHEAST
};

enum class COLLISION_SHAPE {
	CIRCULAR,
	SQUARE
};

#endif /* TYPES_H_ */
