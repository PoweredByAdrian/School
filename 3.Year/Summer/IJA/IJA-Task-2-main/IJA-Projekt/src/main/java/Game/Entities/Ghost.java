package Game.Entities;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import Game.Logger;
import Interfaces.Field;
import Interfaces.MazeEntity;

import java.awt.*;

/**
 * Class represents ghosts in maze
 */
public class Ghost implements MazeEntity {
    private Logger logger;
    private Point ghostPosition = new Point();
    private Point ghostCameFrom = new Point(0, 0);
    private int lives = 3;
    private Field leftField;
    private Field rightField;
    private Field bottomField;
    private Field upperField;
    private Field actualField;

    public Ghost(int x, int y, int cameFromX, int cameFromY, Logger logger) {
        ghostPosition.x = x;
        ghostPosition.y = y;
        ghostCameFrom.setLocation(cameFromX, cameFromY);
        this.logger = logger;
    }
    @Override
    public void setGhost(Field field) {
        this.actualField = field;
        this.leftField = field.nextField(Field.Direction.L);
        this.rightField = field.nextField(Field.Direction.R);
        this.bottomField = field.nextField(Field.Direction.D);
        this.upperField = field.nextField(Field.Direction.U);
    }
    @Override
    public boolean canMove(Field.Direction dir){
        switch (dir){
            case L -> {
                if (leftField != null) {
                    return this.leftField.canMove();
                }
            }
            case R -> {
                if (rightField != null) {
                    return this.rightField.canMove();
                }
            }
            case U -> {
                if (upperField != null) {
                    return this.upperField.canMove();
                }
            }
            case D -> {
                if (bottomField != null) {
                    return this.bottomField.canMove();
                }
            }
        }
        return false;
    }
    @Override
    public boolean move(Field.Direction dir, int x, int y, Point cameFrom) {
        switch (dir) {
            case L:
                leftField.moveGhost(ghostPosition.x - 1, ghostPosition.y, cameFrom.x, cameFrom.y);
                logger.logMessageMove(this, this.ghostPosition.x, this.ghostPosition.y, x, y);

                /*if (leftField.hasEntity()){
                    if (leftField.getEntity().isPacman()) {
                        Pacman pacman = (Pacman) leftField.getEntity();
                        pacman.decreaseLives();
                    }
                }*/

                return true;
            case R:
                rightField.moveGhost(ghostPosition.x + 1, ghostPosition.y, cameFrom.x, cameFrom.y);
                logger.logMessageMove(this, this.ghostPosition.x, this.ghostPosition.y, x, y);

                /*if (rightField.hasEntity()){
                    if (rightField.getEntity().isPacman()) {
                        Pacman pacman = (Pacman) rightField.getEntity();
                        pacman.decreaseLives();
                    }
                }*/

                return true;
            case U:
                upperField.moveGhost(ghostPosition.x, ghostPosition.y - 1, cameFrom.x, cameFrom.y);
                logger.logMessageMove(this, this.ghostPosition.x, this.ghostPosition.y, x, y);

                /*if (upperField.hasEntity()){
                    if (upperField.getEntity().isPacman()) {
                        Pacman pacman = (Pacman) upperField.getEntity();
                        pacman.decreaseLives();
                    }
                }*/

                return true;
            case D:
                bottomField.moveGhost(ghostPosition.x, ghostPosition.y + 1, cameFrom.x, cameFrom.y);
                logger.logMessageMove(this, this.ghostPosition.x, this.ghostPosition.y, x, y);

                /*if (bottomField.hasEntity()){
                    if (bottomField.getEntity().isPacman()) {
                        Pacman pacman = (Pacman) bottomField.getEntity();
                        pacman.decreaseLives();
                    }
                }*/

                return true;
        }
        //TODO move
        return false;
    }
    @Override
    public Field getField(){
        return this.actualField;
    }

    @Override
    public Point getPosition() {
        return ghostPosition;
    }

    @Override
    public void setFields(Field field) {
        this.actualField = field;
        this.leftField = field.nextField(Field.Direction.L);
        this.rightField = field.nextField(Field.Direction.R);
        this.bottomField = field.nextField(Field.Direction.D);
        this.upperField = field.nextField(Field.Direction.U);
    }

    @Override
    public Point Movement(Point cameFrom){
        while (true) {
            Field.Direction dir = Field.Direction.randomDirection();
            if (canMove(dir)) {
                switch (dir) {
                    case L -> {
                        Point pointTo = new Point(-1, 0);
                        if (!pointTo.equals(flipPoint(ghostCameFrom))) {
                            move(dir, ghostPosition.x - 1, ghostPosition.y, pointTo);
                            return pointTo;
                        }
                    }
                    case R -> {
                        Point pointTo = new Point(1, 0);
                        if (!pointTo.equals(flipPoint(ghostCameFrom))) {
                            ghostCameFrom.setLocation(pointTo.x, pointTo.y);
                            move(dir, ghostPosition.x + 1, ghostPosition.y, pointTo);
                            return pointTo;
                        }
                    }
                    case U -> {
                        Point pointTo = new Point(0, -1);
                        if (!pointTo.equals(flipPoint(ghostCameFrom))) {
                            ghostCameFrom.setLocation(pointTo.x, pointTo.y);
                            move(dir, ghostPosition.x, ghostPosition.y - 1, pointTo);
                            return pointTo;
                        }
                    }
                    case D -> {
                        Point pointTo = new Point(0, 1);
                        if (!pointTo.equals(flipPoint(ghostCameFrom))) {
                            ghostCameFrom.setLocation(pointTo.x, pointTo.y);
                            move(dir, ghostPosition.x, ghostPosition.y + 1, pointTo);
                            return pointTo;
                        }
                    }
                }
            }
        }
    }
    private Point flipPoint(Point cameFrom) {
        return new Point(-cameFrom.x, -cameFrom.y);
    }
}
