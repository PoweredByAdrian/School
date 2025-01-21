package Interfaces;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import java.awt.*;
import java.util.Random;
public interface Field {
    /**
     * Method that return Field's coordinates
     */
    Point getFieldPosition();

    /**
     * Method that returns if there is pacman on field.
     * @return True if field has Pacman
     */
    default boolean hasPacman() {return false;}
    /**
     * Method that returns if there is object on field
     * @return True if there is object on field
     */
    default boolean hasObject() {return false;}
    /**
     * Method that return next field in direction
     * @param dir direction
     * @return Field
     */
    Field nextField(Direction dir);
    /**
     * Method that check if there is ghost on field
     * @return True if is ghost in field
     */
    default boolean hasGhost() {return false;}
    /**
     * Method that return Ghost entity that is on field
     * @return Ghost entity
     */
    default MazeEntity getGhostEntity() { return null; }
    /**
     * Method that return Pacman entity that is on field
     * @return Pacman entity
     */
    default MazeEntity getPacmanEntity() { return null; }
    /**
     * Method that return Object that is on field
     * @return Maze Object
     */
    default MazeObject getObject() { return null; }
    /**
     * Method that return Ghost entity that is on field
     * @return Ghost entity
     */
    boolean canMove();
    /**
     * Method that check if entity can move on this field
     * @return True if yes
     */
    default boolean containPoint(){ return false; }
    /**
     * Method that remove point from field when taken
     */
    void pointTaken();
    /**
     * Method that bottom field
     * @param bottom Field for set up
     */
    void setBottomField(Field bottom);
    /**
     * Method that right field
     * @param right Field for set up
     */
    void setRightField(Field right);
    /**
     * Method that upper field
     * @param upper Field for set up
     */
    void setUpperField(Field upper);
    /**
     * Method that left field
     * @param left Field for set up
     */
    void setLeftField(Field left);
    /**
     * Method that move pacman into new coordinates
     * @param x x coordinates
     * @param y y coordinates
     * @param lives Pacman's lives
     */
    void movePacman(int x, int y, int lives);
    /**
     * Method that move Ghost into new coordinates
     * @param x x coordinates
     * @param y y coordinates
     * @param cameFromX X coordinates of origin
     * @param cameFromY y coordinates of origin
     */
    void moveGhost(int x, int y, int cameFromX, int cameFromY);
    /**
     * Method that remove ghost from path
     */
    default void removeGhost() {}
    default void removePacman() {}
    /**
     * Method that return if door are open
     * @return  true if is open
     */
    default boolean doorIsOpen() { return false; }
    enum Direction {
        L(0, -1),
        U(-1, 0),
        R(0, 1),
        D(1, 0);

        private final int r;
        private final int c;

        private Direction(int dr, int dc) {
            this.r = dr;
            this.c = dc;
        }

        public int deltaRow() {
            return this.r;
        }

        public int deltaCol() {
            return this.c;
        }

        private static final Random PRNG = new Random();

        public static Field.Direction randomDirection()  {
            Direction[] directions = values();
            return directions[PRNG.nextInt(directions.length)];
        }
    }
}
