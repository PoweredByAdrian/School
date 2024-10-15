/**
 * @brief Game interface header file
 *
 * @author Matúš Vráblik (xvrabl05)
 */
#pragma once
#include "MazeConfigure.h"
#include "mainwindow.h"
#include "Logger.h"
#include <tuple>
class MazeConfigure;
class Maze;
class mainwindow;
class Logger;

using namespace std;
class GameInterface
{
protected:
	MazeConfigure* config;
	list<tuple<int, int>>* ghosts;
	list<tuple<int, int>>* walls;
	list<tuple<int, int>>* keys;
	tuple<int, int> door;
	bool doorOpen = false;
	tuple<int, int> player;
	int lives;
	int maxLives;
	int maxKeys;
	Maze* maze;

public:

	GameInterface(mainwindow* window);
	~GameInterface();
	void loadMap();
	void loadMap(std::string file);
	void loadReplay(list<string> maze);
	void startGame();
	void endGame();
	int getLives();
	int getMaxLives();
	int getMaxKeys();
	tuple<int, int> getMapSize();
	list<tuple<int, int>> getWalls();
	list<tuple<int, int>> getGhosts();
	list<tuple<int, int>> getKeys();
	tuple<int, int> getPlayer();
	tuple<int, int> getDoor();
	bool isDoorOpen();
	void movePlayer(int d);

	void notifyMove(int fromX, int fromY, int toX, int toY);
	void notifyPickKey(int x, int y);
	void notifyOpenDoors(int x, int y);
	void notifyLives();
	void notifyEndLevel();
	void notifyGameOver();
private:
	Logger* logger;
	mainwindow* window;
	void updateVariables();
};
