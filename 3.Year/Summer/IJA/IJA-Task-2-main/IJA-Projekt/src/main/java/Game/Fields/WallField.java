package Game.Fields;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import Interfaces.Field;

import java.awt.*;
/**
 * Class represents Wall in maze (where entity can NOT be placed).
 */
public class WallField implements Field {
    private Field leftField;
    private Field rightField;
    private Field bottomField;
    private Field upperField;
    private int row;
    private int col;
    public WallField(int row, int col) {
        this.row = row;
        this.col = col;
    }

    @Override
    public Point getFieldPosition() {
        return new Point(row, col);
    }

    @Override
    public Field nextField(Direction dir) {
        switch (dir){
            case L:
                return this.leftField;
            case R:
                return this.rightField;
            case U:
                return this.upperField;
            case D:
                return this.bottomField;
            default:
                return null;
        }
    }

    @Override
    public boolean canMove(){
        return false;
    }
    @Override
    public  void pointTaken(){
    }

    @Override
    public void setBottomField(Field bottom) {
        this.bottomField = bottom;
    }

    @Override
    public void setRightField(Field right) {
        this.rightField = right;
    }

    @Override
    public void setUpperField(Field upper) {
        this.upperField = upper;
    }

    @Override
    public void setLeftField(Field left) {
        this.leftField = left;
    }

    @Override
    public void movePacman(int x, int y, int lives) {

    }
    @Override
    public void moveGhost(int x, int y, int cameFromX, int cameFromY) {

    }
    @Override
    public boolean equals(Object o) {
        if (o == this) {
            return true;
        }
        if (!(o instanceof WallField)) {
            return false;
        }
        return row == ((WallField) o).row && col == ((WallField) o).col;
    }
}

