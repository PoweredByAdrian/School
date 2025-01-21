package ija.ija2022.homework2.common;

import ija.ija2022.homework2.common.Maze;
public interface Field {
    enum Direction { D, L, R, U };

    boolean canMove();

    MazeObject get();

    boolean isEmpty();

    Field nextField(Field.Direction dirs);

    boolean put(MazeObject object);

    boolean remove(MazeObject object);

    void setMaze(Maze maze);

    void setSurroundingFields(Field bottom, Field right, Field upper, Field left);

    char getFieldType();

    void setPacmanObject();

    void createNewPacman(int dirRow, int dirCol);
}
