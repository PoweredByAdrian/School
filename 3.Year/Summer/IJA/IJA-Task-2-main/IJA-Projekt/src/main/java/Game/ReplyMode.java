package Game;

/**
 * Authors: Adrian Horvath, xhorva14
 */

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ReplyMode {

    private char[][] Maze;
    private int rows;
    private int cols;
    private List<List<String>> parsedLogs;
    private static final String LOG_REGEX = "(Pacman|Ghost) \\[(\\d+),(\\d+)\\]( -> \\[(\\d+),(\\d+)\\])?" +
            "|Door \\[(\\d+),(\\d+)\\] \\[OPEN\\]" +
            "|Key \\[(\\d+),(\\d+)\\] \\[PICKED UP\\]" +
            "|Pacman lives \\[(\\d+)\\] -> \\[(\\d+)\\]";
    private static final String TURN_REGEX = "TURN (\\d+)";
    /**
     * Method that Parse all log file and store logs into turn arrays and set up maze size
     */
    public void parseLogsFromFile() {
        String filePath = "Log.log";
        List<List<String>> turns = new ArrayList<>();
        List<String> currentTurn = null;

        try  {
            FileReader fileReader = new FileReader(filePath);
            BufferedReader reader = new BufferedReader(fileReader);
            String line;

            line = reader.readLine();
            String[] tokens = line.split(" ");
            this.rows = Integer.parseInt(tokens[0]);
            this.cols = Integer.parseInt(tokens[1]);
            this.Maze = new char[this.rows][this.cols];
            for (int x = 0; x < this.rows; x++){
                line = reader.readLine();
                this.Maze[x] = line.toCharArray();
            }


            while ((line = reader.readLine()) != null) {
                line = line.trim();

                // Check for TURN line
                Matcher turnMatcher = Pattern.compile(TURN_REGEX).matcher(line);
                if (turnMatcher.matches()) {
                    // Start a new list for the turn
                    currentTurn = new ArrayList<>();
                    turns.add(currentTurn);
                    currentTurn.add(line);
                    continue;
                }

                // Check for log line
                Matcher logMatcher = Pattern.compile(LOG_REGEX).matcher(line);
                if (logMatcher.matches() && currentTurn != null) {
                    currentTurn.add(line);
                }
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        this.parsedLogs = turns;
    }

    public int getRows() {
        return rows;
    }
    public int getCols() {
        return cols;
    }
    public char[][] getMaze() {
        return Maze;
    }

    /**
     * Method that parse all logs in one turn and set up TurnInfo claass
     * @param i turn number
     */
    public TurnInfo parseLogs(int i) {
        List<String> turnLogs = parsedLogs.get(i);
        TurnInfo turnInfo = new TurnInfo();
        for (String logLine : turnLogs) {
            Matcher matcher = Pattern.compile(LOG_REGEX).matcher(logLine);

            if (matcher.matches()) {
                if (matcher.group(1) != null) {
                    // Pacman or Ghost log
                    String objectType = matcher.group(1);
                    int x1 = Integer.parseInt(matcher.group(2));
                    int y1 = Integer.parseInt(matcher.group(3));
                    int x2 = matcher.group(4) != null ? extractXCoordinate(matcher.group(4)) : -1;
                    int y2 = matcher.group(4) != null ? extractYCoordinate(matcher.group(4)) : -1;

                    // Perform actions based on the log content
                    if (objectType.equals("Pacman")) {
                        turnInfo.addPacman(x1, y1, x2, y2);
                    } else if (objectType.equals("Ghost")) {
                        turnInfo.addGhost(x1, y1, x2, y2);
                    }
                } else if (matcher.group(7) != null) {
                    // Door log
                    int x = Integer.parseInt(matcher.group(7));
                    int y = Integer.parseInt(matcher.group(8));

                    turnInfo.addDoor(x, y);
                } else if (matcher.group(9) != null) {
                    // Key log
                    int x = Integer.parseInt(matcher.group(9));
                    int y = Integer.parseInt(matcher.group(10));

                    turnInfo.addKey(x, y);
                } else if (matcher.group(11) != null) {
                    // Pacman HP log
                    int hp1 = Integer.parseInt(matcher.group(11));
                    int hp2 = Integer.parseInt(matcher.group(12));

                    turnInfo.PacmanHP = hp2;
                }
            } else {
                // Invalid log format
                //System.out.println("Invalid log: " + logLine);
            }
        }
        return turnInfo;
    }

    public int getTurns() {
        return parsedLogs.size();
    }

    private int extractXCoordinate(String coordinateGroup) {
        Matcher matcher = Pattern.compile("\\[(\\d+),(\\d+)\\]").matcher(coordinateGroup);
        if (matcher.find()) {
            return Integer.parseInt(matcher.group(1));
        }
        return -1;
    }

    private int extractYCoordinate(String coordinateGroup) {
        Matcher matcher = Pattern.compile("\\[(\\d+),(\\d+)\\]").matcher(coordinateGroup);
        if (matcher.find()) {
            return Integer.parseInt(matcher.group(2));
        }
        return -1;
    }

    /**
     * Class that holds all information about changes in last turn
     */
    public class TurnInfo {
        public Pacman pacman = null;
        public List<Ghost> ghosts = new ArrayList<>();
        public Door door = null;
        public Key key = null;
        public int PacmanHP = -1;
        public class Pacman{
            public int fromX;
            public int fromY;
            public int toX;
            public int toY;
        }
        public class Ghost{
            public int fromX;
            public int fromY;
            public int toX;
            public int toY;
        }
        public class Door{
            public int x;
            public int y;
        }
        public class Key{
            public int x;
            public int y;
        }

        public void addPacman(int fromX, int fromY, int toX, int toY){
            this.pacman = new Pacman();
            this.pacman.fromX = fromX;
            this.pacman.fromY = fromY;
            this.pacman.toX = toX;
            this.pacman.toY = toY;
        }
        public void addGhost(int fromX, int fromY, int toX, int toY){
            Ghost ghost = new Ghost();

            ghost.fromX = fromX;
            ghost.fromY = fromY;
            ghost.toX = toX;
            ghost.toY = toY;

            ghosts.add(ghost);
        }

        public void addDoor(int x, int y){
            this.door = new Door();
            this.door.x = x;
            this.door.y = y;
        }

        public void addKey(int x, int y){
            this.key = new Key();
            key.x = x;
            key.y = y;

        }
    }
}
