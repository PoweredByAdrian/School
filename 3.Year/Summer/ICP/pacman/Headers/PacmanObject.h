/**
* @brief PacmanObject header file
*
* @author Adrian Horvath(xhorva14)
*
*/
#pragma once

#include "MazeObject.h"
#include "PathField.h"
#include "Field.h"
#include "GameInterface.h"
#include "Logger.h"
#include "KeyObject.h"
class PathField;
class GameInterface;
class Logger;
class KeyObject;

class PacmanObject : public MazeObject {
private:
	GameInterface* observer;
	Logger* logger;
	int lives = 3;
	std::list<KeyObject*> keys;
protected:
	int row;
	int col;
	PathField* callerField;
	Field* bottomField;
	Field* rightField;
	Field* upperField;
	Field* leftField;

public:
	/**
	* @brief Constructs a PacmanObject with the specified row, column, and field.
	*
	* @param row The row position of the PacmanObject.
	* @param col The column position of the PacmanObject.
	* @param field The PathField that the PacmanObject belongs to.
	*/
	PacmanObject(int row, int col, PathField* field);
	~PacmanObject();

	/**
	* @brief Sets the surrounding fields for PacmanObject.
	*
	* @param bottom The bottom field of the PacmanObject.
	* @param right The right field of the PacmanObject.
	* @param upper The upper field of the PacmanObject.
	* @param left The left field of the PacmanObject.
	* @param curPath The current PathField of the PacmanObject.
	*/
	void setSurroundinFieldsPacman(Field* bottom, Field* right, Field* upper, Field* left, PathField* curPath);

	bool canMove(Field::Direction var1) override;
	Field* getField() override;
	bool isPacman() override;
	int getLives() override;
	void start() override;
	bool move(Field::Direction dir) override;

	/**
	* @brief Decreases the number of lives.
	*
	* @return True if the lives were successfully decreased, false if there are no more lives remaining.
	*/
	bool decreaseLives();

	void setLogger(Logger* logger) override;
	void attach(GameInterface* o) override;
	void detach(GameInterface* o) override;
};
