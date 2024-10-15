/**
* @brief DoorObject header file.
*
* @authors	Matus Vrablik(xvrabl05)
*			Adrian Horvath(xhorva14)
*
*/
#pragma once

#include "MazeObject.h"
#include "GameInterface.h"
#include "Logger.h"
#include "PathField.h"
class PathField;

class DoorObject : public MazeObject {
private:
	bool open;
	Logger* logger;
	GameInterface* observer;
protected:
	int row;
	int col;
	int keys;
	PathField* callerField;
public:
	/**
	* @brief Constructor for the DoorObject class.
	*
	* Initializes a DoorObject instance with the specified row, column, and associated field.
	*
	* @param row The row position of the door.
	* @param col The column position of the door.
	* @param field The associated PathField for the door.
	*/
	DoorObject(int row, int col, PathField* field);
	/**
	* @brief Destructor for the DoorObject class.
	*
	* Cleans up any resources used by the DoorObject instance.
	*/
	~DoorObject();
	/**
	* @brief Set the number of keys for the door.
	*
	* This function sets the number of keys required to unlock the door.
	*
	* @param keys The number of keys to set for the door.
	*/
	void setKeys(int keys);
	/**
	* @brief Check if the door is open.
	*
	* This function returns a boolean value indicating whether the door is open or not.
	*
	* @return True if the door is open, false otherwise.
	*/
	bool isOpen();
	/**
	 * @brief Open the doors if all keys are picked up.
	*
	* This function checks if there are no more keys available to unlock the doors. If no keys are present, it sets the `open` flag to true, prints the door information using the logger, and notifies the observer about the doors being opened.
	*/
	void openDoors();

	bool canMove(Field::Direction dir) override { return false; }
	bool move(Field::Direction dir) override { return false; }
	/**
	* @brief Get the field associated with the door.
	*
	* This function returns a pointer to the Field object associated with the door.
	*
	* @return A pointer to the Field object.
	*/
	Field* getField() override;
	bool isPacman() override { return false; }
	int getLives() override { return 0; }
	/**
	* @brief Start the door object.
	*
	* This function starts the door by opening it using the `openDoors` function.
	*/
	void start() override;
	/**
	* @brief Set the logger for the door.
	*
	* This function sets the logger object to be used for logging door-related information.
	*
	* @param logger A pointer to the Logger object to set.
	*/
	void setLogger(Logger* logger) override;
	/**
	* @brief Attach an observer to the door.
	*
	* This function attaches a GameInterface object as an observer to the door.
	*
	* @param o A pointer to the GameInterface object to attach.
	*/
	void attach(GameInterface* o) override;
	/**
	* @brief Detach an observer from the door.
	*
	* This function detaches a GameInterface object as an observer from the door.
	*
	* @param o A pointer to the GameInterface object to detach.
	*/
	void detach(GameInterface* o) override;
};