package Game.MazeBoard;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import Game.Logger;
import Interfaces.Maze;

import java.io.IOException;

/**
 * Class for set up and create maze
 */
public class MazeConfigure {
    public Logger logger = new Logger();
    private MazeClass newMaze = new MazeClass(this.logger);
    private boolean correctLine = true;
    private boolean doneReading = false;
    private int countLines = 0;
    private int rows;
    private int cols;
    public MazeConfigure() throws IOException {
    }
    /**
     * Method to set up array and ready for Reading Maze file.
     * @param rows number of rows in maze
     * @param cols number of cols in maze
     */
    public void startReading(int rows, int cols){
        this.rows = rows;
        this.cols = cols;
        logger.printFirstLine(rows + 2, cols + 2);
        newMaze.createArray(rows + 2, cols + 2);
        String wall = "X";
        logger.printMazeLine(wall.repeat(cols + 2));
    }
    /**
     * Method that process one line from given file.
     * @param line Line from input
     */
    public boolean processLine(String line){
        logger.printMazeLine("X" + line + "X");
        this.countLines++;
        if (this.correctLine){
            if (newMaze.numCols() != line.length() + 2){
                return this.correctLine = false;
            }
            newMaze.insertLine(line);
        }
        return this.correctLine;
    }
    /**
     * Method for finishing reading from file.
     * @return True if reading was successful
     */
    public boolean stopReading() {
        String wall = "X";
        logger.printMazeLine(wall.repeat(cols + 2));
        logger.printTurnZero();
        if (this.countLines != newMaze.numRows() - 2) {
            return false;
        }
        newMaze.setFields();
        newMaze.setEntitiesFields();
        return this.doneReading = true;
    }
    /**
     * Method to create Maze
     * @return created maze
     */
    public Maze createMaze() {
        if (this.doneReading && this.correctLine) {
            return this.newMaze;
        }
        else {
            return null;
        }
    }
}
