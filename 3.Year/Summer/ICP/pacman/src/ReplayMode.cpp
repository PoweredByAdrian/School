/**
* @brief	ReplayMode implementation
*
* @author	Adrian Horvath(xhorva14)
*
*/
#include "ReplayMode.h"


void ReplayMode::setMazeSize(std::string line)
{
	std::istringstream iss(line);
	iss >> this->rows >> this->cols;
}

void ReplayMode::setMazeFromLog(std::string line)
{
	this->Maze.push_back(line);
}

void ReplayMode::parseLogsFromFile(std::string filePath)
{
	std::ifstream inputFile(filePath);;
	if (inputFile.is_open())
	{
		std::string line;
		std::getline(inputFile, line);
		setMazeSize(line);
		std::getline(inputFile, line);
		do
		{
			setMazeFromLog(line);
			std::getline(inputFile, line);
		} while (!(std::regex_match(line, turnRegex)));

		std::list<std::string> logList;
		std::string prevLine = line;
		while (std::getline(inputFile, line))
		{
			if (std::regex_match(prevLine, turnRegex) && parsedTurns.empty() && std::regex_match(line, turnRegex))
			{
				parsedTurns.push_back(logList);
				logList.clear();
				continue;
			}
			if (std::regex_match(line, turnRegex))
			{
				parsedTurns.push_back(logList);
				logList.clear();
				continue;
			}
			logList.push_back(line);
		}
		parsedTurns.push_back(logList);
		inputFile.close();
	}
}

ReplayMode::ReplayMode(std::string filePath, mainwindow* window, bool fromStart) : logRegex("(Pacman|Ghost) \\[(\\d+),(\\d+)\\] -> \\[(\\d+),(\\d+)\\]"
	"|Doors \\[(\\d+),(\\d+)\\]\\[OPEN\\]"
	"|Key \\[(\\d+),(\\d+)\\]\\[PICKED UP\\]"
	"|Pacman lives \\[(\\d+)\\] -> \\[(\\d+)\\]"), turnRegex("TURN: (\\d+)")
{
	this->parseLogsFromFile(filePath);
	turns = 0;
	while (this->parseLogsFromTurn()) {}
	turns = -1;
	this->window = window;
}

ReplayMode::~ReplayMode()
{
	this->turnList.clear();
}

std::list<std::string> ReplayMode::getMaze()
{
	return this->Maze;
}

bool ReplayMode::parseLogsFromTurn()
{
	if (this->turns > parsedTurns.size() - 1 || this->turns < 0)
	{
		return false;
	}

	std::list<std::string> logsInTurn = parsedTurns[turns];

	std::list<Action> parsedTurnList;

	for (auto it = logsInTurn.begin(); it != logsInTurn.end(); ++it) {

		const std::string& log = *it;

		std::smatch match;
		if (std::regex_match(log, match, logRegex)) {
			if (match[1].matched) {

				// Pacman or Ghost log
				std::string objectType = match[1].str();
				int x1 = std::stoi(match[2].str());
				int y1 = std::stoi(match[3].str());
				int x2 = -1;
				int y2 = -1;

				if (match[4].matched && match[5].matched) {
					x2 = std::stoi(match[4].str());
					y2 = std::stoi(match[5].str());
				}

				// Perform actions based on the log content
				if (objectType == "Pacman") {
					//TODO move pacman
					parsedTurnList.push_back(Action(x1, y1, x2, y2, true));
					//this->window->replayStep(this->turns);
					//this->window->replayPacmanMove(x1, y1, x2, y2);
					// Use the extracted values
					std::cout << "Object Type: " << "Pacman" << std::endl;
					std::cout << "x1: " << x1 << std::endl;
					std::cout << "y1: " << y1 << std::endl;
					std::cout << "x2: " << x2 << std::endl;
					std::cout << "y2: " << y2 << std::endl;

				}
				else if (objectType == "Ghost") {
					//TODO move ghost
					parsedTurnList.push_back(Action(x1, y1, x2, y2, false));
					//this->window->replayGhostMove(x1, y1, x2, y2);
					// Use the extracted values
					std::cout << "Object Type: " << "Ghost" << std::endl;
					std::cout << "x1: " << x1 << std::endl;
					std::cout << "y1: " << y1 << std::endl;
					std::cout << "x2: " << x2 << std::endl;
					std::cout << "y2: " << y2 << std::endl;

				}
			}
			else if (match[6].matched) {
				// Door log
				int x = std::stoi(match[6].str());
				int y = std::stoi(match[7].str());

				//TODO doors
				parsedTurnList.push_back(Action(true));
				//this->window->replayDoor(true);
				std::cout << "Door x: " << x << std::endl;
				std::cout << "Door y: " << y << std::endl;
			}
			else if (match[9].matched) {
				// Key log
				int x = std::stoi(match[8].str());
				int y = std::stoi(match[9].str());

				//TODO key
				parsedTurnList.push_back(Action(x, y));
				//this->window->replayKey(x, y);
				std::cout << "Key x: " << x << std::endl;
				std::cout << "Key y: " << y << std::endl;
			}
			else if (match[11].matched) {
				// Pacman HP log
				int hp1 = std::stoi(match[10].str());
				int hp2 = std::stoi(match[11].str());

				//TODO pacman hp
				parsedTurnList.push_back(Action(hp1, hp2, true));
				//this->window->replayLive(hp1, hp2);
				std::cout << "Pacman HP 1: " << hp1 << std::endl;
				std::cout << "Pacman HP 2: " << hp2 << std::endl;
			}
		}
		else {
			// Invalid log format
			//TODO

			std::cout << "No match found." << std::endl;
		}
	}
	this->turnList.push_back(parsedTurnList);
	this->turns++;

	return true;
}

