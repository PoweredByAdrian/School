#include "PacmanObject.h"
/**
* @brief	PacmanObject implementation
*
* @author	Adrian Horvath(xhorva14)
*			Matus Vrablik(xvrabl05)
*
*/

#include "PacmanObject.h"

PacmanObject::PacmanObject(int row, int col, PathField* Field) {
	this->row = row;
	this->col = col;
	this->callerField = Field;
}

PacmanObject::~PacmanObject()
{
	for (KeyObject* key : this->keys)
	{
		delete key;
	}
}

void PacmanObject::setSurroundinFieldsPacman(Field* bottom, Field* right, Field* upper, Field* left, PathField* curPath) {
	this->bottomField = bottom;
	this->rightField = right;
	this->leftField = left;
	this->upperField = upper;
	this->callerField = curPath;
}

bool PacmanObject::canMove(Field::Direction dir) {
	if (this->bottomField != nullptr && dir == Field::D && this->bottomField->canMove())
	{
		return true;
	}
	else if (this->rightField != nullptr && dir == Field::R && this->rightField->canMove())
	{
		return true;
	}
	else if (this->upperField != nullptr && dir == Field::U && this->upperField->canMove())
	{
		return true;
	}
	else if (this->leftField != nullptr && dir == Field::L && this->leftField->canMove())
	{
		return true;
	}
	else
	{
		return false;
	}
}

Field* PacmanObject::getField() {
	return this->callerField;
}

bool PacmanObject::isPacman()
{
	return true;
}

int PacmanObject::getLives() {
	return this->lives;
}

void PacmanObject::start()
{
}

bool PacmanObject::move(Field::Direction dir) {
	PathField* nextField = nullptr;
	PathField* prevField = nullptr;
	int prevRow = this->row;
	int prevCol = this->col;
	int nextRow = this->row;
	int nextCol = this->col;

	switch (dir)
	{
	case Field::L:
		if (this->leftField == nullptr || !(this->leftField->canMove())) { return false; }
		nextField = static_cast<PathField*>(this->leftField);
		this->col -= 1;
		nextCol = this->col;
		break;
	case Field::U:
		if (this->upperField == nullptr || !(this->upperField->canMove())) { return false; }
		nextField = static_cast<PathField*>(this->upperField);
		this->row -= 1;
		nextRow = this->row;
		break;
	case Field::R:
		if (this->rightField == nullptr || !(this->rightField->canMove())) { return false; }
		nextField = static_cast<PathField*>(this->rightField);
		this->col += 1;
		nextCol = this->col;
		break;
	case Field::D:
		if (this->bottomField == nullptr || !(this->bottomField->canMove())) { return false; }
		nextField = static_cast<PathField*>(this->bottomField);
		this->row += 1;
		nextRow = this->row;
		break;
	default:
		return false;
	}

	prevField = this->callerField;
	bool lives = false;
	prevField->objectMoved(this);
	nextField->setPacmanObject(this);
	observer->notifyMove(prevRow, prevCol, nextRow, nextCol);
	logger->printMovement(this, prevRow, prevCol, nextRow, nextCol);
	if (!(nextField->isEmpty()))
	{
		MazeObject* object = nextField->get();
		if (!nextField->fieldObjectList->empty())
		{
			MazeObject* keyRemove = nullptr;
			for (MazeObject* obj : *nextField->fieldObjectList)
			{
				GhostObject* ghost = (GhostObject*)obj;
				DoorObject* door = (DoorObject*)obj;
				KeyObject* key = (KeyObject*)obj;
				if (typeid(*ghost) == typeid(GhostObject))
				{
					lives = true;
				}
				else if (typeid(*door) == typeid(DoorObject)) {
					if (door->isOpen())
					{
						this->observer->notifyEndLevel();
						return false;
					}
				}
				else if (typeid(*key) == typeid(KeyObject))
				{
					this->observer->notifyPickKey(nextRow, nextCol);
					this->logger->printKey(nextRow, nextCol);
					keyRemove = obj;
					this->keys.emplace_back(key);
				}
			}
			nextField->fieldObjectList->remove(keyRemove);
		}
	}
	if (lives)
	{
		if (!this->decreaseLives()) { return false; }
	}

	return true;
}

bool PacmanObject::decreaseLives() {
	this->lives -= 1;
	if (this->lives == 0) {
		observer->notifyGameOver();
		return false;
	}
	observer->notifyLives();
	logger->printLives(lives);
	return true;
}

void PacmanObject::setLogger(Logger* logger)
{
	this->logger = logger;
}

void PacmanObject::attach(GameInterface* o) {
	this->observer = o;
}

void PacmanObject::detach(GameInterface* o) {
	if (observer == o)
	{
		this->observer = nullptr;
	}
};
