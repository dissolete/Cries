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
	//(normal.x == 0)
	else
	{
		calculatedVector.x = incoming.x;
	}



	return calculatedVector;
}

Ogre::Vector3 getWallNormal(Ogre::Vector2 initialPos, Ogre::Vector2 wallPosition)
{
	Ogre::Vector3 normal = Ogre::Vector3::ZERO;

	// If coming from the left
	if(initialPos.x < wallPosition.x)
	{
		//std::cerr << "if(initialPos.x < wallPosition.x)" << std::endl;
		normal.z = -1;
	}
	else if(initialPos.x > wallPosition.x){
		//std::cerr << "else if(initialPos.x > wallPosition.x)" << std::endl;
		normal.z = 1;
	}
	// If coming from the bottom
	if(initialPos.y < wallPosition.y){
		//std::cerr << "(initialPos.y < wallPosition.y)" << std::endl;
		normal.x = -1;
	}
	else if(initialPos.y > wallPosition.y){
		//std::cerr << "(initialPos.y > wallPosition.y)" << std::endl;
		normal.x = 1;
	}

	return normal;
}

Ogre::Vector3 getPositionRelativeToListener(Ogre::Vector3 listenerPos, Ogre::Vector3 listenerLookVector, Ogre::Vector3 soundSource)
{
	Ogre::Vector3 dis = soundSource - listenerPos;
	Ogre::Vector3 referenceDirection(0, 0, -1);

	Ogre::Radian angleBetween = listenerLookVector.angleBetween(referenceDirection);

	return Ogre::Quaternion(angleBetween, Ogre::Vector3::UNIT_Z) * dis;


}
