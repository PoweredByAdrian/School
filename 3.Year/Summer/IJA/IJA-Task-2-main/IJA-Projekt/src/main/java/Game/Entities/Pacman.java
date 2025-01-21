package Game.Entities;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import Game.Triplet;
import Interfaces.Field;
import Interfaces.MazeEntity;
import Game.Logger;
import java.awt.*;
import java.util.*;
import java.util.List;

public class Pacman implements MazeEntity  {
    private Logger logger;
    private Point pacmanPosition = new Point();
    private boolean setOnce = true;
    private int lives;
    private Field leftField;
    private Field rightField;
    private Field bottomField;
    private Field upperField;
    private Field actualField;

    public Pacman(int x, int y, int lives, Logger logger) {
        pacmanPosition.x = x;
        pacmanPosition.y = y;
        this.lives = lives;
        this.logger = logger;
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
    public boolean decreaseLives(){
        lives -= 1;
        if (lives == 0) {
            return false;
        }
        logger.logMessageLives(lives);
        return true;
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
                if (leftField != null) {
                    leftField.movePacman(pacmanPosition.x - 1, pacmanPosition.y, lives);
                    logger.logMessageMove(this, this.pacmanPosition.x, this.pacmanPosition.y, x, y);
                    return true;
                }
                return false;
            case R:
                if (rightField != null) {
                    rightField.movePacman(pacmanPosition.x + 1, pacmanPosition.y, lives);
                    logger.logMessageMove(this, this.pacmanPosition.x, this.pacmanPosition.y, x, y);
                    return true;
                }
                return false;
            case U:
                if (upperField != null) {
                    upperField.movePacman(pacmanPosition.x, pacmanPosition.y - 1, lives);
                    logger.logMessageMove(this, this.pacmanPosition.x, this.pacmanPosition.y, x, y);
                    return true;
                }
                return false;
            case D:
                if (bottomField != null) {
                    bottomField.movePacman(pacmanPosition.x, pacmanPosition.y + 1, lives);
                    logger.logMessageMove(this, this.pacmanPosition.x, this.pacmanPosition.y, x, y);
                    return true;
                }
                return false;
        }
        return false;
    }
    @Override
    public int getLives() {
        return lives;
    }
    @Override
    public void setLives(int lives) {
        this.lives = lives;
    }
    @Override
    public Field getField(){
        return this.actualField;
    }
    @Override
    public boolean isPacman(){
        return true;
    }

    @Override
    public Point getPosition() {
        return pacmanPosition;
    }
    @Override
    public void setGhost(Field field) {

    }
    // Breadth-First search
    @Override
    public List findPath(Field startPosition, Field endPosition) {
        LinkedList<Triplet> queue = new LinkedList<>();
        LinkedList<Triplet> visited = new LinkedList<>();
        queue.add(new Triplet(startPosition, null, null));
        while (queue.size() > 0) {
            if (checkField(queue, visited, endPosition)) {
                break;
            }
        }
        LinkedList<Triplet> path = new LinkedList<>();
        if (startPosition.equals(endPosition)) {
            return path;
        }
        Field currentPath = endPosition;
        for (int i = 0; i < visited.size(); i++) {
            if (visited.get(i).getCurrentField().equals(currentPath)) {
                path.add(visited.get(i));
                currentPath = visited.get(i).getPreviousField();
                i = 0;
            }
        }
        return path;
    }

    private boolean checkField(LinkedList<Triplet> queue, LinkedList<Triplet> visited, Field endPosition) {
        Triplet field = queue.pop();
        if (field.getCurrentField() == null || !field.getCurrentField().canMove()) {
            return false;
        }
        if (visited.size() != 0) {
            for (Triplet visitedField : visited) {
                if (field.getCurrentField().equals(visitedField.getCurrentField())) {
                    return false;
                }
            }
            if (field.getCurrentField().equals(endPosition)) {
                visited.add(new Triplet(field.getCurrentField(), field.getPreviousField(), field.getDirection()));
                return true;
            }
        }
        //((PathField)field.getCurrentField()).print();
        // ((PathField)endPosition).print();
        visited.add(new Triplet(field.getCurrentField(), field.getPreviousField(), field.getDirection()));

        queue.add(new Triplet(field.getCurrentField().nextField(Field.Direction.U),
                field.getCurrentField(), Field.Direction.U));
        queue.add(new Triplet(field.getCurrentField().nextField(Field.Direction.R),
                field.getCurrentField(), Field.Direction.R));
        queue.add(new Triplet(field.getCurrentField().nextField(Field.Direction.D),
                field.getCurrentField(), Field.Direction.D));
        queue.add(new Triplet(field.getCurrentField().nextField(Field.Direction.L),
                field.getCurrentField(), Field.Direction.L));
        return false;
    }
}
