/**
* @brief MazeObject header file
*
* @author Adrian Horvath(xhorva14)
*
*/
#pragma once
#include <stdbool.h>
#include "Field.h"
class Field;
class GameInterface;
class Logger;

class MazeObject {
public:
	/**
	* @brief Destructor for the MazeObject class.
	*
	* The destructor is declared as virtual to ensure that derived classes can properly clean up their resources when an object is destroyed.
	*/
	virtual ~MazeObject() = default;
	/**
	* @brief Checks if the MazeObject can move in the specified direction.
	*
	* @param var1 The direction in which the MazeObject intends to move.
	* @return True if the MazeObject can move in the specified direction, false otherwise.
	*/
	virtual bool canMove(Field::Direction var1) = 0;
	/**
	* @brief Moves the MazeObject in the specified direction.
	*
	* @param var1 The direction in which the MazeObject should move.
	* @return True if the move was successful, false otherwise.
	*/
	virtual bool move(Field::Direction var1) = 0;
	/**
	* @brief Gets the Field object on which the MazeObject is currently located.
	*
	* @return A pointer to the Field object on which the MazeObject is located.
	*/
	virtual Field* getField() = 0;
	/**
	* @brief Checks if the MazeObject is a Pacman object.
	*
	* @return True if the MazeObject is a Pacman object, false otherwise.
	*/
	virtual bool isPacman() = 0;
	/**
	* @brief Gets the number of lives remaining for the Pacman object.
	*
	* @return The number of lives remaining for the Pacman object.
	*/
	virtual int getLives() = 0;
	/**
	* @brief Checks if this MazeObject is equal to the specified MazeObject.
	*
	* The equality check is performed based on the memory addresses of the objects.
	*
	* @param obj A pointer to the MazeObject to compare.
	* @return True if the objects are equal (i.e., have the same memory address), false otherwise.
	*/
	bool equals(MazeObject* obj);

	virtual void start() = 0;
	/**
	* @brief Sets the logger for the MazeObject.
	*
	* @param logger A pointer to the Logger object to set.
	*/
	virtual void setLogger(Logger* logger) = 0;
	/**
	* @brief Attaches a GameInterface object as an observer to the MazeObject.
	*
	* @param o A pointer to the GameInterface object to attach as an observer.
	*/
	virtual void attach(GameInterface* o) = 0;
	/**
	* @brief Detaches a GameInterface object from the MazeObject's observers.
	*
	* @param o A pointer to the GameInterface object to detach from the observers.
	*/
	virtual void detach(GameInterface* o) = 0;
};
