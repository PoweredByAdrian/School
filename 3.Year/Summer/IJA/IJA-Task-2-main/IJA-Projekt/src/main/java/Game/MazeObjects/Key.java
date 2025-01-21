package Game.MazeObjects;

import Game.Logger;
import Interfaces.MazeObject;
/**
 * Class represents Keys in maze
 */
public class Key implements MazeObject {
    private Logger logger;
    private boolean onGround = true;
    private int x;
    private int y;

    public Key(int x, int y, Logger logger){
        this.x = x;
        this.y = y;
        onGround = true;
        this.logger = logger;
    }
    @Override
    public void pickUpKey(){
        onGround = false;
        logger.logMessageKey(this.x, this.y);
    }
    @Override
    public boolean keyTaken() {
        return onGround;
    }
}
