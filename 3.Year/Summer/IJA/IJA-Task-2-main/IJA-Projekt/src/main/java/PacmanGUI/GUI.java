package PacmanGUI;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.Rectangle2D;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;
import java.util.LinkedList;

import Game.Entities.Ghost;
import Game.Entities.Pacman;
import Game.Fields.PathField;
import Game.Fields.WallField;
import Game.Logger;
import Game.MazeBoard.MazeConfigure;
import Game.MazeObjects.Door;
import Game.MazeObjects.Key;
import Game.Statistics;
import Game.Triplet;
import Interfaces.Field;
import Interfaces.Maze;
import Interfaces.MazeEntity;

public class GUI extends JPanel implements ActionListener, KeyListener, MouseListener {
    final private CardLayout layout;
    final private JPanel deck;
    final private Statistics statistics = new Statistics();
    private MazeEntity pacman;
    final private List<MazeEntity> ghostsList = new LinkedList<>();
    final private int blockSize = 24;
    final private Point blocksN = new Point();
    private Rectangle2D keyRec, doorRec, pacmanRec;
    private int score = 0;
    Timer t = new Timer(500, this);
    private int moveX = 0;
    private int moveY = 0;
    private int mouseIterator;
    private Logger logger;
    private Image pacmanImageRight, pacmanImageLeft, pacmanImageUp, pacmanImageDown,
            key, doorImageClosed, doorImageOpened, ghostImage, heartImage, emptyHeartImage;
    private boolean pacmanPositionSet = true;
    private boolean ghostCantMove, ghostPositionSet, movement, countDistance;
    private Maze maze;
    private Point whereToMove = new Point(0, 0);
    private Field.Direction pacmanDirection = Field.Direction.R;
    private List<Triplet> path;
    String levelName;

