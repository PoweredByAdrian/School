package Game.MazeBoard;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import Game.Entities.Pacman;
import Game.Fields.WallField;
import Game.Fields.PathField;
import Game.Logger;
import Interfaces.Maze;
import Interfaces.Field;
import Interfaces.MazeEntity;

import java.util.LinkedList;
import java.util.List;
/**
 * Class represents entire maze.
 */
public class MazeClass implements Maze {
    private Logger logger;
    private Field[][] fieldArray;
    private int numberOfLines = 1;

    public MazeClass(Logger logger){
        this.logger = logger;
    }
    public void createArray(int rows, int cols){
        this.fieldArray = new Field[rows][cols];
        this.setOuterWalls(rows, cols);
    }
    /**
     * Method to create outer walls of maze.
     * @param rows number of rows in maze
     * @param cols number of cols in maze
     */
    private void setOuterWalls(int rows, int cols) {
        for (int c = 0; c < cols; c++) {
            this.fieldArray[0][c] = new WallField(0, c);
            this.fieldArray[rows - 1][c] = new WallField(rows - 1, c);
        }
        for (int r = 1; r < rows - 1; r++) {
            this.fieldArray[r][0] = new WallField(r, 0);
            this.fieldArray[r][cols - 1] = new WallField(r, cols - 1);
        }
    }
    /**
     * Method to insert one line from string into Maze.
     * @param line number of rows in maze
     */
    public void insertLine(String line) {
        for (int i = 1; i < this.numCols() - 1; i++) {
            switch (line.charAt(i - 1)){
                case '.':
                    this.fieldArray[i][numberOfLines] = new PathField(i, numberOfLines, this.logger);
                    break;
                case 'X':
                    this.fieldArray[i][numberOfLines] = new WallField(i, numberOfLines);
                    break;
                case 'S':
                case 'G':
                case 'T':
                case 'K':
                    this.fieldArray[i][numberOfLines] = new PathField(i, numberOfLines, line.charAt(i - 1), this.logger);
                    break;
            }
        }
        this.numberOfLines++;
    }
    @Override
    public void setFields() {
        for (int c = 0; c < this.numCols(); c++) {
            for (int r = 0; r < this.numRows(); r++) {
                if (c + 1 < this.numCols()) {
                    fieldArray[r][c].setBottomField(fieldArray[r][c + 1]);
                }
                if (r + 1 < this.numRows()) {
                    fieldArray[r][c].setRightField(fieldArray[r + 1][c]);
                }
                if (c - 1 > 0) {
                    fieldArray[r][c].setUpperField(fieldArray[r][c - 1]);
                }
                if (r - 1 > 0) {
                    fieldArray[r][c].setLeftField(fieldArray[r - 1][c]);
                }
            }
        }
    }

    @Override
    public void setEntitiesFields() {
        for (int c = 0; c < numCols(); c++) {
            for (int r = 0; r < numRows(); r++) {
                MazeEntity entity = fieldArray[r][c].getPacmanEntity();
                if (entity != null) {
                    entity.setFields(fieldArray[r][c]);
                }
                entity = fieldArray[r][c].getGhostEntity();
                if (entity != null) {
                    entity.setFields(fieldArray[r][c]);
                }
            }
        }
    }


    @Override
    public int numCols() {
        return this.fieldArray[0].length;
    }

    @Override
    public int numRows() {
        return this.fieldArray.length;
    }

    @Override
    public Field getField(int row, int col) {
        return this.fieldArray[row][col];
    }
}
