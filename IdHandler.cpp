/*
 * IdHandler.cpp
 *
 *  Created on: Mar 22, 2015
 *      Author: Author: David Gvirts, Shahar Osovsky
 */
#include "IdHandler.h"

/**
 * A constructor for IdHandler, creating a "bank" of maxNum Id's.
 */
IdHandler::IdHandler(unsigned int maxNum): _maxNum(maxNum),_idVec(new std::vector<int>)
{
}
/**
 * A destructor for IdHandler.
 */
IdHandler::~IdHandler()
{
	delete _idVec;
}
/**
 * Returning the lowest available ID.
 */
int IdHandler::getNewId()
{
	for (unsigned int var = 1; var < _idVec->size(); ++var)
	{
		if(_idVec->at(var) == 0)
		{
			_idVec->at(var) = 1;
			return var;
		}
	}
	if(_idVec->size() < _maxNum)
	{
		_idVec->push_back(1);
		return _idVec->size() - 1;
	}
	return -1;
}
/**
 * Removing the given id (hence, marking it as available).
 */
void IdHandler::removeId(int id)
{
	_idVec->at(id) = 0;
}

/**
 * Returns whether the given id has been handled or not.
 */
bool IdHandler::idExists(int id)
{
	return (_idVec->at(id) == 1);
}
