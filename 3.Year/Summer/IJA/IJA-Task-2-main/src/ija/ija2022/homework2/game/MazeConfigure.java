package ija.ija2022.homework2.game;

import  ija.ija2022.homework2.tool.common.CommonMaze;

public class MazeConfigure {
    protected MazeClass newMaze = new MazeClass();
    protected boolean correctLine = true;
    protected boolean doneReading = false;
    private int countLines = 0;

    public MazeConfigure() {

    }
    public void startReading(int rows, int cols) {
        newMaze.createArray(rows + 2, cols + 2);
    }
    public boolean processLine(String line) {
        this.countLines++;
        if (this.correctLine) {
            // TODO
            if (newMaze.numCols() != line.length() + 2) {
                return this.correctLine = false;
            }
            newMaze.insertLine(line);
        }
        return this.correctLine;
    }
    public boolean stopReading() {
        if (this.countLines != newMaze.numRows() - 2) {
            return false;
        }
        newMaze.setFields();
        return this.doneReading = true;
    }
    public CommonMaze createMaze() {
        if (this.doneReading && this.correctLine) {
            return this.newMaze;
        }
        else {
            return null;
        }
    }
}
