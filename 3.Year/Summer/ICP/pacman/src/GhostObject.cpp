/**
* @brief	GhostObject implementation
*
* @author Adrian Horvath(xhorva14)
*
*/

#include "GhostObject.h"


GhostObject::GhostObject(int row, int col, PathField* Field) {
	this->row = row;
	this->col = col;
	this->callerField = Field;
	this->bottomField = nullptr;
	this->upperField = nullptr;
	this->rightField = nullptr;
	this->leftField = nullptr;
	this->logger = nullptr;
	this->observer = nullptr;
}

GhostObject::~GhostObject()
{
}

void GhostObject::setSurroundinFieldsGhost(Field* bottom, Field* right, Field* upper, Field* left, PathField* curPath) {
	this->bottomField = bottom;
	this->rightField = right;
	this->leftField = left;
	this->upperField = upper;
	this->callerField = curPath;
}

bool GhostObject::canMove(Field::Direction dir) {

	GhostObject* ghost = nullptr;
	if (this->bottomField != nullptr && dir == Field::D && this->bottomField->canMove())
	{
		ghost = (GhostObject*)this->bottomField->get();
	}
	else if (this->rightField != nullptr && dir == Field::R && this->rightField->canMove())
	{
		ghost = (GhostObject*)this->rightField->get();
	}
	else if (this->upperField != nullptr && dir == Field::U && this->upperField->canMove())
	{
		ghost = (GhostObject*)this->upperField->get();
	}
	else if (this->leftField != nullptr && dir == Field::L && this->leftField->canMove())
	{
		ghost = (GhostObject*)this->leftField->get();
	}

	if (ghost != nullptr && typeid(*ghost) == typeid(GhostObject)) {
		return false;
	}
	return true;
}

Field* GhostObject::getField() {
	return this->callerField;
}

bool GhostObject::isPacman()
{
	return false;
}

int GhostObject::getLives() {
	return 0;
}

bool GhostObject::move(Field::Direction dir) {
	PathField* nextField = nullptr;
	PathField* prevField = nullptr;
	int prevRow = this->row;
	int prevCol = this->col;
	int nextRow = this->row;
	int nextCol = this->col;

	switch (dir)
	{
	case Field::L:
		if (this->leftField == NULL || !this->leftField->canMove()) { return false; }
		nextField = static_cast<PathField*>(this->leftField);
		this->col -= 1;
		nextCol = this->col;
		break;
	case Field::U:
		if (this->upperField == NULL || !this->upperField->canMove()) { return false; }
		nextField = static_cast<PathField*>(this->upperField);
		this->row -= 1;
		nextRow = this->row;
		break;
	case Field::R:
		if (this->rightField == NULL || !this->rightField->canMove()) { return false; }
		nextField = static_cast<PathField*>(this->rightField);
		this->col += 1;
		nextCol = this->col;
		break;
	case Field::D:
		if (this->bottomField == nullptr || !this->bottomField->canMove()) { return false; }
		nextField = static_cast<PathField*>(this->bottomField);
		this->row += 1;
		nextRow = this->row;
		break;
	default:
		return false;
	}
	prevField = this->callerField;
	if (!(nextField->isEmpty()))
	{
		bool player = false;
		bool ghost = false;
		if (!nextField->fieldObjectList->empty())
		{
			for (MazeObject* obj : *nextField->fieldObjectList)
			{
				if (typeid(*obj) == typeid(*this))
				{
					ghost = true;
					break;
				}
				if (typeid(*obj) == typeid(PacmanObject))
				{
					player = true;
					break;
				}
			}
		}
		if (ghost) { return false; }
		MazeObject* object = nextField->get();
		if (player)
		{
			PacmanObject* pacman = static_cast<PacmanObject*>(object);
			if (!pacman->decreaseLives()) { return false; }
		}
	}
	//moved from 109
	prevField->objectMoved(this);
	nextField->setGhostObject(this);
	observer->notifyMove(prevRow, prevCol, nextRow, nextCol);
	logger->printMovement(this, prevRow, prevCol, nextRow, nextCol);
	return true;
}

void GhostObject::start()
{
	list<Field::Direction> moveTo;
	list<Field::Direction> dirs = { Field::Direction::D, Field::Direction::R, Field::Direction::U, Field::Direction::L };
	list<Field::Direction>::iterator it;
	moveTo.clear();
	for (Field::Direction d : dirs)
	{
		if (this->canMove(d)) {
			moveTo.push_back(d);
		}
	}

	int randomPos = std::rand() % (moveTo.size() + 1);
	if (randomPos == moveTo.size())
	{
		return;
	}
	int oldRow = this->row;
	int oldCol = this->col;
	it = std::next(moveTo.begin(), randomPos);
	if (!this->move(*it)) { return; }
	int newRow = this->row;
	int newCol = this->col;
	this->observer->notifyMove(oldRow, oldCol, newRow, newCol);
}

void GhostObject::setLogger(Logger* logger)
{
	this->logger = logger;
}

void GhostObject::attach(GameInterface* o) {
	this->observer = o;
}

void GhostObject::detach(GameInterface* o) {
	if (observer == o)
	{
		this->observer = nullptr;
	}
}
