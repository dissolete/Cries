#include <VectorMath.h>


Ogre::Vector3 getReflectionVector(Ogre::Vector3 incoming, Ogre::Vector3 normal, Ogre::Vector3 priorNormal)
{
	Ogre::Vector3 calculatedVector = Ogre::Vector3::ZERO;

	if(normal.x == 0 and normal.z == 0)
	{
		// Use prior normal
		if(priorNormal.x == 0)
			calculatedVector.x = incoming.x;
		else
			calculatedVector.z = incoming.z;
	}
	else if(normal.x == 0 and normal.z != 0)
	{
		//std::cerr << "a" << std::endl;

		calculatedVector.x = incoming.x;

	}
	else if(normal.z == 0 and normal.x != 0)
	{
		std::cerr << "b" << std::endl;
		calculatedVector.z = incoming.z;
	}

	else
	{
		//std::cerr << "else reached in getReflectionVector! The normal is "<< normal.x << ", "<< normal.y << ", "<< normal.z << ", "<< std::endl;
		if(priorNormal.x == 0)
			calculatedVector.x = incoming.x;
		else
			calculatedVector.z = incoming.z;
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
