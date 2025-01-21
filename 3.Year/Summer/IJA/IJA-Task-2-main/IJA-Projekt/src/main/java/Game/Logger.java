package Game;

/**
 * Authors: Adrian Horvath, xhorva14
 */

import Game.Entities.Pacman;
import Interfaces.MazeEntity;
import java.io.FileWriter;
import java.io.IOException;

/**
 * Class for writing logs in file
 */
public class Logger {
    private FileWriter fileWriter;
    private String filePath;
    private String lastMessage;
    private int turn = 0;

    public Logger() throws IOException {
        this.filePath = "Log.log";
        this.lastMessage = null;
        fileWriter = new FileWriter(filePath);
    }
    /**
     * Print Zero turn in case ghosts will move firsts.
     */
    public void printTurnZero()
    {
        String message = "TURN " + turn;
        this.log(message);
    }
    /**
     * Method for printing size of maze
     * @param rows number of rows in maze
     * @param cols number of cols in maze

     */
    public void printFirstLine(int rows, int cols)
    {
        String message = rows + " " + cols;
        this.log(message);
    }
    /**
     * Function that print Maze in log file
     * @param line Line to be printed
     */
    public void printMazeLine(String line)
    {
        this.log(line);
    }
    /**
     * Method that print Log for movement
     * @param entity Ghost/Pacman
     * @param x x coordinates
     * @param y y coordinates
     * @param tox final x coordinates
     * @param toy final y coordinates
     */
    public void logMessageMove(MazeEntity entity, int x, int y, int tox, int toy) {
        String message;
        String turns;
        if (entity instanceof Pacman){
            message = "Pacman [" + x + "," + y + "] -> [" + tox + "," + toy + "]";
        }
        else{
            message = "Ghost [" + x + "," + y + "] -> [" + tox + "," + toy + "]";
        }
        if (!message.equals(lastMessage)) {
            if (entity instanceof Pacman){
             this.turn++;
             turns = "TURN " + turn;
             this.log(turns);
            }
            this.log(message);
            lastMessage = message;
        }
    }
    /**
     * Method that print Key was picked up
     * @param x x coordinates
     * @param y y coordinates
     */
    public void logMessageKey(int x, int y) {
        String message;

        message = "Key [" + x + "," + y + "] [PICKED UP]";

        if (!message.equals(lastMessage)) {
            this.log(message);
            lastMessage = message;
        }
    }
    /**
     * Method that print Door was open
     * @param x x coordinates
     * @param y y coordinates
     */
    public void logMessageDoors(int x, int y){
        String message;

        message = "Door [" + x + "," + y + "] [OPEN]";

        if (!message.equals(lastMessage)) {
            this.log(message);
            lastMessage = message;
        }
    }
    /**
     * Method that print Pacman's lives changed
     * @param lives Pacman's lives
     */
    public void logMessageLives(int lives){
        String message;
        int prevLives = lives + 1;

        message = "Pacman lives [" + prevLives + "]" + " -> " + "[" + lives + "]";

        if (!message.equals(lastMessage)) {
            this.log(message);
            lastMessage = message;
        }
    }
    /**
     * Method for writing in file
     * @param message line to be written in file
     */
    public void log(String message) {
        try {
            fileWriter.write(message + "\n");
            fileWriter.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    /**
     * Method for closing file
     */
    public void close() {
        try {
            fileWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
