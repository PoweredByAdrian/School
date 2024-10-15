/**
* @brief The base abstract class for a field in the maze.
*
* @author Adrian Horvath(xhorva14)
*
*/
#pragma once
class MazeObject;

class Field {
public:
	virtual ~Field() = default;
	/**
	 * @brief Enumeration representing directions in the maze.
	 */
	enum Direction {
		L,	/**< Left direction */
		U,	/**< Up direction */
		R,	/**< Right direction */
		D	/**< Down direction */
	};
	/**
	 * @brief Check if the field allows movement.
	 *
	 * @return `true` if movement is allowed, `false` otherwise.
	 */
	virtual bool canMove() = 0;
	/**
	 * @brief Get the MazeObject on the field.
	 *
	 * @return A pointer to the MazeObject on the field.
	 */
	virtual MazeObject* get() = 0;
	/**
	 * @brief Check if the field is empty.
	 *
	 * @return `true` if the field is empty, `false` otherwise.
	 */
	virtual bool isEmpty() = 0;
	/**
	 * @brief Get the next field in the specified direction.
	 *
	 * @param var1 The direction to move to.
	 * @return A pointer to the next field in the specified direction.
	 */
	virtual Field* nextField(Direction var1) = 0;
	/**
	 * @brief Check if the field contains the specified MazeObject.
	 *
	 * @param var1 A pointer to the MazeObject to check.
	 * @return `true` if the field contains the specified MazeObject, `false` otherwise.
	 */
	virtual bool contains(MazeObject* var1) = 0;
	/**
	 * @brief Get the type of the field.
	 *
	 * @return The character representing the type of the field.
	 */
	virtual char getType() = 0;
};