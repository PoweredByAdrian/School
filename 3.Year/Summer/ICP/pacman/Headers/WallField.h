/**
* @brief WallField header file
*
* @author Adrian Horvath(xhorva14)
*
*/
#pragma once

#include <typeinfo>
#include "Field.h"

class WallField : public Field
{
private:
	char fieldType;
	int row;
	int col;

public:
	WallField(int row, int col);
	~WallField();

	bool canMove() override;

	/**
	*
	* @brief Checks if the field contains the specified MazeObject.
	* @param obj The MazeObject to check for.
	* @return bool True if the field contains the MazeObject, false otherwise.
	*/
	bool contains(MazeObject* obj);

	MazeObject* get() override;

	bool isEmpty() override;
	/**
	*
	* @brief Returns the next field in the specified direction.
	* @param dir The direction to move in.
	* @return Field* The next field in the specified direction.
	*/
	Field* nextField(Field::Direction dir);

	char getType() override;
};
