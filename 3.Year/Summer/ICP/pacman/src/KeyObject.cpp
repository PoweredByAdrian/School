/**
 * @brief KeyObject class file
 *
 * @author Matus Vrablik (xvrabl05)
 *
 */

#include "KeyObject.h"

KeyObject::KeyObject(int row, int col, PathField* field)
{
	this->row = row;
	this->col = col;
	this->callerField = field;
	this->logger = nullptr;
	this->observer = nullptr;
}

KeyObject::~KeyObject()
{
}

Field* KeyObject::getField()
{
	return this->callerField;
}

void KeyObject::setLogger(Logger* logger)
{
	this->logger = logger;
}

void KeyObject::attach(GameInterface* o)
{
	this->observer = o;
}

void KeyObject::detach(GameInterface* o)
{
	if (observer == o)
	{
		this->observer = nullptr;
	}
}
