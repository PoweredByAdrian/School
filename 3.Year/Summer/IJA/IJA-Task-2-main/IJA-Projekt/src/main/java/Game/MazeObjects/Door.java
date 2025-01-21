package Game.MazeObjects;

import Game.Logger;
import Interfaces.MazeObject;
/**
 * Class represents Door in maze
 */
public class Door implements MazeObject {
    private boolean open = false;
    private int row;
    private int col;
    private Logger logger;
    public Door(int row, int col, Logger logger){
        this.row = row;
        this.col = col;
        this.logger = logger;
    }
    /**
     * Method to check if door are open.
     * @return  true when open
     */
    public boolean isOpen(){
        return open;
    }
    @Override
    public void openDoor(){
        this.logger.logMessageDoors(this.row, this.col);
        open = true;
    }
    public void closeTheDoors(){
        open = false;
    }
}