bool ReplayMode::nextTurn() {
	int turnListSize = this->turnList.size() - 1;
	int turns = this->turns;
	bool err = turns >= turnListSize;
	if (err)
	{
		return false;
	}
	this->turns++;
	this->window->replayStep(this->turns + 1);
	replayAction(this->turns, false);
	return true;
}

bool ReplayMode::prevTurn() {
	int turns = this->turns;
	if (turns < 0)
	{
		return false;
	}
	this->window->replayStep(this->turns);
	replayAction(this->turns, true);
	this->turns--;
	return true;
}

void ReplayMode::replayAction(int turn, bool reverse) {
	if (!reverse) {
		for (auto it = this->turnList[turn].begin(); it != this->turnList[turn].end(); ++it) {
			Action action = *it;
			resolveAction(action, reverse);
		}
	}
	else {
		for (auto it = this->turnList[turn].rbegin(); it != this->turnList[turn].rend(); ++it) {
			Action action = *it;
			resolveAction(action, reverse);
		}
	}
}

void ReplayMode::resolveAction(Action action, bool reverse) {
	string type = action.getType();
	if (type == "movePlayer") {
		int x1 = get<0>(action.moveFrom);
		int y1 = get<1>(action.moveFrom);
		int x2 = get<0>(action.moveTo);
		int y2 = get<1>(action.moveTo);
		if (reverse) this->window->replayPacmanMove(x2, y2, x1, y1);
		else this->window->replayPacmanMove(x1, y1, x2, y2);
	}
	else if (type == "moveGhost") {
		int x1 = get<0>(action.moveFrom);
		int y1 = get<1>(action.moveFrom);
		int x2 = get<0>(action.moveTo);
		int y2 = get<1>(action.moveTo);
		if (reverse) this->window->replayGhostMove(x2, y2, x1, y1);
		else this->window->replayGhostMove(x1, y1, x2, y2);
	}
	else if (type == "hp") {
		if (reverse) this->window->replayLive(action.hpTo, action.hpFrom);
		else this->window->replayLive(action.hpFrom, action.hpTo);
	}
	else if (type == "door") {
		if (reverse) this->window->replayDoor(!action.door);
		else this->window->replayDoor(action.door);
	}
	else if (type == "key") {
		this->window->replayKey(get<1>(action.key), get<0>(action.key));
	}
	else {
		throw runtime_error("Unknown action type: " + type);
	}
}

