package PacmanGUI;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import Game.ReplyMode;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.LinkedList;

public class ReplayGame extends JPanel implements ActionListener, KeyListener {
    final private CardLayout layout;
    final private JPanel deck;
    final private char[][] maze;
    final private int blockSize = 24;
    Timer t = new Timer(500, this);
    private ReplyMode mode;
    private ReplyMode.TurnInfo turnInfo;
    private Point pacmanPosition, doorPosition;
    private boolean setPacman, setGhosts, keyPickedUp, doorOpened;
    private int replayIterator = 0;
    private LinkedList<Point> ghostPositions = new LinkedList<>();
    private Image pacmanImageRight, pacmanImageLeft, pacmanImageUp, pacmanImageDown,
            key, doorImageClosed, doorImageOpened, ghostImage, heartImage, emptyHeartImage;
    private int pacmanHP = 3;
    private int turns;
    private boolean backwards;
    private boolean backwards2 = false;
    private boolean set = false;

    /**
     * Replay game mode.
     * @param layout Cardlayout
     * @param deck Jpanel with other components which change throughout the game
     */
    public ReplayGame(CardLayout layout, JPanel deck) {
        this.layout = layout;
        this.deck = deck;
        setBackground(Color.BLACK);
        addKeyListener(this);
        setFocusable(true);
        mode = new ReplyMode();
        turnInfo = mode.new TurnInfo();
        mode.parseLogsFromFile();
        maze = mode.getMaze();
        loadAllImages();
    }

    /**
     * Loading all images used in game.
     */
    private void loadAllImages() {
        pacmanImageRight = loadImage("/PacmanRight.png");
        pacmanImageLeft = loadImage("/PacmanLeft.png");
        pacmanImageDown = loadImage("/PacmanDown.png");
        pacmanImageUp = loadImage("/PacmanUp.png");
        key = loadImage("/Key.png");
        doorImageOpened = loadImage("/DoorOpened.png");
        doorImageClosed = loadImage("/DoorClosed.png");
        ghostImage = loadImage("/Ghost.png");
        heartImage = loadImage("/Heart.png");
        emptyHeartImage = loadImage("/EmptyHeart.png");
    }

    /**
     * Load one image from resources folder.
     * @param imageName Name of image
     * @return Image
     */
    private Image loadImage(String imageName) {
        Image temp = new ImageIcon(getClass().getResource(imageName)).getImage();
        Image scaleImage = temp.getScaledInstance(blockSize, blockSize, Image.SCALE_SMOOTH);
        return new ImageIcon(scaleImage).getImage();
    }

    /**
     * Draw walls on the map.
     * @param g2d Graphics component
     * @param x X coordinate on the map
     * @param y Y coordinate on the map
     * @param mazeX X index on 2d array
     * @param mazeY Y index on 2d array
     */
    private void drawWalls(Graphics2D g2d, int x, int y, int mazeX, int mazeY) {
        if (mazeY - 1 > 0 && maze[mazeX][mazeY - 1] != 'X') {
            g2d.drawLine(x, y, x, y + blockSize);
        }
        if (mazeX + 1 < maze[mazeY].length && maze[mazeX + 1][mazeY] != 'X') {
            g2d.drawLine(x, y + blockSize, x + blockSize, y + blockSize);
        }
        if (mazeY + 1 < maze.length && maze[mazeX][mazeY + 1] != 'X') {
            g2d.drawLine(x + blockSize, y, x + blockSize, y + blockSize);
        }
        if (mazeX - 1 > 0 && maze[mazeX - 1][mazeY] != 'X') {
            g2d.drawLine(x, y, x + blockSize, y);
        }
    }

    /**
     * Main drawing method
     * @param g the <code>Graphics</code> object to protect
     */
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;
        int mazeX = 0;
        for (int y = 0; y < maze.length * blockSize; y += blockSize) {
            int mazeY = 0;
            for (int x = 0; x < maze[mazeX].length * blockSize; x += blockSize) {
                g2d.setColor(new Color(40, 60, 255));
                switch (maze[mazeX][mazeY]) {
                    case 'X' -> drawWalls(g2d, x, y, mazeX, mazeY);
                    case 'G' -> {
                        if (!setGhosts) {
                            ghostPositions.add(new Point(x / blockSize, y / blockSize));
                        }
                        for (Point ghost : ghostPositions) {
                            g2d.drawImage(ghostImage, ghost.x * blockSize, ghost.y * blockSize, this);
                        }
                    }
                    case 'T' -> {
                        if (!doorOpened) {
                            g2d.drawImage(doorImageClosed, x, y, this);
                        }
                        else {
                            g2d.drawImage(doorImageOpened, x, y, this);
                        }
                    }
                    case 'K' -> {
                        if (!keyPickedUp) {
                            g2d.drawImage(key, x, y, this);
                        }
                    }
                    case 'S' -> {
                        if (!setPacman) {
                            pacmanPosition = new Point(x / blockSize, y / blockSize);
                            setPacman = true;
                        }
                        g2d.drawImage(pacmanImageRight, pacmanPosition.x * blockSize,
                                pacmanPosition.y * blockSize, this);
                    }
                }
                mazeY += 1;
            }
            mazeX += 1;
        }
        setGhosts = true;
    }

    /**
     * End of game in replay mode.
     */
    private void endGame() {
        if (pacmanPosition.equals(doorPosition) && doorOpened) {
            setFocusable(false);
            t.stop();
            layout.show(deck, "endGameWon");
        }
        else if (pacmanHP == 0) {
            setFocusable(false);
            t.stop();
            layout.show(deck, "endGameLost");
        }
    }

    /**
     * Performing the action.
     * @param e the event to be processed
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        if (!set) {
            if (!backwards2) {
                replayIterator = turns - 1;
                System.out.println("TU");
            }
            else {
                replayIterator = 0;
            }
            set = true;
        }
        System.out.println(replayIterator);
        turnInfo = mode.parseLogs(replayIterator);
        if (turnInfo.pacman != null) {
            pacmanPosition.setLocation(turnInfo.pacman.toX, turnInfo.pacman.toY);
        }
        if (turnInfo.ghosts != null) {
            for (int i = 0; i < turnInfo.ghosts.size(); i++) {
                ghostPositions.set(i, new Point(turnInfo.ghosts.get(i).toX, turnInfo.ghosts.get(i).toY));
            }
        }
        if (turnInfo.key != null) {
            keyPickedUp = true;
        }
        if (turnInfo.door != null) {
            doorPosition = new Point(turnInfo.door.x, turnInfo.door.y);
            doorOpened = true;
        }
        if (turnInfo.PacmanHP != -1) {
            pacmanHP = turnInfo.PacmanHP;
        }
        if (!backwards) {
            if (replayIterator == 0) {
                setFocusable(false);
                t.stop();
                layout.show(deck, "endGameWon");
            }
            replayIterator -= 1;
        }
        else {
            replayIterator += 1;
        }
        endGame();
        repaint();
    }

    @Override
    public void keyTyped(KeyEvent e) {

    }

    /**
     * Starting the replay after ENTER key is pressed.
     * @param e the event to be processed
     */
    @Override
    public void keyPressed(KeyEvent e) {
        int key = e.getKeyCode();
        if (key == KeyEvent.VK_ENTER) {
            t.start();
        }
        if (key == KeyEvent.VK_B) {
            turns = mode.getTurns();
            backwards = false;
        }
        if (key == KeyEvent.VK_F) {
            backwards2 = true;
            backwards = true;
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {

    }
}