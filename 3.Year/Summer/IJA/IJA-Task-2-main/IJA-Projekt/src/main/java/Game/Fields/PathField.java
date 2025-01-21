package Game.Fields;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import Game.Entities.Ghost;
import Game.Entities.Pacman;
import Game.Logger;
import Game.MazeObjects.Door;
import Game.MazeObjects.Key;
import Interfaces.Field;
import Interfaces.MazeEntity;
import Interfaces.MazeObject;

import java.awt.*;
/**
 * Class represents Path in maze (where entity can be placed).
 */
public class PathField implements Field {
    private int row;
    private int col;
    private Logger logger;
    private MazeObject object = null;
    private MazeEntity ghostEntity = null;
    private MazeEntity pacmanEntity = null;
    private Field leftField;
    private Field rightField;
    private Field bottomField;
    private Field upperField;
    private boolean ownPoint;

    @Override
    public void pointTaken(){
        ownPoint = false;
    }
    @Override
    public  boolean containPoint(){
        return ownPoint;
    }

    @Override
    public Point getFieldPosition() {
        return new Point(row, col);
    }

    @Override
    public Field nextField(Direction dir){
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
    public boolean hasGhost(){
        return !(ghostEntity == null);
    }
    @Override
    public boolean hasPacman() {
        return !(pacmanEntity == null);
    }
    @Override
    public boolean hasObject(){
        return !(object == null);
    }
    @Override
    public boolean canMove(){
        return true;
    }
    @Override
    public MazeEntity getPacmanEntity(){
        return pacmanEntity;
    }
    @Override
    public MazeEntity getGhostEntity() {
        return ghostEntity;
    }
    public MazeObject getObject(){
        return object;
    }

    public PathField(int row, int col, char c, Logger logger){
        this.logger = logger;
        this.row = row;
        this.col = col;
        switch (c){
            case 'G' -> ghostEntity = new Ghost(row, col, 0, 0, logger);
            case 'S' -> pacmanEntity = new Pacman(row, col, 3, logger);
            case 'T' -> object = new Door(row, col, logger);
            case 'K' -> object = new Key(row, col, logger);
        }
        ownPoint = false;
    }
    public PathField(int row, int col, Logger logger) {
        this.row = row;
        this.col = col;
        this.ownPoint = true;
        this.logger = logger;
    }
    @Override
    public void setBottomField(Field bottom) {
        this.bottomField = bottom;
        //setPacmanFields();
    }
    @Override
    public void setRightField(Field right) {
        this.rightField = right;
        //setPacmanFields();
    }
    @Override
    public void setUpperField(Field upper) {
        this.upperField = upper;
        //setPacmanFields();
    }
    @Override
    public void setLeftField(Field left) {
        this.leftField = left;
        //setPacmanFields();
    }
    @Override
    public void movePacman(int x, int y, int lives) {
        if (pacmanEntity == null) {
            pacmanEntity = new Pacman(x, y, lives, logger);
            pacmanEntity.setFields(this);
        }
        else {
            pacmanEntity.setLives(lives);
        }
    }
    @Override
    public void moveGhost(int x, int y, int cameFromX, int cameFromY) {
        if (ghostEntity == null) {
            ghostEntity = new Ghost(x, y, cameFromX, cameFromY, logger);
            ghostEntity.setFields(this);
        }
    }
    @Override
    public void removeGhost() {
        ghostEntity = null;
    }
    @Override
    public void removePacman() {
        pacmanEntity = null;
    }
    @Override
    public boolean doorIsOpen() {
        if (object != null) {
            return object.isOpen();
        }
        return false;
    }
    @Override
    public boolean equals(Object o) {
        if (o == this) {
            return true;
        }
        if (!(o instanceof PathField)) {
            return false;
        }
        return row == ((PathField) o).row && col == ((PathField) o).col;
    }
}
