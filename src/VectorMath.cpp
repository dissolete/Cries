#include <VectorMath.h>


Ogre::Vector3 getReflectionVector(Ogre::Vector3 incoming, Ogre::Vector3 normal)
{
	Ogre::Vector3 calculatedVector = Ogre::Vector3::ZERO;

	// Check normal

	// If left or right wall
	if(normal.z == 0)
	{
		calculatedVector.z = incoming.z;
	}
	// or top and bottom wall
	else
		calculatedVector.x = incoming.x;


	return calculatedVector;
}

Ogre::Vector3 getWallNormal(Ogre::Vector2 initialPos, Ogre::Vector2 wallPosition)
{
	Ogre::Vector3 normal = Ogre::Vector3::ZERO;

	// If coming from the left
	if(initialPos.x < wallPosition.x)
	{
		normal.z = -1;
	}
	else if(initialPos.x > wallPosition.x)
		normal.z = 1;
	// If coming from the bottom
	else if(initialPos.y < wallPosition.y)
		normal.x = -1;
	else if(initialPos.y > wallPosition.y)
		normal.x = 1;

	return normal;
}
