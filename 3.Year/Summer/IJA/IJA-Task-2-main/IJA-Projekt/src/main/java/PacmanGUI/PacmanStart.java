package PacmanGUI;

/**
 * Authors: Matus Tabi, xtabim01
 */

import javax.swing.*;
import java.awt.*;

public class PacmanStart extends JFrame {
    final private JPanel deck;
    final private CardLayout layout;

    /**
     * Method that creates panel and CardLayout.
     * While using the application, the panels change according to the situation.
     */
    public PacmanStart() {
        deck  = new JPanel();
        layout = new CardLayout();
        deck.setLayout(layout);
        deck.add("introScreen", new IntroScreen(layout, deck));
        //deck.add("endGameWon", new EndGame(layout, deck, "Game over, you won!", Color.GREEN));
        //deck.add("endGameLost", new EndGame(layout, deck, "You just lost the game.", Color.RED));
        layout.show(deck, "introScreen");
        add(deck);
    }

    public static void main(String[] args) {
        PacmanStart pacman = new PacmanStart();
        pacman.setVisible(true);
        pacman.setTitle("Pacman");
        pacman.setPreferredSize(new Dimension(420, 480));
        pacman.setDefaultCloseOperation(EXIT_ON_CLOSE);
        pacman.setResizable(false);
        pacman.pack();
        pacman.setLocationRelativeTo(null);
    }
}
