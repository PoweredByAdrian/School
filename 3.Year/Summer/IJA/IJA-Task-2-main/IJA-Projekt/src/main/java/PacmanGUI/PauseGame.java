package PacmanGUI;

/**
 * Authors: Matus Tabi, xtabim01
 *          Adrian Horvath, xhorva14
 */

import Game.Statistics;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;

public class PauseGame extends JPanel {
    final private CardLayout layout;
    final private JPanel deck;
    private Statistics statistics;
    private boolean showStatistics = false;

    /**
     * Pause menu.
     * @param layout CardLayout
     * @param deck Jpanel with other components which change throughout the game
     * @param statistics Statistics
     */
    public PauseGame(CardLayout layout, JPanel deck, Statistics statistics) {
        this.layout = layout;
        this.deck = deck;
        this.statistics = statistics;
        setBorder(new EmptyBorder(100, 100, 100, 100));
        setBackground(Color.BLACK);
        add(resumeButton());
        add(statisticsButton());
    }

    /**
     * Button that will resume played game.
     * @return Resume button
     */
    private JButton resumeButton() {
        JButton resume = new JButton("Resume");
        resume.setBackground(Color.BLACK);
        resume.setFont(new Font("Arial", Font.BOLD, 14));
        resume.setForeground(Color.WHITE);
        resume.setPreferredSize(new Dimension(200, 50));
        resume.addActionListener(e -> {
            resume.setFocusable(false);
            layout.show(deck, "game");
        });
        return resume;
    }

    /**
     * Button that will show statistics of pacman.
     * @return Statistics button
     */
    private JButton statisticsButton() {
        JButton statisticsButton = new JButton("Statistics");
        statisticsButton.setBackground(Color.BLACK);
        statisticsButton.setFont(new Font("Arial", Font.BOLD, 14));
        statisticsButton.setForeground(Color.WHITE);
        statisticsButton.setPreferredSize(new Dimension(200, 50));
        statisticsButton.addActionListener(e -> {
            statisticsButton.setFocusable(false);
            showStatistics = true;
            repaint();
        });
        return statisticsButton;
    }
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (showStatistics) {
            Graphics2D g2d = (Graphics2D) g;
            g2d.setColor(Color.GREEN);
            g2d.setFont(new Font("Arial", Font.BOLD, 12));
            g2d.drawString("Points collected: " + statistics.getPointsCollected(), 2, 12);
            g2d.drawString("Pacman lives: " + statistics.getPacmanLives(), 2, 24);
            g2d.drawString("Distance travelled: " + statistics.getDistanceTravelled(), 2, 36);
        }
    }
}
