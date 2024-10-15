/**
* @brief Ghost object header file
*
* @author Adrian Horvath(xhorva14)
*
*/
#pragma once

#include "MazeObject.h"
#include "PacmanObject.h"
#include "PathField.h"
#include "Logger.h"
#include <chrono>
#include <thread>
#include <iostream>
class PathField;
class GameInterface;
class Logger;

class GhostObject : public MazeObject {
private:
	Logger* logger;
	GameInterface* observer;
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
	* @brief Constructs a GhostObject with the specified row, column, and field.
	*
	* @param row The row coordinate of the GhostObject in the maze.
	* @param col The column coordinate of the GhostObject in the maze.
	* @param field A pointer to the PathField that the GhostObject belongs to.
	*/
	GhostObject(int row, int col, PathField* field);
	~GhostObject();
	/**
	* @brief Sets the surrounding fields for a Ghost MazeObject.
	*
	* This function sets the neighboring fields of a Ghost MazeObject. The neighboring fields include the bottom, right,
	* upper, and left fields, along with the current path field.
	*
	* @param bottom A pointer to the Field representing the bottom neighbor.
	* @param right A pointer to the Field representing the right neighbor.
	* @param upper A pointer to the Field representing the upper neighbor.
	* @param left A pointer to the Field representing the left neighbor.
	* @param curPath A pointer to the PathField representing the current path field.
	*/
	void setSurroundinFieldsGhost(Field* bottom, Field* right, Field* upper, Field* left, PathField* curPath);

	bool canMove(Field::Direction var1) override;
	Field* getField() override;
	bool isPacman() override;
	int getLives() override;
	bool move(Field::Direction dir) override;
	void start() override;

	void setLogger(Logger* logger) override;
	void attach(GameInterface* o) override;
	void detach(GameInterface* o) override;
};
