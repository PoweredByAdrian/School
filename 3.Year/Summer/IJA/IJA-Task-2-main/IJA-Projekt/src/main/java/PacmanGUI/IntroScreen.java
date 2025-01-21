package PacmanGUI;

/**
 * Authors: Matus Tabi, xtabim01
 */

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

public class IntroScreen extends JPanel {
    final private JPanel deck;
    final private CardLayout layout;
    private JButton mouse, keyboard;
    private boolean movement = false;

    /**
     * Introscreen panel.
     * @param layout Cardlayout
     * @param deck Jpanel with other components which change throughout the game
     */
    public IntroScreen(CardLayout layout, JPanel deck) {
        this.layout = layout;
        this.deck = deck;
        setBorder(new EmptyBorder(100, 100, 100, 100));
        setBackground(Color.BLACK);
        add(level1Button());
        add(level2Button());
        add(keyboardMovement());
        add(mouseChoice());
        add(exitButton());
    }

    /**
     * Button that stars first level.
     * @return Level 1 button
     */
    private JButton level1Button() {
        JButton level1 = new JButton("Play level 1");
        level1.setBackground(Color.BLACK);
        level1.setFont(new Font("Arial", Font.BOLD, 14));
        level1.setForeground(Color.WHITE);
        level1.setPreferredSize(new Dimension(200, 50));
        level1.addActionListener(e -> {
            level1.setFocusable(false);
            JPanel game = new GUI(layout, deck, movement, "/level1.txt");
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
        return level1;
    }

    /**
     * Button that starts second level.
     * @return Level 2 button
     */
    private JButton level2Button() {
        JButton level2 = new JButton("Play level 2");
        level2.setBackground(Color.BLACK);
        level2.setFont(new Font("Arial", Font.BOLD, 14));
        level2.setForeground(Color.WHITE);
        level2.setPreferredSize(new Dimension(200, 50));
        level2.addActionListener(e -> {
            level2.setFocusable(false);
            JPanel game = new GUI(layout, deck, movement, "/level2.txt");
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
        return level2;
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

    /**
     * Button that enables keyboard movement. Is set to true by default.
     * @return
     */
    private JButton keyboardMovement() {
        JButton keyBoardMovement = new JButton("Keyboard");
        keyBoardMovement.setBackground(Color.GREEN);
        keyBoardMovement.setFont(new Font("Arial", Font.BOLD, 14));
        keyBoardMovement.setForeground(Color.BLACK);
        keyBoardMovement.setPreferredSize(new Dimension(200, 50));
        keyBoardMovement.addActionListener(e -> {
            movement = false;
            keyBoardMovement.setBackground(Color.GREEN);
            keyBoardMovement.setForeground(Color.BLACK);
            mouse.setBackground(Color.BLACK);
            mouse.setForeground(Color.WHITE);
        });
        keyboard = keyBoardMovement;
        return keyBoardMovement;
    }

    /**
     * Button that enables mouse movement.
     * @return Mouse choice button
     */
    private JButton mouseChoice() {
        JButton mouseChoice = new JButton("Mouse");
        mouseChoice.setBackground(Color.BLACK);
        mouseChoice.setFont(new Font("Arial", Font.BOLD, 14));
        mouseChoice.setForeground(Color.WHITE);
        mouseChoice.setPreferredSize(new Dimension(200, 50));
        mouseChoice.addActionListener(e -> {
            movement = true;
            mouseChoice.setBackground(Color.GREEN);
            mouseChoice.setForeground(Color.BLACK);
            keyboard.setBackground(Color.BLACK);
            keyboard.setForeground(Color.WHITE);
        });
        mouse = mouseChoice;
        return mouseChoice;
    }
}
