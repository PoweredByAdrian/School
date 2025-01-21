package ija.ija2022.homework2.game;

//import ija.ija2022.homework2.common.Field;
//import ija.ija2022.homework2.common.Maze;
//import ija.ija2022.homework2.common.MazeObject;

import ija.ija2022.homework2.tool.common.CommonField;
import ija.ija2022.homework2.tool.common.CommonMaze;
import ija.ija2022.homework2.tool.common.CommonMazeObject;

import java.nio.file.Path;

public class PacmanObject implements CommonMazeObject {
    protected boolean objectHasMoved = false;

    private int lives = 3;
    protected int pacmanIdentificator = 0;
    protected int row;
    protected int col;
    protected CommonField callerField;
    protected CommonField bottomField;
    protected CommonField rightField;
    protected CommonField upperField;
    protected CommonField leftField;

    public PacmanObject(int row, int col) {
        this.row = row;
        this.col = col;
        this.callerField = new PathField(row, col);
    }

    public void setSurroundingFieldsPacman(CommonField bottom, CommonField right, CommonField upper, CommonField left) {
        this.bottomField = bottom;
        this.rightField = right;
        this.upperField = upper;
        this.leftField = left;
    }

    public boolean decreaseLives(){
        if (lives == 0)
            return false; //RIP Pacman
        this.lives -= 1;
        return true;
    }

    @Override
    public boolean canMove(CommonField.Direction dir) {



        if (dir == CommonField.Direction.D && this.bottomField.canMove()) {
            if (this.bottomField.get().isPacman()){
                PacmanObject pacman = (PacmanObject) this.bottomField.get();
                pacman.decreaseLives();
            }
            return true;
        }
        else if (dir == CommonField.Direction.R && this.rightField.canMove()) {
            if (this.rightField.get().isPacman()){
                PacmanObject pacman = (PacmanObject) this.rightField.get();
                pacman.decreaseLives();
            }
            return true;
        }
        else if (dir == CommonField.Direction.U && this.upperField.canMove()) {
            if (this.upperField.get().isPacman()){
                PacmanObject pacman = (PacmanObject) this.upperField.get();
                pacman.decreaseLives();
            }
            return true;
        }
        else if (dir == CommonField.Direction.L && this.leftField.canMove()) {
            if (this.leftField.get().isPacman()){
                PacmanObject pacman = (PacmanObject) this.leftField.get();
                pacman.decreaseLives();
            }
            return true;
        }
        else {
            return false;
        }
    }

    @Override
    public CommonField getField() {
        return this.callerField;
    }

    @Override
    public int getLives() {
        return this.lives;
    }

    @Override
    public boolean move(CommonField.Direction dir) {
/*
        if (dir == CommonField.Direction.D && this.bottomField.canMove()) {
            this.objectHasMoved = true;
            this.bottomField.setPacmanObject();
            return true;
        }
        else if (dir == CommonField.Direction.R && this.rightField.canMove()) {
            this.objectHasMoved = true;
            this.rightField.setPacmanObject();
            return true;
        }
        else if (dir == CommonField.Direction.U && this.upperField.canMove()) {
            this.objectHasMoved = true;
            this.upperField.setPacmanObject();
            return true;
        }
        else if (dir == CommonField.Direction.L && this.leftField.canMove()) {
            this.objectHasMoved = true;
            this.leftField.setPacmanObject();
            return true;
        }*/
        return false;
    }

    @Override
    public boolean isPacman(){return true;}


    public boolean hasMoved() {
        return this.objectHasMoved;
    }

    @Override
    public boolean equals(Object object) {
        System.out.print(this.row + "," + this.col);
        if (object == null) {
            return false;
        }
        if (!(object instanceof PacmanObject)) {
            return false;
        }
        PacmanObject pacman = (PacmanObject) object;
        return pacman instanceof PacmanObject;
    }
}
