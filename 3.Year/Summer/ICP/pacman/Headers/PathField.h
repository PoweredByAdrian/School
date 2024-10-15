/**
* @brief PathField header file
*
* @author Adrian Horvath(xhorva14)
*
*/
#pragma once

#include <typeinfo>
#include <list>
#include "Field.h"
#include "PacmanObject.h"
#include "GhostObject.h"
#include "DoorObject.h"
#include "KeyObject.h"

class PathField : public Field {
private:
	int row;
	int col;
	char fieldType;
	Field* bottomField;
	Field* upperField;
	Field* rightField;
	Field* leftField;
	char typeOfObject;

public:
	/**
	* @brief Notifies the PathField that an object has moved from it.
	*
	* @param object The MazeObject that moved from the PathField.
	*/
	void objectMoved(MazeObject* object);
	/**
	* @brief Gets the list of MazeObjects present in the PathField.
	*
	* @return A pointer to the list of MazeObjects in the PathField.
	*/
	std::list<MazeObject*>* fieldObjectList;

	PathField(int row, int col);

	PathField(int row, int col, char typeOfObject);

	~PathField();
	/**
	* @brief Sets the surrounding fields for the PathField.
	*
	* @param bottom The bottom field of the PathField.
	* @param right The right field of the PathField.
	* @param upper The upper field of the PathField.
	* @param left The left field of the PathField.
	*/
	void setSurroundingFields(Field* bottom, Field* right, Field* upper, Field* left);

	bool canMove() override;

	/**
	*
	* @brief Checks if the PathField contains the specified MazeObject.
	* @param obj The MazeObject to check for in the PathField.
	* @return True if the PathField contains the MazeObject, false otherwise.
	*/
	bool contains(MazeObject* obj);

	MazeObject* get() override;
	/**
	* @brief Gets the list of MazeObjects present in the PathField.
	*
	* @return A pointer to the list of MazeObjects in the PathField.
	*/
	std::list<MazeObject*>* getObjectList();

	bool isEmpty() override;

	Field* nextField(Field::Direction dirs) override;
	/**
	* @brief Sets the MazeObject as the PacmanObject in the PathField.
	*
	* @param fieldObject The MazeObject to set as the PacmanObject.
	*/
	void setPacmanObject(MazeObject* fieldObject);
	/**
 * @brief Sets the MazeObject as the GhostObject in the PathField.
 *
 * @param fieldObject The MazeObject to set as the GhostObject.
 */
	void setGhostObject(MazeObject* fieldObject);

	/**
	*
	* @brief Sets the object fields of the PathField of the specified MazeObject.
	* @param object The MazeObject to set as the object fields of the PathField.
	*/
	void setObjectFields(MazeObject* object);

	char getType() override;
};
