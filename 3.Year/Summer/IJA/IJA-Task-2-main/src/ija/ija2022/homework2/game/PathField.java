package ija.ija2022.homework2.game;

import ija.ija2022.homework2.common.Field;
import ija.ija2022.homework2.common.Maze;
import ija.ija2022.homework2.common.MazeObject;
import ija.ija2022.homework2.tool.common.AbstractObservableField;
import ija.ija2022.homework2.tool.common.CommonField;
import ija.ija2022.homework2.tool.common.CommonMazeObject;
import ija.ija2022.homework2.tool.common.Observable;

public class PathField extends AbstractObservableField implements CommonField {
    protected int row;
    protected int col;
    protected char fieldType;
    protected char typeOfObject;
    protected CommonField bottomField;
    protected CommonField rightField;
    protected CommonField upperField;
    protected CommonField leftField;
    public CommonMazeObject fieldObject;
    public PathField(int row, int col) {
        this.row = row;
        this.col = col;
        this.fieldType = '.';
        this.typeOfObject = 0;
        this.fieldObject = null;
    }
    public PathField(int row, int col, char typeOfObject) {
        this.row = row;
        this.col = col;
        this.fieldType = '.';
        this.typeOfObject = typeOfObject;
        if (typeOfObject == 'S')
            this.fieldObject = new PacmanObject(row, col);
        else if (typeOfObject == 'G')
            this.fieldObject = new GhostObject(row, col, this);
    }



    public void setPacmanObject() {
        this.fieldObject = new PacmanObject(this.row, this.col);
        this.typeOfObject = 'S';
    }

    public void setGhostObject() {
        this.fieldObject = new GhostObject(this.row, this.col, this);
        this.typeOfObject = 'G';
    }

    public void setPacmanObject(CommonMazeObject fieldObject) {
        this.fieldObject = fieldObject;
        this.typeOfObject = 'S';
        this.setSurroundingFields(this.bottomField, this.rightField, this.upperField, this.leftField);
    }

    public void setGhostObject(CommonMazeObject fieldObject) {
        this.fieldObject = fieldObject;
        this.typeOfObject = 'G';
        this.setSurroundingFields(this.bottomField, this.rightField, this.upperField, this.leftField);
    }

    public void objectMoved(){
        this.fieldObject = null;
    }

    @Override
    public boolean canMove() {
        return !(this.fieldType == 'X');
    }

    @Override
    public boolean contains(CommonMazeObject obj){
        if (obj.equals(this.get())) {
            return true;
        }
        return false;
    }

    @Override
    public CommonMazeObject get() {
        return this.fieldObject;
    }

    @Override
    public boolean isEmpty() {
        if (this.fieldObject == null) {
            return true;
        }
        return false;
    }

    @Override
    public CommonField nextField(CommonField.Direction dirs) {
        if (dirs == Direction.D) {
            return this.bottomField;
        }
        else if (dirs == Direction.R) {
            return this.rightField;
        }
        else if (dirs == Direction.U)
            return this.upperField;
        else if (dirs == Direction.L){
            return this.leftField;
        }
        else {
            return null;
        }
    }

    /*@Override
    public boolean put(MazeObject object) {
        if (this.isEmpty()) {
            this.fieldObject = object;
            return true;
        }
        return false;
    }*/

    /*@Override
    public boolean remove(MazeObject object) {
        if (this.fieldObject != null && object instanceof PacmanObject) {
            this.fieldObject = null;
            return true;
        }
        return false;
    }*/

    /*@Override
    public void setMaze(Maze maze) {

    }*/

    @Override
    public boolean equals(Object object) {
        if (object == null) {
            return false;
        }
        if (!(object instanceof PathField)) {
            return false;
        }
        PathField path = (PathField) object;
        return row == path.row && col == path.col && fieldType == path.fieldType;
    }

    /*@Override
    public void addObserver(Observable.Observer o){
        //this.newObserver = o;
    }

    @Override
    public void removeObserver(Observable.Observer o){
        //this.newObserver = null;
    }

    @Override
    public void notifyObservers(){

    }*/
    public void setObjectFields(CommonMazeObject object){

        if (object instanceof PacmanObject) {
            PacmanObject pacman = (PacmanObject) object;
            pacman.setSurroundingFieldsPacman(this.bottomField, this.rightField, this.upperField, this.leftField);
        }
        else if (object instanceof GhostObject) {
            GhostObject ghost = (GhostObject) object;
            ghost.setSurroundingFieldsGhost(this.bottomField, this.rightField, this.upperField, this.leftField, this);
        }
    }

    public void setSurroundingFields(CommonField bottom, CommonField right, CommonField upper, CommonField left) {
        this.bottomField = bottom;
        this.rightField = right;
        this.upperField = upper;
        this.leftField = left;

        if (this.fieldObject != null) {
            if (this.fieldObject instanceof PacmanObject) {
                PacmanObject pacman = (PacmanObject) this.fieldObject;
                pacman.setSurroundingFieldsPacman(bottom, right, upper, left);
            }
            else if (this.fieldObject instanceof GhostObject) {
                GhostObject ghost = (GhostObject) this.fieldObject;
                ghost.setSurroundingFieldsGhost(bottom, right, upper, left, this);
            }
        }
    }
    public char getFieldType() {
        return this.fieldType;
    }
}
