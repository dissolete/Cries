#ifndef __VECTOR_MATH_H_INCLUDED
#define __VECTOR_MATH_H_INCLUDED

#include <OgreMath.h>
#include <OgreVector2.h>
#include <OgreVector3.h>

Ogre::Vector3 getReflectionVector(Ogre::Vector3 incoming, Ogre::Vector3 normal);

Ogre::Vector3 getWallNormal(Ogre::Vector2 initialPos, Ogre::Vector2 wallPosition);

#endif
