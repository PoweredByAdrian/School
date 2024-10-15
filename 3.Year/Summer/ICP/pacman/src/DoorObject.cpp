/**
* @brief	DoorObject implementation
*
* @author Adrian Horvath(xhorva14)
*
*/
#include "DoorObject.h"

DoorObject::DoorObject(int row, int col, PathField* field)
{
	this->open = false;
	this->row = row;
	this->col = col;
	this->keys = 0;
	this->callerField = field;
}

DoorObject::~DoorObject()
{
}

void DoorObject::setKeys(int keys)
{
	this->keys = keys;
}

bool DoorObject::isOpen()
{
	return open;
}

void DoorObject::openDoors()
{
	if (observer->getKeys().size() == 0)
	{
		open = true;
		logger->printDoor(this->row, this->col);
		observer->notifyOpenDoors(this->row, this->col);
	}
}

Field* DoorObject::getField()
{
	return this->callerField;
}

void DoorObject::start()
{
	this->openDoors();
}

void DoorObject::setLogger(Logger* logger)
{
	this->logger = logger;
}

void DoorObject::attach(GameInterface* o)
{
	this->observer = o;
}

void DoorObject::detach(GameInterface* o)
{
	if (observer == o)
	{
		this->observer = nullptr;
	}
}
