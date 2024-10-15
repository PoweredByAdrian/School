/**
* @brief	MazeConfigure implementation
*
* @authors	Adrian Horvath(xhorva14)
*			Matúš Vráblik (xvrabl05)
*
*/

#include "MazeConfigure.h"

MazeConfigure::MazeConfigure(Logger* logger)
{
	this->logger = logger;
	this->newMaze = nullptr;
}

MazeConfigure::~MazeConfigure()
{
	delete this->newMaze;
}

void MazeConfigure::loadMapFromFile(std::string file)
{
	using namespace std;
	ifstream map(file);
	string lineText;
	if (!getline(map, lineText, ' ')) { return; }
	int rows = stoi(lineText);
	if (!getline(map, lineText)) { return; }
	int cols = stoi(lineText);
	this->startReading(rows, cols);
	while (getline(map, lineText))
	{
		this->processLine(lineText);
	}
	this->stopReading();
}

void MazeConfigure::startReading(int rows, int cols) {
	this->logger->printFirstLine(rows, cols);
	newMaze = new MazeClass(rows, cols);
}

bool MazeConfigure::processLine(std::string line) {
	this->logger->printMazeLine(line);
	this->counLines++;
	if (this->correctLine)
	{
		if (newMaze->numCols() != line.length() + 2)
		{
			return this->correctLine = false;
		}
		newMaze->insertLine(line);
	}
	return this->correctLine;
}

bool MazeConfigure::stopReading() {
	this->logger->printTurnZero();
	if (this->counLines != newMaze->numRows() - 2)
	{
		return false;
	}
	newMaze->setFields();
	return this->doneReading = true;
}

Maze* MazeConfigure::createMaze() {
	if (this->doneReading && this->correctLine)
	{
		return this->newMaze;
	}
	else
	{
		return nullptr;
	}
}
