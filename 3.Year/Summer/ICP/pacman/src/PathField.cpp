/**
* @brief	PathField implementation
*
* @authors	Adrian Horvath(xhorva14)
*			Matus Vrablik(xvrabl05)
*
*/

#include "PathField.h"

PathField::PathField(int row, int col) {
	this->col = col;
	this->row = row;
	this->fieldType = '.';
	this->typeOfObject = 0;
	this->fieldObjectList = new list<MazeObject*>();
}

PathField::PathField(int row, int col, char typeOfObject) {
	this->col = col;
	this->row = row;
	this->fieldType = '.';
	this->typeOfObject = typeOfObject;
	this->fieldObjectList = new list<MazeObject*>();
	switch (typeOfObject)
	{
	case 'S':
		this->fieldObjectList->push_back(new PacmanObject(row, col, this));
		this->typeOfObject = 'S';
		break;
	case 'G':
		this->fieldObjectList->push_back(new GhostObject(row, col, this));
		this->typeOfObject = 'G';
		break;
	case 'T':
		this->fieldObjectList->push_front(new DoorObject(row, col, this));
		this->typeOfObject = 'T';
		break;
	case 'K':
		this->fieldObjectList->push_front(new KeyObject(row, col, this));
		this->typeOfObject = 'K';
		break;
	default:
		break;
	}

}

PathField::~PathField()
{
	for (MazeObject* obj : *fieldObjectList)
	{
		delete obj;
	}
	delete fieldObjectList;
}

void PathField::setSurroundingFields(Field* bottom, Field* right, Field* upper, Field* left) {
	this->bottomField = bottom;
	this->rightField = right;
	this->upperField = upper;
	this->leftField = left;
}

void PathField::objectMoved(MazeObject* object)
{
	if (!this->fieldObjectList->empty())
	{
		for (MazeObject* obj : *this->fieldObjectList)
		{
			if (typeid(*obj) == typeid(*object))
			{
				this->fieldObjectList->remove(obj);
				break;
			}
		}
	}
}

bool PathField::canMove() {
	return !(this->fieldType == 'X');
}

bool PathField::contains(MazeObject* obj) {
	list<MazeObject*>::iterator lastElem = fieldObjectList->end();
	if (obj->equals(*lastElem))
	{
		return true;
	}
	else
	{
		return false;
	}
}

MazeObject* PathField::get() { return !fieldObjectList->empty() ? fieldObjectList->back() : nullptr; }

list<MazeObject*>* PathField::getObjectList()
{
	return fieldObjectList;
}
bool PathField::isEmpty() {
	if (this->fieldObjectList->empty())
	{
		return true;
	}
	return false;
}

Field* PathField::nextField(Field::Direction dirs) {
	if (dirs == Field::Direction::D) {
		return this->bottomField;
	}
	else if (dirs == Field::Direction::R) {
		return this->rightField;
	}
	else if (dirs == Field::Direction::U)
		return this->upperField;
	else if (dirs == Field::Direction::L) {
		return this->leftField;
	}
	else {
		return nullptr;
	}
}

void PathField::setPacmanObject(MazeObject* fieldObject) {
	this->fieldObjectList->emplace_back(fieldObject);
	this->typeOfObject = 'S';
	this->setObjectFields(fieldObject);
}

void PathField::setGhostObject(MazeObject* fieldObject) {
	this->fieldObjectList->emplace_back(fieldObject);
	this->typeOfObject = 'G';
	this->setObjectFields(fieldObject);
}

void PathField::setObjectFields(MazeObject* object) {

	PacmanObject* pacman = (PacmanObject*)object;
	GhostObject* ghost = (GhostObject*)object;
	if (object == nullptr) { return; };
	if (typeid(*pacman) == typeid(PacmanObject)) {
		pacman->setSurroundinFieldsPacman(this->bottomField, this->rightField, this->upperField, this->leftField, this);
	}
	else if (typeid(*ghost) == typeid(GhostObject)) {
		ghost->setSurroundinFieldsGhost(this->bottomField, this->rightField, this->upperField, this->leftField, this);
	}
}

char PathField::getType()
{
	return this->fieldType;
}
