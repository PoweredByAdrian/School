/**
* @brief KeyObject header file
*
* @author Matus Vrablik(xvrabl05)
*
*/
#pragma once

#include "MazeObject.h"
#include "GameInterface.h"
#include "Logger.h"
#include "PathField.h"
class PathField;

class KeyObject : public MazeObject {
private:
	Logger* logger;
	GameInterface* observer;
protected:
	int row;
	int col;
	PathField* callerField;
public:
	/**
	* @brief Constructs a KeyObject with the specified row, column, and field.
	*
	* @param row The row coordinate of the KeyObject in the maze.
	* @param col The column coordinate of the KeyObject in the maze.
	* @param field A pointer to the PathField that the KeyObject belongs to.
	*/
	KeyObject(int row, int col, PathField* field);
	~KeyObject();

	bool canMove(Field::Direction dir) override { return false; }
	bool move(Field::Direction dir) override { return false; }
	Field* getField() override;
	bool isPacman() override { return false; }
	int getLives() override { return 0; }
	void start() override {};

	void setLogger(Logger* logger) override;
	void attach(GameInterface* o) override;
	void detach(GameInterface* o) override;
};
