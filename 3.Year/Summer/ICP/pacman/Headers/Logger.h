/**
* @brief Logger header file
*
* @author Adrian Horvath(xhorva14)
*
*/
#pragma once
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "MazeObject.h"
#include "PacmanObject.h"
#include "GhostObject.h"
#include "Action.h"

class Logger {
private:
	std::string m_filename;
	std::ofstream m_logFile;
	int turn = 0;
protected:
public:
	/**
	 * @brief Constructs a Logger object.
	 */
	Logger();
	/**
	 * @brief Destroys the Logger object.
	 */
	~Logger();
	/**
	 * @brief Prints the initial message for turn zero.
	 */
	void printTurnZero();
	/**
	 * @brief Prints the first line with size of the maze.
	 *
	 * @param rows The number of rows in the maze.
	 * @param cols The number of columns in the maze.
	 */
	void printFirstLine(int rows, int cols);
	/**
	 * @brief Prints a line of the maze.
	 *
	 * @param line The string representing a line of the maze.
	 */
	void printMazeLine(std::string line);
	/**
	 * @brief Prints the movement of a MazeObject from one position to another.
	 *
	 * @param obj A pointer to the MazeObject that moved.
	 * @param fromX The starting X coordinate of the movement.
	 * @param fromY The starting Y coordinate of the movement.
	 * @param toX The destination X coordinate of the movement.
	 * @param toY The destination Y coordinate of the movement.
	 */
	void printMovement(MazeObject* obj, int fromX, int fromY, int toX, int toY);
	/**
	 * @brief Prints the coordinates of a door that is opened.
	 *
	 * @param x The X coordinate of the door.
	 * @param y The Y coordinate of the door.
	 */
	void printDoor(int x, int y);
	/**
	 * @brief Prints the coordinates of a key that is picked up.
	 *
	 * @param x The X coordinate of the key.
	 * @param y The Y coordinate of the key.
	 */
	void printKey(int x, int y);
	/**
	 * @brief Prints the number of lives remaining.
	 *
	 * @param lives The number of lives remaining.
	 */
	void printLives(int lives);
	/**
	 * @brief Erase log in replay mode
	 * 
	 */
	void erase();
};

