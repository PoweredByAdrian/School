/**
* @brief MazeObject template
*
* @author Adrian Horvath(xhorva14)
*
*/
#pragma once
#include <list>

#include "MazeObject.h"

class Maze {
public:
	/**
	 * @brief Get the field at the specified coordinates.
	 *
	 * @param var1 The row coordinate of the field.
	 * @param var2 The column coordinate of the field.
	 * @return A pointer to the Field at the specified coordinates.
	 */
	virtual Field* getField(int var1, int var2) = 0;
	/**
	 * @brief Get the number of rows in the maze.
	 *
	 * @return The number of rows in the maze.
	 */
	virtual int numRows() = 0;
	/**
	* @brief Get the number of columns in the maze.
	*
	* @return The number of columns in the maze.
	*/
	virtual int numCols() = 0;
	/**
	 * @brief Get a list of MazeObject pointers representing the ghosts in the maze.
	 *
	 * @return A list of MazeObject pointers representing the ghosts in the maze.
	 */
	virtual std::list<MazeObject*> ghosts() = 0;
};