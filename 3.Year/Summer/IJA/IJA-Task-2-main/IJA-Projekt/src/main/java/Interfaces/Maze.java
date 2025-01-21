package Interfaces;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import java.util.List;

public interface Maze {
    Field getField(int var1, int var2);

    int numRows();

    int numCols();
    /**
     * Method that sets Fields of maze
     */
    void setFields();
    /**
     * Method that set Fields for entity
     */
    void setEntitiesFields();
}