    /**
     * Class which implements the game itself using Jpanel and paintComponent.
     * @param layout CardLayout.
     * @param deck Jpanel with other components which change throughout the game.
     * @param movement True - mouse movement, False - keyboard movement.
     * @param levelName Name of level.
     */
    public GUI(CardLayout layout, JPanel deck, boolean movement, String levelName) {
        this.layout = layout;
        this.deck = deck;
        this.movement = movement;
        this.levelName = levelName;
        addKeyListener(this);
        addMouseListener(this);
        ToolTipManager.sharedInstance().registerComponent(this);
        ToolTipManager.sharedInstance().setDismissDelay(Integer.MAX_VALUE);
        try {
            initMaze(levelName);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        setFocusable(true);
        setBackground(Color.BLACK);
        loadAllImages();
        UIManager.put("ToolTip.background", Color.BLACK);
        UIManager.put("ToolTip.foreground", Color.ORANGE);
        UIManager.put("ToolTip.font", new Font("Arial", Font.BOLD, 14));
    }

    /**
     * Initialization of Pacman maze.
     * @param levelName Name of level.
     * @throws IOException
     */
    private void initMaze(String levelName) throws IOException {
        MazeConfigure mazeCfg = new MazeConfigure();
        logger = mazeCfg.logger;
        BufferedReader level = new BufferedReader(new InputStreamReader(
                this.getClass().getResourceAsStream(levelName)));
        try {
            String line = level.readLine();
            String[] firstLine = line.split(" ");
            blocksN.x = Integer.parseInt(firstLine[0]);
            blocksN.y = Integer.parseInt(firstLine[1]);
            mazeCfg.startReading(blocksN.x, blocksN.y );
            line = level.readLine();
            while (line != null) {
                mazeCfg.processLine(line);
                line = level.readLine();
            }
            level.close();
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        mazeCfg.stopReading();
        maze = mazeCfg.createMaze();
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
     * Auxiliary function, that loads one image from resources folder.
     * @param imageName Name of image.
     * @return Image
     */
    private Image loadImage(String imageName) {
        Image temp = new ImageIcon(getClass().getResource(imageName)).getImage();
        Image scaleImage = temp.getScaledInstance(blockSize, blockSize, Image.SCALE_SMOOTH);
        return new ImageIcon(scaleImage).getImage();
    }

    /**
     * Drawing walls on the map.
     * @param g2d Graphics component
     * @param x X coordinate in the window
     * @param y Y coordinate in the window
     * @param mazeX X index in 2d array
     * @param mazeY Y index in 2d array
     */
    private void drawWalls(Graphics2D g2d, int x, int y, int mazeX, int mazeY) {
        if (maze.getField(mazeX, mazeY).nextField(Field.Direction.U) instanceof PathField) {
            g2d.drawLine(x, y, x + blockSize, y);
        }
        if (maze.getField(mazeX, mazeY).nextField(Field.Direction.R) instanceof PathField) {
            g2d.drawLine(x + blockSize, y, x + blockSize, y + blockSize);
        }
        if (maze.getField(mazeX, mazeY).nextField(Field.Direction.D) instanceof PathField) {
            g2d.drawLine(x, y + blockSize, x + blockSize, y + blockSize);
        }
        if (maze.getField(mazeX, mazeY).nextField(Field.Direction.L) instanceof PathField) {
            g2d.drawLine(x, y, x, y + blockSize);
        }
    }

    /**
     * Drawing path field on the map.
     * @param g2d Graphics component
     * @param x X coordinate in the window
     * @param y Y coordinate in the window
     * @param xMaze X index in 2d array
     * @param yMaze Y index in 2d Array
     */
    private void drawPaths(Graphics2D g2d, int x, int y, int xMaze, int yMaze) {
        if (maze.getField(xMaze, yMaze).containPoint()) {
            g2d.setColor(new Color(255, 255, 224));
            g2d.fillOval(x + 10, y + 10, blockSize / 6, blockSize / 6);
        }
    }

    /**
     * Drawing pacman on the map. The position of pacman changes
     * if it moves.
     * @param g2d Graphics component
     * @param xMaze X index in 2d array
     * @param yMaze Y index in 2d array
     */
    private void drawPacman(Graphics2D g2d, int xMaze, int yMaze) {
        if (pacmanPositionSet) {
            pacman = maze.getField(xMaze, yMaze).getPacmanEntity();
            pacmanPositionSet = false;
        }
        Point pos = pacman.getPosition();
        if (pacman.getField().containPoint()) {
            score += 10;
            statistics.pointCollected();
            pacman.getField().pointTaken();
        }
        if (maze.getField(xMaze, yMaze).getObject() instanceof Key) {
            maze.getField(xMaze, yMaze).getObject().pickUpKey();
        }
        pacmanRec = new Rectangle2D.Double(pos.x * blockSize, pos.y * blockSize, blockSize, blockSize);
        drawPacmanDirection(g2d, pos);
        drawPacmanLives(g2d);
    }

    /**
     * Drawing pacman lives.
     * @param g2d Graphics component
     */
    private void drawPacmanLives(Graphics2D g2d) {
        switch (pacman.getLives()) {
            case 3 -> {
                g2d.drawImage(heartImage, 120 , 0, this);
                g2d.drawImage(heartImage, 144 , 0, this);
                g2d.drawImage(heartImage, 168 , 0, this);
            }
            case 2 -> {
                g2d.drawImage(heartImage, 120 , 0, this);
                g2d.drawImage(heartImage, 144 , 0, this);
                g2d.drawImage(emptyHeartImage, 168 , 0, this);
            }
            case 1 -> {
                g2d.drawImage(heartImage, 120 , 0, this);
                g2d.drawImage(emptyHeartImage, 144 , 0, this);
                g2d.drawImage(emptyHeartImage, 168 , 0, this);
            }
        }
    }

    /**
     * Drawing direction the pacman is facing.
     * @param g2d Graphics component
     * @param pos Position of pacman
     */
    private void drawPacmanDirection(Graphics2D g2d, Point pos) {
        switch (pacmanDirection) {
            case R -> g2d.drawImage(pacmanImageRight, pos.x * blockSize, pos.y * blockSize, this);
            case L -> g2d.drawImage(pacmanImageLeft, pos.x * blockSize, pos.y * blockSize, this);
            case U -> g2d.drawImage(pacmanImageUp, pos.x * blockSize, pos.y * blockSize, this);
            case D -> g2d.drawImage(pacmanImageDown, pos.x * blockSize, pos.y * blockSize, this);
        }
    }

    /**
     * Drawing closed or opened door.
     * @param g2d Graphics component
     * @param x X coordinate in the window
     * @param y Y coordinate in the window
     * @param xMaze X index in 2d array
     * @param yMaze Y index in 2d array
     */
    private void drawDoor(Graphics2D g2d, int x, int y, int xMaze, int yMaze) {
        if (keyRec != null && !maze.getField((int) keyRec.getX() / blockSize, (int) keyRec.getY() / blockSize).getObject().keyTaken()) {
            maze.getField(xMaze, yMaze).getObject().openDoor();
            g2d.drawImage(doorImageOpened, x, y, this);
        }
        else {
            g2d.drawImage(doorImageClosed, x, y, this);
        }
    }

    /**
     * Drawing key until it is picked up.
     * @param g2d Graphics component
     * @param x X coordinate in the window
     * @param y Y coordinate in the window
     * @param xMaze X index in 2d array
     * @param yMaze Y index in 2d array
     */
    private void drawKey(Graphics2D g2d, int x, int y, int xMaze, int yMaze) {
        if (maze.getField(xMaze, yMaze).getObject().keyTaken()) {
            g2d.setColor(new Color(192, 192, 192));
            g2d.drawImage(key, x, y, this);
        }
    }

    /**
     * Drawing ghosts on the map. The position of all the ghosts
     * is changing if they move.
     * @param g2d Graphics component
     * @param xMaze X index in 2d array
     * @param yMaze Y index in 2d array
     */
    private void drawGhost(Graphics2D g2d, int xMaze, int yMaze) {
        if (!ghostCantMove) {
            if (!ghostPositionSet) {
                MazeEntity ghost = maze.getField(xMaze, yMaze).getGhostEntity();
                Point ghostPosition = ghost.getPosition();
                g2d.drawImage(ghostImage, ghostPosition.x * blockSize, ghostPosition.y * blockSize, this);
                ghostsList.add(ghost);
                return;
            }
            for (int i = 0; i < ghostsList.size(); i++) {
                Point ghostPosition = ghostsList.get(i).getPosition();
                whereToMove = ghostsList.get(i).Movement(whereToMove);
                g2d.drawImage(ghostImage, (ghostPosition.x + whereToMove.x) * blockSize,
                        (ghostPosition.y + whereToMove.y) * blockSize, this);
                maze.getField(ghostPosition.x, ghostPosition.y).removeGhost();
                ghostsList.set(i, maze.getField(ghostPosition.x + whereToMove.x, ghostPosition.y + whereToMove.y).getGhostEntity());
            }
            ghostCantMove = true;
        }
    }

    /**
     * Main drawing method, which draws whole map in the window.
     * @param g the <code>Graphics</code> object to protect
     */
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;
        g2d.setColor(new Color(0, 255, 0));
        g2d.setFont(new Font("Arial", Font.BOLD, 12));
        g2d.drawString("Score: " + score, 2, 12);
        int yMaze = 0;
        Point screenSize = new Point(blocksN.x * blockSize, blocksN.y * blockSize);
        for (int y = 0; y < screenSize.x + 2 * blockSize; y += blockSize) {
            int xMaze = 0;
            for (int x = 0; x < screenSize.y + 2 * blockSize; x += blockSize) {
                g2d.setColor(new Color(40, 60, 255));
                if (maze.getField(xMaze, yMaze) instanceof WallField) {
                    drawWalls(g2d, x, y, xMaze, yMaze);
                }
                if (maze.getField(xMaze, yMaze) instanceof PathField) {
                    drawPaths(g2d, x, y, xMaze, yMaze);
                }
                if (maze.getField(xMaze, yMaze).getPacmanEntity() instanceof Pacman) {
                    drawPacman(g2d, xMaze, yMaze);
                }
                if (maze.getField(xMaze, yMaze).getObject() instanceof Key) {
                    keyRec = new Rectangle2D.Double(x, y, blockSize, blockSize);
                    drawKey(g2d, x, y, xMaze, yMaze);
                }
                if (maze.getField(xMaze, yMaze).getObject() instanceof Door) {
                    doorRec = new Rectangle2D.Double(x, y, blockSize, blockSize);
                    drawDoor(g2d, x, y, xMaze, yMaze);
                }
                if (maze.getField(xMaze, yMaze).getGhostEntity() instanceof Ghost) {
                    drawGhost(g2d, xMaze, yMaze);
                }
                xMaze += 1;
            }
            yMaze += 1;
        }
        ghostPositionSet = true;
    }

    /**
     * Tooltip for objects and entities on the map.
     * @param event the {@code MouseEvent} that initiated the
     *              {@code ToolTip} display
     * @return tooltip
     */
    @Override
    public String getToolTipText(MouseEvent event) {
        Point p = new Point(event.getX(), event.getY());
        String t = tooltip(p);
        if (t != null) {
            return t;
        }
        return super.getToolTipText(event);
    }

    /**
     * Auxiliary method which returns tooltip for specific component on the map.
     * @param p Point, where mouse points.
     * @return tooltip for object or entity
     * @throws NullPointerException
     */
    protected String tooltip(Point p) throws NullPointerException {
        if (pacmanRec.contains(p)) {
            return "<html>" +
                    "Name: Pacman<br>" +
                    "Age: 8<br>" +
                    "Date of birth: Just a few seconds ago<br>" +
                    "Nationality: Slovak<br>" +
                    "Languages: Unknown<br>" +
                    "Sex: Other<br>" +
                    "Hobbies: Jogging, eating, playing games<br>" +
                    "</html>";
        }
        if (keyRec.contains(p)) {
            return "It's just a key";
        }
        if (doorRec.contains(p)) {
            return "A door, obviously";
        }
        return null;
    }

    /**
     * Control method, which checks whether Pacman met ghost.
     */
    private void pacmanMeetsGhost() {
        if (maze.getField(pacman.getPosition().x, pacman.getPosition().y).hasGhost()) {
            if (!pacman.decreaseLives()) {
                logger.close();
                t.stop();
                deck.add("endGameLost", new EndGame(layout, deck, "You just lost the game.", Color.RED, levelName, movement));
                layout.show(deck, "endGameLost");
            }
            statistics.lives(pacman.getLives());
        }
    }

    /**
     * End of successful game.
     */
    private void endGame() {
        if (maze.getField(pacman.getPosition().x, pacman.getPosition().y).doorIsOpen()) {
            logger.close();
            t.stop();
            deck.add("endGameWon", new EndGame(layout, deck, "Game over, you won!", Color.GREEN, levelName, movement));
            layout.show(deck, "endGameWon");
        }
    }

    /**
     * Mouse movement.
     * @param e the event to be processed
     */
    @Override
    public void mouseClicked(MouseEvent e) {
        if (movement) {
            path = pacman.findPath(maze.getField(pacman.getPosition().x, pacman.getPosition().y),
                    maze.getField(e.getX() / blockSize, e.getY() / blockSize));
            if (!maze.getField(e.getX() / blockSize, e.getY() / blockSize).canMove() ||
                path.size() == 0) {
                t.start();
                return;
            }
            t.start();
            mouseIterator = path.size();
        }
    }

    @Override
    public void mousePressed(MouseEvent e) {}
    @Override
    public void mouseReleased(MouseEvent e) {}
    @Override
    public void mouseEntered(MouseEvent e) {}
    @Override
    public void mouseExited(MouseEvent e) {}

    /**
     * @param e the event to be processed
     */
    public void actionPerformed(ActionEvent e) {
        repaint();
        if (!movement) {
            if (!countDistance) {
                statistics.setDistanceTravelled();
                countDistance = true;
            }
            pacman = maze.getField(pacman.getPosition().x + moveX, pacman.getPosition().y + moveY).getPacmanEntity();
        }
        else {
            if (!countDistance) {
                statistics.setDistanceTravelled();
                countDistance = true;
            }
            movePacmanMouse();
        }
        ghostCantMove = false;
        pacmanMeetsGhost();
        endGame();
    }

    /**
     * Pacman movement using mouse.
     */
    private void movePacmanMouse() {
        if (path.size() != 0) {
            if (mouseIterator != 0) {
                mouseIterator -= 1;
            }
            else {
                mouseIterator = 0;
            }
            Triplet pacmanMouseMove = path.get(mouseIterator);
            pacman.move(pacmanMouseMove.getDirection(), pacmanMouseMove.getCurrentField().getFieldPosition().x,
                    pacmanMouseMove.getCurrentField().getFieldPosition().y, null);
            pacman = maze.getField(path.get(mouseIterator).getCurrentField().getFieldPosition().x,
                    path.get(mouseIterator).getCurrentField().getFieldPosition().y).getPacmanEntity();
            pacmanDirection = pacmanMouseMove.getDirection();
            countDistance = false;
        }
    }

    /**
     * Movement with key pressing.
     * @param e the event to be processed
     */
    public void keyPressed(KeyEvent e) {
        if (!movement) {
            t.start();
            int key = e.getKeyCode();
            if (key == KeyEvent.VK_RIGHT || key == KeyEvent.VK_D) {
                if (pacman.canMove(Field.Direction.R)) {
                    moveX = 1;
                    moveY = 0;
                    pacman.move(Field.Direction.R, pacman.getPosition().x + moveX, pacman.getPosition().y + moveY, null);
                    pacmanDirection = Field.Direction.R;
                    countDistance = false;
                } else {
                    moveX = moveY = 0;
                }
            } else if (key == KeyEvent.VK_LEFT || key == KeyEvent.VK_A) {
                if (pacman.canMove(Field.Direction.L)) {
                    moveX = -1;
                    moveY = 0;
                    pacman.move(Field.Direction.L, pacman.getPosition().x + moveX, pacman.getPosition().y + moveY, null);
                    pacmanDirection = Field.Direction.L;
                    countDistance = false;
                } else {
                    moveX = moveY = 0;
                }
            } else if (key == KeyEvent.VK_DOWN || key == KeyEvent.VK_S) {
                if (pacman.canMove(Field.Direction.D)) {
                    moveX = 0;
                    moveY = 1;
                    pacman.move(Field.Direction.D, pacman.getPosition().x + moveX, pacman.getPosition().y + moveY, null);
                    pacmanDirection = Field.Direction.D;
                    countDistance = false;
                } else {
                    moveX = moveY = 0;
                }
            } else if (key == KeyEvent.VK_UP || key == KeyEvent.VK_W) {
                if (pacman.canMove(Field.Direction.U)) {
                    moveX = 0;
                    moveY = -1;
                    pacman.move(Field.Direction.U, pacman.getPosition().x + moveX, pacman.getPosition().y + moveY, null);
                    pacmanDirection = Field.Direction.U;
                    countDistance = false;
                } else {
                    moveX = moveY = 0;
                }
            }
        }
        if (e.getKeyCode() == KeyEvent.VK_P) {
            deck.add("pause", new PauseGame(layout, deck, statistics));
            layout.show(deck, "pause");
        }
    }
    public void keyTyped(KeyEvent e) {}

    /**
     * Releasing key.
     * @param e the event to be processed
     */
    public void keyReleased(KeyEvent e) {
        int key = e.getKeyCode();
        if (key == KeyEvent.VK_RIGHT || key == KeyEvent.VK_LEFT ||
            key == KeyEvent.VK_DOWN || key == KeyEvent.VK_UP) {
            moveX = 0;
            moveY = 0;
        }
    }
}
