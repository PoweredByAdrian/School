package ija.ija2022.homework2.common;

import ija.ija2022.homework2.common.Field;
import ija.ija2022.homework2.game.PathField;

public interface Maze {
    public Field getField(int row, int col);

    public int numCols();

    public int numRows();

    public void createArray(int rows, int cols);

}
