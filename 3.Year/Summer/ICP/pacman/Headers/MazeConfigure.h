/**
* @brief MazeConfigure header file
*
* @author Adrian Horvath(xhorva14)
*
*/
#pragma once

#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "MazeClass.h"
#include "Logger.h"
class MazeClass;
class Maze;

class MazeConfigure {
public:
	/**
	* @brief Constructs a MazeConfigure object with the specified logger.
	*
	* @param logger A pointer to the logger object to use for logging.
	*/
	MazeConfigure(Logger* logger);
	/**
	 * @brief Destructor for the MazeConfigure object.
	 */
	~MazeConfigure();
	/**
	 * @brief Loads the map from the specified file.
	 *
	 * @param file The path to the file containing the map.
	 */
	void loadMapFromFile(std::string file);
	/**
	* @brief Starts reading the map with the specified number of rows and columns.
	*
	* @param rows The number of rows in the map.
	* @param cols The number of columns in the map.
	*/
	void startReading(int rows, int cols);
	/**
	 * @brief Processes a line of the map.
	 *
	 * @param line The line of the map to process.
	 * @return True if the line was processed successfully, false otherwise.
	 */
	bool processLine(std::string line);
	/**
	 * @brief Stops reading the map.
	 *
	 * @return True if reading was stopped successfully, false otherwise.
	 */
	bool stopReading();
	/**
	 * @brief Creates a maze based on the loaded map configuration.
	 *
	 * @return A pointer to the created maze object.
	 */
	Maze* createMaze();
protected:
	MazeClass* newMaze;
	bool correctLine = true;
	bool doneReading = false;
private:
	Logger* logger;
	int counLines = 0;
};
