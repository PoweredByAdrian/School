package PacmanGUI;

/**
 * Authors: Matus Tabi, xtabim01
 */

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

public class EndGame extends JPanel {
    final private CardLayout layout;
    final private JPanel deck;
    final private String message;
    final private Color color;
    final private String levelname;
    final private boolean movement;

    /**
     * End game panel with buttons.
     * @param layout Cardlayout
     * @param deck Jpanel with other components which change throughout the game.
     * @param message Message to be shown on the screen
     * @param color Color of message
     * @param levelName Name of level
     * @param movement Movement choice
     */
    public EndGame(CardLayout layout, JPanel deck, String message, Color color, String levelName, boolean movement) {
        this.layout = layout;
        this.deck = deck;
        this.message = message;
        this.color = color;
        this.levelname = levelName;
        this.movement = movement;
        setBorder(new EmptyBorder(100, 100, 100, 100));
        setBackground(Color.BLACK);
        add(mainMenuButton());
        add(tryAgainButton());
        add(replayButton());
        add(exitButton());
    }
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;
        g2d.setColor(color);
        g2d.setFont(new Font("Arial", Font.BOLD, 20));
        g2d.drawString(message, 100, 50);
    }

    /**
     * Button that redirects user to main menu.
     * @return Main menu button
     */
    private JButton mainMenuButton() {
        JButton mainMenu = new JButton("Main menu");
        mainMenu.setBackground(Color.BLACK);
        mainMenu.setFont(new Font("Arial", Font.BOLD, 14));
        mainMenu.setForeground(Color.WHITE);
        mainMenu.setPreferredSize(new Dimension(200, 50));
        mainMenu.addActionListener(e -> {
            mainMenu.setFocusable(false);
            layout.show(deck, "introScreen");
        });
        return mainMenu;
    }

    /**
     * Button which restarts the game.
     * @return Try again button
     */
    private JButton tryAgainButton() {
        JButton tryAgain = new JButton("Try again");
        tryAgain.setBackground(Color.BLACK);
        tryAgain.setFont(new Font("Arial", Font.BOLD, 14));
        tryAgain.setForeground(Color.WHITE);
        tryAgain.setPreferredSize(new Dimension(200, 50));
        tryAgain.addActionListener(e -> {
            tryAgain.setFocusable(false);
            JPanel game = new GUI(layout, deck, movement, levelname);
            game.setFocusable(true);
            game.addComponentListener(new ComponentAdapter() {
                @Override
                public void componentShown(ComponentEvent e) {
                    game.requestFocusInWindow();
                }
            });
            deck.add("game", game);
            layout.last(deck);
        });
        return tryAgain;
    }

    /**
     * Button that starts replaying the last played game.
     * @return Replay button
     */
    private JButton replayButton() {
        JButton replayGame = new JButton("Replay last game");
        replayGame.setBackground(Color.BLACK);
        replayGame.setFont(new Font("Arial", Font.BOLD, 14));
        replayGame.setForeground(Color.WHITE);
        replayGame.setPreferredSize(new Dimension(200, 50));
        replayGame.addActionListener(e -> {
            replayGame.setFocusable(false);
            JPanel replay = new ReplayGame(layout, deck);
            replay.setFocusable(true);
            replay.addComponentListener(new ComponentAdapter() {
                @Override
                public void componentShown(ComponentEvent e) {
                    replay.requestFocusInWindow();
                }
            });
            deck.add("replayMode", new ReplayGame(layout, deck));
            layout.show(deck, "replayMode");
        });
        return replayGame;
    }

    /**
     * Button that exits the game.
     * @return Exit button
     */
    private JButton exitButton() {
        JButton exit = new JButton("Exit game");
        exit.setBackground(Color.BLACK);
        exit.setFont(new Font("Arial", Font.BOLD, 14));
        exit.setForeground(Color.WHITE);
        exit.setPreferredSize(new Dimension(200, 50));
        exit.addActionListener( e -> System.exit(0));
        return exit;
    }
}
