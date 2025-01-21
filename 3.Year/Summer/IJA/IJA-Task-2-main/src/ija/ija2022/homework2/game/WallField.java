package ija.ija2022.homework2.game;

//import ija.ija2022.homework2.common.Field;
//import ija.ija2022.homework2.common.Maze;
//import ija.ija2022.homework2.common.MazeObject;


import ija.ija2022.homework2.tool.common.AbstractObservableField;
import ija.ija2022.homework2.tool.common.CommonField;
import ija.ija2022.homework2.tool.common.CommonMazeObject;
import ija.ija2022.homework2.tool.common.Observable;

public class WallField extends Object implements CommonField {
    protected int row;
    protected int col;
    protected char fieldType;
    protected CommonField bottomField;
    protected CommonField rightField;
    protected CommonField upperField;
    protected CommonField leftField;

    public WallField(int row, int col) {
        this.row = row;
        this.col = col;
        this.fieldType = 'X';
    }

    public void setPacmanObject() {
        throw new UnsupportedOperationException("Nepovolena operacia nad tymto objektom\n");
    }

    @Override
    public boolean canMove() {
        return fieldType != 'X';
    }

    @Override
    public boolean contains (CommonMazeObject object){
        //TODO
        return true;
    }

    @Override
    public CommonMazeObject get() {
        return null;
    }

    @Override
    public boolean isEmpty() {
        return fieldType == 'X';
    }

    @Override
    public CommonField nextField(CommonField.Direction dirs) {
        throw new UnsupportedOperationException("Nepovolena operacia nad tymto objektom\n");
    }



    @Override
    public void addObserver(Observable.Observer o){

    }

    @Override
    public void removeObserver(Observable.Observer o){

    }

    @Override
    public void notifyObservers(){

    }

    /*@Override
    public boolean put(CommonMazeObject object) {
        throw new UnsupportedOperationException("Nepovolena operacia nad tymto objektom\n");
    }*/

    /*@Override
    public boolean remove(CommonMazeObject object) {
        throw new UnsupportedOperationException("Nepovolena operacia nad tymto objektom\n");
    }*/

    /*@Override
    public void setMaze(CommonMaze maze) {
    }*/

    @Override
    public boolean equals(Object object) {
        if (object == null) {
            return false;
        }
        if (!(object instanceof WallField)) {
            return false;
        }
        WallField wall = (WallField) object;
        return row == wall.row && col == wall.col && fieldType == wall.fieldType;
    }

    //@Override
    //TODO
    public void setSurroundingFields(CommonField bottom, CommonField right, CommonField upper, CommonField left) {
        this.bottomField = bottom;
        this.rightField = right;
        this.upperField = upper;
        this.leftField = left;
    }

    public char getFieldType() {
        return this.fieldType;
    }

    /*@Override
    public void createNewPacman(int dirRow, int dirCol) {

    }*/
}
