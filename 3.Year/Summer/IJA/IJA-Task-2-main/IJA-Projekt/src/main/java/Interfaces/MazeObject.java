package Interfaces;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

public interface MazeObject {
    /**
     * Method that pick up key
     */
    default void pickUpKey() {};
    /**
     * Method that return if key was taken
     * @return  true if was taken
     */
    default boolean keyTaken() {return false;}
    /**
     * Method that open door
     */
    default void openDoor() {};
    /**
     * Method that return if door are open
     * @return  true if is open
     */
    default boolean isOpen() { return false; }
}
