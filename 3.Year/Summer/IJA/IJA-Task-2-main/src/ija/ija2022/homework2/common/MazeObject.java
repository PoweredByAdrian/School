package ija.ija2022.homework2.common;

public interface MazeObject {
    enum Direction { D, L, R, U };

    void setSurroundingFieldsPacman(Field bottom, Field right, Field upper, Field left);

    boolean canMove(Field.Direction dir);

    boolean move(Field.Direction dir);

    boolean hasMoved();
}
