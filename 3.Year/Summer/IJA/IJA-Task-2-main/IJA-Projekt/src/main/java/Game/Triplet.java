package Game;

/**
 * Authors: Matus Tabi, xtabim01
 */

import Interfaces.Field;

public class Triplet {
    final private Field currentField;
    final private Field previousField;
    final private Field.Direction direction;

    /**
     * Class representing triplet used in finding path with Breath-First search algorithm
     * @param currentField Current field where pacman stands
     * @param previousField Goal field
     * @param direction Direction pacman is facing
     */
    public Triplet(Field currentField, Field previousField, Field.Direction direction) {
        this.currentField = currentField;
        this.previousField = previousField;
        this.direction = direction;
    }

    /**
     * @return Current field where pacman stands
     */
    public Field getCurrentField() {
        return currentField;
    }

    /**
     *
     * @return Previous field where pacman was standing
     */
    public Field getPreviousField() {
        return previousField;
    }

    /**
     *
     * @return Direction pacman is facing
     */
    public Field.Direction getDirection() {
        return direction;
    }
}