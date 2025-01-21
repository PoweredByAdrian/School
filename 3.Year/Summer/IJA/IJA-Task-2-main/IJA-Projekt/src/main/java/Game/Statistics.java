package Game;

/**
 * Authors: Matus Tabi, xtabim01
 */

public class Statistics {
    private int pointsCollected = 0;
    private int pacmanLives = 3;
    private int distanceTravelled = 0;
    public void pointCollected() {
        pointsCollected += 1;
    }
    public String getPointsCollected() {
        return String.valueOf(pointsCollected);
    }

    public void lives(int pacmanLives) {
        this.pacmanLives = pacmanLives;
    }

    public String getPacmanLives() {
        return String.valueOf(pacmanLives);
    }

    public void setDistanceTravelled() {
        distanceTravelled += 1;
    }

    public int getDistanceTravelled() {
        return distanceTravelled;
    }
}
