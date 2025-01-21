package Interfaces;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import java.awt.*;
import java.util.List;

public interface MazeEntity {
    /**
     * Method that check if entity can move to direction
     * @param dir direction
     * @return  true if can move
     */
    boolean canMove(Field.Direction dir);
    /**
     * Method that move entity
     * @param dir direction
     * @param cameFrom coordinates from where entity came from
     * @param x x coordinates
     * @param y y coordinates
     * @return  true is sucess
     */
    boolean move(Field.Direction dir, int x, int y, Point cameFrom);
    /**
     * Method that return number of Pacman's lives
     * @return  number of lives
     */
    default int getLives() {return 0; }
    /**
     * Method that return field entity standing on
     * @return  current field
     */
    Field getField();
    /**
     * Method that return if entity is pacman
     * @return  true if is pacman
     */
    default boolean isPacman(){ return false;}
    /**
     * Method that return coordinates of entity
     * @return  coordinates
     */
    Point getPosition();
    /**
     * Method that sets fields for entity
     * @param field current field
     */
    void setFields(Field field);
    void setGhost(Field field);
    /**
     * Method that move entity
     * @param cameFrom coordinates where entity came from
     */
    default Point Movement(Point cameFrom) { return null; }
    /**
     * Method that decrease lives to pacman
     * @return  true if is dead
     */
    default boolean decreaseLives() { return false; }
    /**
     * Method that find the path for entity using Breath-First algorithm
     * @param startPosition start position
     * @param endPosition end position
     * @return  list of pathfields
     */

    default List findPath(Field startPosition, Field endPosition) { return null; }
    default void setLives(int lives) {}
}
