package ija.ija2022.homework2.game;

//import ija.ija2022.homework2.common.Field;
//import ija.ija2022.homework2.common.Maze;
//import ija.ija2022.homework2.common.MazeObject;

import ija.ija2022.homework2.tool.common.CommonField;
//import ija.ija2022.homework2.tool.common.CommonMaze;
import ija.ija2022.homework2.tool.common.CommonMazeObject;
import ija.ija2022.homework2.tool.common.Observable;
import ija.ija2022.homework2.tool.view.FieldView;

//import java.nio.file.Path;

public class GhostObject implements CommonMazeObject {
    protected boolean objectHasMoved = false;
    //protected int pacmanIdentificator = 0;
    protected int row;
    protected int col;
    protected PathField callerField;
    protected CommonField bottomField;
    protected CommonField rightField;
    protected CommonField upperField;
    protected CommonField leftField;

    public GhostObject(int row, int col, PathField Field) {
        this.row = row;
        this.col = col;
        this.callerField = Field;
    }


    public void setSurroundingFieldsGhost(CommonField bottom, CommonField right, CommonField upper, CommonField left, PathField curPath) {
        this.bottomField = bottom;
        this.rightField = right;
        this.upperField = upper;
        this.leftField = left;
        this.callerField = curPath;
    }

    @Override
    public boolean canMove(CommonField.Direction dir) {
        if (dir == CommonField.Direction.D && this.bottomField.canMove()) {
            return true;
        }
        else if (dir == CommonField.Direction.R && this.rightField.canMove()) {
            return true;
        }
        else if (dir == CommonField.Direction.U && this.upperField.canMove()) {
            return true;
        }
        else if (dir == CommonField.Direction.L && this.leftField.canMove()) {
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
        return 0;
    }

    @Override
    public boolean move(CommonField.Direction dir) {
        if (dir == CommonField.Direction.D && canMove(dir)) {

            PathField thisPath = (PathField) this.callerField;
            if (!thisPath.fieldObject.isPacman())
                thisPath.objectMoved();
            PathField nextPath = (PathField) this.bottomField;




            if (nextPath.fieldObject != null && nextPath.fieldObject.isPacman()){
                PacmanObject pacman = (PacmanObject) nextPath.get();
                pacman.decreaseLives();
            }
            if (nextPath.isEmpty())
                nextPath.setGhostObject(this);
            else
                nextPath.setObjectFields(this);
            thisPath.notifyObservers();
            nextPath.notifyObservers();

            row += 1;

            return true;
        }
        else if (dir == CommonField.Direction.R && canMove(dir)) {

            PathField thisPath = (PathField) this.callerField;
            if (!thisPath.fieldObject.isPacman())
                thisPath.objectMoved();
            PathField nextPath = (PathField) this.rightField;




            if (nextPath.fieldObject != null && nextPath.fieldObject.isPacman()){
                PacmanObject pacman = (PacmanObject) nextPath.get();
                pacman.decreaseLives();
            }
            if (nextPath.isEmpty())
                nextPath.setGhostObject(this);
            else
                nextPath.setObjectFields(this);
            thisPath.notifyObservers();
            nextPath.notifyObservers();
            col += 1;

            return true;
        }
        else if (dir == CommonField.Direction.U && canMove(dir)) {

            PathField thisPath = (PathField) this.callerField;
            if (!thisPath.fieldObject.isPacman())
                thisPath.objectMoved();
            PathField nextPath = (PathField) this.upperField;




            if (nextPath.fieldObject != null && nextPath.fieldObject.isPacman()){
                PacmanObject pacman = (PacmanObject) nextPath.get();
                pacman.decreaseLives();
            }
            if (nextPath.isEmpty())
                nextPath.setGhostObject(this);
            else
                nextPath.setObjectFields(this);
            thisPath.notifyObservers();
            nextPath.notifyObservers();
            row -= 1;

            return true;
        }
        else if (dir == CommonField.Direction.L && canMove(dir)) {

            PathField thisPath = (PathField) this.callerField;
            if (!thisPath.fieldObject.isPacman())
                thisPath.objectMoved();
            PathField nextPath = (PathField) this.leftField;


            //thisPath.notifyObservers();


            if (nextPath.fieldObject != null && nextPath.fieldObject.isPacman()){
                PacmanObject pacman = (PacmanObject) nextPath.get();
                pacman.decreaseLives();
            }
            if (nextPath.isEmpty())
                nextPath.setGhostObject(this);
            else
                nextPath.setObjectFields(this);

            thisPath.notifyObservers();
            nextPath.notifyObservers();
            col -= 1;

            return true;
        }
        // TODO
        return false;
    }


    public boolean hasMoved() {
        return this.objectHasMoved;
    }

    @Override
    public boolean equals(Object object) {
        System.out.print(this.row + "," + this.col);
        if (object == null) {
            return false;
        }
        if (!(object instanceof GhostObject)) {
            return false;
        }
        GhostObject ghost = (GhostObject) object;
        return ghost instanceof GhostObject;
    }
}