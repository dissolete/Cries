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
	ARCH
};

enum class COMMAND_TYPE{
	SEARCH,
	PURSUE
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
	GAMEPLAY
};

enum class Status {
	WAITING,
	ALERTED,
	SEARCHING
};

#endif /* TYPES_H_ */
