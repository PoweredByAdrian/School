/**
* @brief
*
* @author Adrian Horvath(xhorva14)
*
*/
#pragma once
#include <iostream>
#include <list>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include "mainwindow.h"
#include "Action.h"

class mainwindow;

class ReplayMode {
private:
	mainwindow* window;
	int turns;
	std::vector<std::list<Action>> turnList;
	std::list<std::string> Maze;
	int rows;
	int cols;
	std::vector<std::list<std::string>> parsedTurns;

	std::regex logRegex;
	std::regex turnRegex;

	void setMazeSize(std::string line);
	void setMazeFromLog(std::string line);
	void parseLogsFromFile(std::string filePath);
	void replayAction(int turn, bool reverse);
	void resolveAction(Action action, bool reverse);

public:

	ReplayMode(std::string filePath, mainwindow* window, bool fromStart = true);
	~ReplayMode();
	std::list<std::string> getMaze();
	bool parseLogsFromTurn();
	bool nextTurn();
	bool prevTurn();
};

