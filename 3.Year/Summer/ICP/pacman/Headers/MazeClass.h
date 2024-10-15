/**
* @brief
*
* @authors	Adrian Horvath(xhorva14)
*			Matus Vrablik(xvrabl05)
*
*/
#pragma once

#include <list>
#include <string>
#include "PathField.h"
#include "WallField.h"
#include "PacmanObject.h"
#include "GhostObject.h"
#include "DoorObject.h"
#include "Maze.h"
class DoorObject;

class MazeClass : public Maze {
public:
	/**
	 * @brief Constructs a MazeClass object with the specified number of rows and columns.
	 *
	 * @param rows The number of rows in the maze.
	 * @param cols The number of columns in the maze.
	 */
	MazeClass(int rows, int cols);
	/**
	 * @brief Destroys the MazeClass object.
	 */
	~MazeClass();
	/**
	 * @brief Creates an array to represent the maze grid with the specified number of rows and columns.
	 *
	 * @param rows The number of rows in the maze.
	 * @param cols The number of columns in the maze.
	 */
	void createArray(int rows, int cols);
	int numCols() override;
	std::list<MazeObject*> ghosts() override;
	int numRows() override;
	Field* getField(int row, int col) override;
	/**
	* @brief Inserts a line of characters into the maze grid.
	*
	* @param line The line of characters to insert.
	*/
	void insertLine(std::string line);
	/**
	* @brief Sets the fields of the maze grid based on the inserted lines.
	*/
	void setFields();
	/**
	* @brief Gets the door object in the maze.
	*
	* @return A pointer to the door object.
	*/
	DoorObject* getDoor();
	/**
	* @brief Gets the number of keys in the maze.
	*
	* @return The number of keys.
	*/
	int getKeys();
private:
	int numberOfLines = 1;
	std::list<MazeObject*> listOfGhosts;
	Field*** fieldArray;

	int rows;
	int cols;
	int keys;
	DoorObject* door;
	/**
	* @brief Inserts a ghost into the specified field in the maze.
	*
	* @param field The field to insert the ghost into.
	*/
	void insertGhost(Field* field);
};
