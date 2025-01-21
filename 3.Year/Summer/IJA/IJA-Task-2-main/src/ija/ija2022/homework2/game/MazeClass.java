package ija.ija2022.homework2.game;



import ija.ija2022.homework2.tool.common.CommonField;
import ija.ija2022.homework2.tool.common.CommonMaze;
import ija.ija2022.homework2.tool.common.CommonMazeObject;

import java.util.LinkedList;
import java.util.List;

public class MazeClass implements CommonMaze {
    protected CommonField[][] fieldArray;
    private int numberOfLines = 1;
    private List<CommonMazeObject> listOfGhosts = new LinkedList<CommonMazeObject>();

    public void createArray(int rows, int cols) {
        this.fieldArray = new CommonField[rows][cols];
        this.horizontalWall(rows, cols);
        this.verticalWall(rows, cols);
    }

    @Override
    public int numCols() {
        return this.fieldArray[0].length;
    }

    @Override
    public List<CommonMazeObject> ghosts() {
        List<CommonMazeObject> clonedList = new LinkedList<CommonMazeObject>(this.listOfGhosts);
        return clonedList;
    }

    @Override
    public int numRows() {
        return this.fieldArray.length;
    }

    @Override
    public CommonField getField(int row, int col) {
        return this.fieldArray[row][col];
    }

    /*
    Vkladanie horizontalnych stien do bludiska na nulty a posledny riadok.
     */
    private void horizontalWall(int rows, int cols) {
        for (int c = 0; c < cols; c++) {
            this.fieldArray[0][c] = new WallField(0, c);
            this.fieldArray[rows - 1][c] = new WallField(rows - 1, c);
        }
    }
    /*
    Vkladanie vertikalnych stien do bludiska, indexovane od prveho riadka po predposledny
     */
    private void verticalWall(int rows, int cols) {
        for (int r = 1; r < rows - 1; r++) {
            this.fieldArray[r][0] = new WallField(r, 0);
            this.fieldArray[r][cols - 1] = new WallField(r, cols - 1);
        }
    }

    public void insertLine(String line) {
        for (int i = 1; i < this.numCols() - 1; i++) {
            if (line.charAt(i - 1) == '.') {
                this.fieldArray[this.numberOfLines][i] = new PathField(this.numberOfLines, i);
            }
            if (line.charAt(i - 1) == 'X') {
                this.fieldArray[this.numberOfLines][i] = new WallField(this.numberOfLines, i);
            }
            if (line.charAt(i - 1) == 'S') {
                this.fieldArray[this.numberOfLines][i] = new PathField(this.numberOfLines, i, 'S');
            }
            if (line.charAt(i - 1) == 'G') {
                this.fieldArray[this.numberOfLines][i] = new PathField(this.numberOfLines, i, 'G');
                if (!this.fieldArray[this.numberOfLines][i].isEmpty()) {
                    this.insertGhost(this.fieldArray[this.numberOfLines][i]);
                }
            }
        }
        this.numberOfLines++;
    }
    public void setFields() {
        for (int r = 1; r < this.numRows() - 1; r++) {
            for (int c = 1; c < this.numCols() - 1; c++) {
                if (this.fieldArray[r][c] instanceof PathField) {
                    PathField path = (PathField) this.fieldArray[r][c];
                    path.setSurroundingFields(
                            this.fieldArray[r + 1][c],
                            this.fieldArray[r][c + 1],
                            this.fieldArray[r - 1][c],
                            this.fieldArray[r][c - 1]
                    );
                }
            }
        }
        //TODO
    }
    private void insertGhost(CommonField field) {
        this.listOfGhosts.add(field.get());
    }
}


